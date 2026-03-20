#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
class ClassVisitor : public RecursiveASTVisitor<ClassVisitor> {
public:
  bool VisitCXXRecordDecl(CXXRecordDecl *D) {
    if (!D->isThisDeclarationADefinition())
      return true;

    if (!D->isStruct() && !D->isClass())
      return true;

    if (D->isImplicit())
      return true;

    printClass(D);
    return true;
  }

private:
  void printClass(CXXRecordDecl *D) {
    llvm::outs() << D->getNameAsString();

    if (D->getNumBases() > 0) {
      llvm::outs() << " -> ";
      for (auto &Base : D->bases()) {
        const Type *T = Base.getType().getTypePtr();
        if (const RecordType *RT = T->getAs<RecordType>()) {
          llvm::outs() << RT->getDecl()->getNameAsString();
        }
      }
    }

    llvm::outs() << "\n";

    printFields(D);
    printMethods(D);
    llvm::outs() << "\n";
  }

  void printFields(CXXRecordDecl *D) {
    llvm::outs() << "|_Fields\n";

    for (auto *Field : D->fields()) {
      llvm::outs() << "| |_ " << Field->getNameAsString() << " ("
                   << Field->getType().getAsString() << "|"
                   << getAccess(Field->getAccess()) << ")\n";
    }
  }

  void printMethods(CXXRecordDecl *D) {
    llvm::outs() << "|\n|_Methods\n";

    for (auto *Method : D->methods()) {
      // Skip implicit compiler-generated methods
      if (Method->isImplicit())
        continue;

      llvm::outs() << "| |_ " << Method->getNameAsString() << " ("
                   << getSignature(Method) << "|"
                   << getAccess(Method->getAccess());

      if (Method->isVirtual())
        llvm::outs() << "|virtual";

      if (Method->size_overridden_methods() > 0)
        llvm::outs() << "|override";

      if (Method->isPureVirtual())
        llvm::outs() << "|pure";

      llvm::outs() << ")\n";
    }
  }

  std::string getAccess(AccessSpecifier AS) {
    switch (AS) {
    case AS_public:
      return "public";
    case AS_protected:
      return "protected";
    case AS_private:
      return "private";
    default:
      return "none";
    }
  }

  std::string getSignature(CXXMethodDecl *M) {
    std::string result = M->getReturnType().getAsString();
    result += "(";

    for (unsigned i = 0; i < M->getNumParams(); ++i) {
      if (i > 0)
        result += ", ";
      result += M->getParamDecl(i)->getType().getAsString();
    }

    result += ")";
    return result;
  }
};

class ClassConsumer : public ASTConsumer {
public:
  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  ClassVisitor Visitor;
};

class ClassPluginAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<ClassConsumer>();
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }
};

static FrontendPluginRegistry::Add<ClassPluginAction>
    X("print-classes", "Print class structure");
