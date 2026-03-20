#ifndef LLVM_TRANSFORMS_UTILS_INSTRUMENTFUNCPASS_H
#define LLVM_TRANSFORMS_UTILS_INSTRUMENTFUNCPASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class InstrumentFuncPass : public PassInfoMixin<InstrumentFuncPass> {
public:
  PreservedAnalyses run(Module &F, ModuleAnalysisManager &AM);
};

} // namespace llvm

#endif
