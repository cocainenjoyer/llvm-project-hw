#include "llvm/Transforms/Instrumentation/InstrumentFunc.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"

using namespace llvm;

PreservedAnalyses InstrumentFuncPass::run(Module &M, ModuleAnalysisManager &) {
  LLVMContext &Ctx = M.getContext();
  FunctionType *funcType = FunctionType::get(Type::getVoidTy(Ctx), false);

  FunctionCallee instrumentStart =
      M.getOrInsertFunction("instrument_start", funcType);
  FunctionCallee instrumentEnd =
      M.getOrInsertFunction("instrument_end", funcType);

  for (Function &F : M) {
    if (F.isDeclaration())
      continue;

    // Вставляем в начало
    IRBuilder<> builder(&*F.getEntryBlock().getFirstInsertionPt());
    builder.CreateCall(instrumentStart);

    // Вставляем перед каждым return
    for (BasicBlock &BB : F) {
      if (ReturnInst *RI = dyn_cast<ReturnInst>(BB.getTerminator())) {
        IRBuilder<> retBuilder(RI);
        retBuilder.CreateCall(instrumentEnd);
      }
    }
  }

  return PreservedAnalyses::none();
}
