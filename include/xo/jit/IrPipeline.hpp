/** @file IrPipeline.hpp
 *
 *  Author: Roland Conybeare
 **/

#pragma once

#include "xo/refcnt/Refcounted.hpp"
#include "LlvmContext.hpp"

/* stuff from kaleidoscope.cpp */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
# include "llvm/ADT/APFloat.h"
# include "llvm/ADT/STLExtras.h"
# include "llvm/IR/BasicBlock.h"
# include "llvm/IR/Constants.h"
# include "llvm/IR/DerivedTypes.h"
# include "llvm/IR/Function.h"
# include "llvm/IR/IRBuilder.h"
# include "llvm/IR/LLVMContext.h"
# include "llvm/IR/Module.h"
# include "llvm/IR/PassManager.h"
# include "llvm/IR/Type.h"
# include "llvm/IR/Verifier.h"
# include "llvm/Passes/PassBuilder.h"
# include "llvm/Passes/StandardInstrumentations.h"
# include "llvm/Support/TargetSelect.h"
# include "llvm/Target/TargetMachine.h"
# include "llvm/Transforms/InstCombine/InstCombine.h"
# include "llvm/Transforms/Scalar.h"
# include "llvm/Transforms/Scalar/GVN.h"
# include "llvm/Transforms/Utils/Mem2Reg.h"
# include "llvm/Transforms/Scalar/Reassociate.h"
# include "llvm/Transforms/Scalar/SimplifyCFG.h"
#pragma GCC diagnostic pop

//#include <cstdint>

namespace xo {
    namespace jit {
        /** @class IrPipeline
         *  @brief represent an LLVM IR pipeline
         *
         *  Represents analysis/transformation short of generating
         *  machine-code.   For now pipeline stages are hardwired;
         *  adapted from the LLVM Kaleidoscope example project.
         *
         *  Conversely,  pipeline *starts* with code already that has
         *  already been expressed in LLVM IR
         **/
        class IrPipeline : public ref::Refcount {
        public:
            explicit IrPipeline(ref::rp<LlvmContext> llvm_cx);

            void run_pipeline(llvm::Function & fn);

        private:
            // ----- transforms (also adapted from kaleidescope.cpp) ------

            /** keepalive for contained llvm::LLVMContext **/
            ref::rp<LlvmContext> llvm_cx_;

            /** manages all the passes+analaysis (?) **/
            std::unique_ptr<llvm::FunctionPassManager> llvm_fpmgr_;
            /** loop analysis (?) **/
            std::unique_ptr<llvm::LoopAnalysisManager> llvm_lamgr_;
            /** function-level analysis (?) **/
            std::unique_ptr<llvm::FunctionAnalysisManager> llvm_famgr_;
            /** cgscc (?) analysis **/
            std::unique_ptr<llvm::CGSCCAnalysisManager> llvm_cgamgr_;
            /** module analsyis (?) **/
            std::unique_ptr<llvm::ModuleAnalysisManager> llvm_mamgr_;
            /** pass instrumentation **/
            std::unique_ptr<llvm::PassInstrumentationCallbacks> llvm_pic_;
            /** standard instrumentation **/
            std::unique_ptr<llvm::StandardInstrumentations> llvm_si_;
        }; /*IrPipeline*/
    } /*namespace jit*/
} /*namespace xo*/


/** end IrPipeline.hpp **/
