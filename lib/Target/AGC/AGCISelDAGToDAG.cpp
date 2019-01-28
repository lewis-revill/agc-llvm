//===-- AGCISelDAGToDAG.cpp - A DAG to DAG instruction selector for AGC ---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares an instruction selector for the AGC target.
//
//===----------------------------------------------------------------------===//

#include "AGC.h"
#include "AGCISelDAGToDAG.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

#define DEBUG_TYPE "agc-isel"

void AGCDAGToDAGISel::Select(SDNode *Node) {
  // If we have a custom node, we have already selected
  if (Node->isMachineOpcode()) {
    LLVM_DEBUG(dbgs() << "== " << Node << "\n");
    Node->setNodeId(-1);
    return;
  }

  switch (Node->getOpcode()) {
  default:
    break;
  case ISD::Constant: {
    SelectAGCConstant(Node);
    return;
  }
  }

  for (auto &Op : Node->ops()) {
    switch (Op.getNode()->getOpcode()) {
    default:
      break;
    case ISD::Constant: {
      SelectAGCConstant(Op.getNode());
      break;
    }
    }
  }

  // Select the default instruction.
  SelectCode(Node);
}

void AGCDAGToDAGISel::SelectAGCConstant(SDNode *Node) {
  assert(Node->getValueType(0) == MVT::i16 &&
         "Non-i16 constants cannot yet be materialized");

  SDLoc DL(Node);

  int64_t ConstImm = cast<ConstantSDNode>(Node)->getSExtValue();
  SDValue ConstValue = CurDAG->getTargetConstant(ConstImm, DL, MVT::i16);
  // Wrap constant nodes with a pseudo instruction that we can materialize later
  // for the appropriate data representation.
  SDNode *New =
      CurDAG->getMachineNode(AGC::PseudoCONST, DL, MVT::i16, ConstValue);
  ReplaceNode(Node, New);
}

// This pass converts a legalized DAG into an AGC-specific DAG, ready for
// instruction scheduling.
FunctionPass *llvm::createAGCISelDag(AGCTargetMachine &TM) {
  return new AGCDAGToDAGISel(TM);
}
