#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/Support/raw_ostream.h"

int main() {
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    llvm::Module *module = new llvm::Module("top", context);

    // Create main function and basic block
    llvm::FunctionType *functionType = llvm::FunctionType::get(builder.getInt32Ty(), false);
    llvm::Function *mainFunction = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "main", module);
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunction);
    builder.SetInsertPoint(entry);

    // Create a global string pointer
    llvm::Value *helloWorld = builder.CreateGlobalStringPtr("hello world\n");

    // Create function pointer for puts
    std::vector<llvm::Type *> putArgs;
    putArgs.push_back(builder.getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type *> argsRef(putArgs);
    llvm::FunctionType *putsType = llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
    /* = FunctionType + Callee-pointer */
    llvm::FunctionCallee putFunction_callee = module->getOrInsertFunction("puts", putsType);

#ifdef NOT_YET
    llvm::Constant * putFunction = llvm::Constant

    // Allocate memory for the function pointer
    llvm::Value *p = builder.CreateAlloca(putFunction->getType(), nullptr, "p");
    builder.CreateStore(putFunction, p, false);

    // Load the function pointer and call it
    llvm::Value *temp = builder.CreateLoad(p);
    builder.CreateCall(temp, helloWorld);

    // Return 0 to complete the main function
    builder.CreateRet(llvm::ConstantInt::get(builder.getInt32Ty(), 0));

    // Print the module (IR code)
    module->print(llvm::errs(), nullptr);
#endif
}
