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

  // Select the default instruction.
  SelectCode(Node);
}

// This pass converts a legalized DAG into an AGC-specific DAG, ready for
// instruction scheduling.
FunctionPass *llvm::createAGCISelDag(AGCTargetMachine &TM) {
  return new AGCDAGToDAGISel(TM);
}
