/* @file IrPipeline.cpp */

#include "IrPipeline.hpp"

namespace xo {
    namespace jit {
        IrPipeline::IrPipeline(llvm::LLVMContext & llvm_cx)
        {
            using std::make_unique;

            this->llvm_fpmgr_ = make_unique<llvm::FunctionPassManager>();
            this->llvm_lamgr_ = std::make_unique<llvm::LoopAnalysisManager>();
            this->llvm_famgr_ = std::make_unique<llvm::FunctionAnalysisManager>();
            this->llvm_cgamgr_ = std::make_unique<llvm::CGSCCAnalysisManager>();
            this->llvm_mamgr_ = std::make_unique<llvm::ModuleAnalysisManager>();
            this->llvm_pic_ = std::make_unique<llvm::PassInstrumentationCallbacks>();
            this->llvm_si_ = std::make_unique<llvm::StandardInstrumentations>(llvm_cx,
                                                                              /*DebugLogging*/ true);

            this->llvm_si_->registerCallbacks(*llvm_pic_, llvm_mamgr_.get());

            /** transform passes **/
            this->llvm_fpmgr_->addPass(llvm::InstCombinePass());
            this->llvm_fpmgr_->addPass(llvm::ReassociatePass());
            this->llvm_fpmgr_->addPass(llvm::GVNPass());
            this->llvm_fpmgr_->addPass(llvm::SimplifyCFGPass());

            /** tracking for analysis passes that share info? **/
            llvm::PassBuilder llvm_pass_builder;
            llvm_pass_builder.registerModuleAnalyses(*llvm_mamgr_);
            llvm_pass_builder.registerFunctionAnalyses(*llvm_famgr_);
            llvm_pass_builder.crossRegisterProxies(*llvm_lamgr_, *llvm_famgr_, *llvm_cgamgr_, *llvm_mamgr_);
        } /*ctor*/

        void
        IrPipeline::run_pipeline(llvm::Function & fn)
        {
            llvm_fpmgr_->run(fn, *llvm_famgr_);
        } /*run_pipeline*/
    } /*namespace jit*/
} /*namespace xo*/



/* end IrPipeline.cpp */
