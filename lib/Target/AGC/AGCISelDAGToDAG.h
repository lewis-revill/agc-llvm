//===-- AGCISelDAGToDAG.cpp - A DAG to DAG instruction selector for AGC ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares an instruction selector for the AGC target.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_AGCISELDAGTODAG_H
#define LLVM_LIB_TARGET_AGC_AGCISELDAGTODAG_H

#include "AGCTargetMachine.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

namespace llvm {
class AGCDAGToDAGISel final : public SelectionDAGISel {
public:
  explicit AGCDAGToDAGISel(AGCTargetMachine &TM) : SelectionDAGISel(TM) {}

  StringRef getPassName() const override {
    return "AGC DAG->DAG Pattern Instruction Selection";
  }

  void Select(SDNode *Node) override;

#include "AGCGenDAGISel.inc"
private:
  void SelectAGCConstant(SDNode *Node);
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_AGC_AGCISELDAGTODAG_H
