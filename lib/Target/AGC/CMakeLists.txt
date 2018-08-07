set(LLVM_TARGET_DEFINITIONS AGC.td)

add_llvm_target(AGCCodeGen
  AGCTargetMachine.cpp
  )

add_subdirectory(TargetInfo)
