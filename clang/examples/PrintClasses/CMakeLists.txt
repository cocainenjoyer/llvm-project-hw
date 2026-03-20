add_llvm_library(PrintClassesPlugin MODULE
  PrintClasses.cpp
)

set_target_properties(PrintClassesPlugin PROPERTIES
  PLUGIN_TOOL clang
  POSITION_INDEPENDENT_CODE ON
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
)
