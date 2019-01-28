//===-- AGCISelLowering.cpp - AGC DAG Lowering Implementation  ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that AGC uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#include "AGCISelLowering.h"
#include "AGCSubtarget.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/CodeGen/CallingConvLower.h"

using namespace llvm;

#define DEBUG_TYPE "agc-lower"

AGCTargetLowering::AGCTargetLowering(const TargetMachine &TM,
                                     const AGCSubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {

  // Ensure TLI knows about the register classes, even though they aren't used
  // in the same way as most register classes would be.
  addRegisterClass(MVT::i16, &AGC::AccRegClass);
  addRegisterClass(MVT::i16, &AGC::AccLowerRegClass);
  addRegisterClass(MVT::i32, &AGC::AccPairRegClass);

  // Compute derived properties from the register classes.
  computeRegisterProperties(STI.getRegisterInfo());

  setOperationAction(ISD::MUL, MVT::i32, Custom);
  setOperationAction(ISD::SDIV, MVT::i32, Custom);
  setOperationAction(ISD::SREM, MVT::i32, Custom);
}

SDValue AGCTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    report_fatal_error("unimplemented operation");
  case ISD::MUL:
    return LowerMUL(Op, DAG);
  case ISD::SDIV:
    return LowerSDIV(Op, DAG);
  case ISD::SREM:
    return LowerSREM(Op, DAG);
  }
}

void AGCTargetLowering::ReplaceNodeResults(SDNode *N,
                                           SmallVectorImpl<SDValue> &Results,
                                           SelectionDAG &DAG) const {
  switch (N->getOpcode()) {
  default:
    SDValue Res = LowerOperation(SDValue(N, 0), DAG);
    for (unsigned I = 0, E = Res->getNumValues(); I != E; ++I)
      Results.push_back(Res.getValue(I));
    break;
  }
}

// Calling Convention Implementation.
#include "AGCGenCallingConv.inc"

SDValue AGCTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  switch (CallConv) {
  default:
    report_fatal_error("unsupported calling convention");
  case CallingConv::C:
    break;
  }

  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();

  if (IsVarArg)
    report_fatal_error("vararg not yet supported");

  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_AGC);

  for (auto &VA : ArgLocs) {
    if (!VA.isRegLoc())
      report_fatal_error("stack arguments not yet supported");

    // Arguments passed in the upper/lower accumulators and scratch 'registers'.
    EVT RegVT = VA.getLocVT();
    if (RegVT != MVT::i32 && RegVT != MVT::i16) {
      LLVM_DEBUG(dbgs() << "LowerFormalArguments Unhandled argument type: "
                        << RegVT.getEVTString() << "\n");
      report_fatal_error("unhandled argument type");
    }

    unsigned VReg = RegInfo.createVirtualRegister(
        RegVT == MVT::i32 ? &AGC::MMD10RegClass : &AGC::MM10RegClass);
    RegInfo.addLiveIn(VA.getLocReg(), VReg);
    SDValue ArgIn = DAG.getCopyFromReg(Chain, DL, VReg, RegVT);

    InVals.push_back(ArgIn);
  }

  return Chain;
}

SDValue
AGCTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::OutputArg> &Outs,
                               const SmallVectorImpl<SDValue> &OutVals,
                               const SDLoc &DL, SelectionDAG &DAG) const {
  if (IsVarArg)
    report_fatal_error("vararg not yet supported");

  // Stores the assignment of the return value to a location.
  SmallVector<CCValAssign, 16> RVLocs;

  // Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeReturn(Outs, RetCC_AGC);

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the upper and lower accumulators.
  for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in upper and lower accumulators!");

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Flag);

    // Guarantee that all emitted copies are stuck together.
    Flag = Chain.getValue(1);
    SDValue ArgOut;
    if (VA.getValNo() == 0) {
      ArgOut = DAG.getRegister(VA.getLocReg(), VA.getLocVT());
    } else if (VA.getValNo() == 1) {
      // TODO: Ensure values returned in the lower accumulator can be no greater
      // than 15 bits.
      ArgOut = DAG.getRegister(VA.getLocReg(), VA.getLocVT());
    } else
      report_fatal_error("more than two register outputs");
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain; // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  return DAG.getNode(AGCISD::RET_FLAG, DL, MVT::Other, RetOps);
}

SDValue AGCTargetLowering::LowerMUL(SDValue Op, SelectionDAG &DAG) const {
  assert(Op.getValueType() == MVT::i32 && "MUL should be i32 only");

  if (Op.getOperand(0).getOpcode() != ISD::SIGN_EXTEND ||
      Op.getOperand(1).getOpcode() != ISD::SIGN_EXTEND)
    report_fatal_error("cannot lower MUL");

  SDValue Arg0 = Op.getOperand(0).getOperand(0);
  SDValue Arg1 = Op.getOperand(1).getOperand(0);

  if (Arg0.getValueType() != MVT::i16 || Arg1.getValueType() != MVT::i16)
    report_fatal_error("cannot lower MUL");

  // Replace the node (mul (sign_extend arg0), (sign_extend arg1)) with
  // (MP arg0, arg1).
  return
      SDValue(DAG.getMachineNode(AGC::MP, SDLoc(Op), MVT::i32, Arg0, Arg1), 0);
}

SDValue AGCTargetLowering::LowerSDIV(SDValue Op, SelectionDAG &DAG) const {
  assert(Op.getValueType() == MVT::i32 && "SDIV should be i32 only");

  if (Op.getOperand(1).getOpcode() != ISD::SIGN_EXTEND)
    report_fatal_error("cannot lower SDIV");

  SDValue Denominator = Op.getOperand(1).getOperand(0);

  if (Denominator.getValueType() != MVT::i16)
    report_fatal_error("cannot lower SDIV");

  // Replace the node (sdiv numerator, (sign_extend denominator)) with
  // (sign_extend (DV numerator, denominator)[0]).
  SDValue Div =
      SDValue(DAG.getMachineNode(AGC::DV, SDLoc(Op), MVT::i16, MVT::i16,
                                 Op.getOperand(0), Denominator), 0);
  return DAG.getNode(ISD::SIGN_EXTEND, SDLoc(Op), MVT::i32, Div);
}

SDValue AGCTargetLowering::LowerSREM(SDValue Op, SelectionDAG &DAG) const {
  assert(Op.getValueType() == MVT::i32 && "SREM should be i32 only");

  if (Op.getOperand(1).getOpcode() != ISD::SIGN_EXTEND)
    report_fatal_error("cannot lower SREM");

  SDValue Denominator = Op.getOperand(1).getOperand(0);

  if (Denominator.getValueType() != MVT::i16)
    report_fatal_error("cannot lower SREM");

  // Replace the node (srem numerator, (sign_extend denominator)) with
  // (sign_extend (DV numerator, denominator)[1]).
  SDValue Div =
      SDValue(DAG.getMachineNode(AGC::DV, SDLoc(Op), MVT::i16, MVT::i16,
                                 Op.getOperand(0), Denominator), 1);
  return DAG.getNode(ISD::SIGN_EXTEND, SDLoc(Op), MVT::i32, Div);
}

const char *AGCTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch ((AGCISD::NodeType)Opcode) {
  default:
    break;
  case AGCISD::RET_FLAG:
    return "AGCISD::RET_FLAG";
  }
  return nullptr;
}
