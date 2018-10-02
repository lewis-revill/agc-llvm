set(LLVM_TARGET_DEFINITIONS AGC.td)

tablegen(LLVM AGCGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM AGCGenInstrInfo.inc -gen-instr-info)

add_public_tablegen_target(AGCCommonTableGen)

add_llvm_target(AGCCodeGen
  AGCTargetMachine.cpp
  )

add_subdirectory(TargetInfo)
