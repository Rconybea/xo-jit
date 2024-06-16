/** @file LlvmContext.hpp
 *
 *  Author: Roland Conybeare
 **/

#pragma once

#include "xo/refcnt/Refcounted.hpp"
#include "llvm/IR/LLVMContext.h"
//#include <cstdint>

namespace xo {
    namespace jit {
        /** @class LlvmContext
         *  @brief Keepalive for a llvm::LLVMContext instance.
         *
         *  For example IrPipeline holds an rp<LlvmContext>
         *  to help ensure validity of embedded llvm::LLVMContext reference
         **/
        class LlvmContext : public ref::Refcount {
        public:
            static xo::ref::rp<LlvmContext> make();

            llvm::LLVMContext & llvm_cx_ref() { return *llvm_cx_; }
            std::unique_ptr<llvm::LLVMContext> & llvm_cx() { return llvm_cx_; }

        private:
            LlvmContext();

        private:
            /** Llvm context.  Ties together fragments of code generation
             *  for AST subtrees that go into the same module.
             **/
            std::unique_ptr<llvm::LLVMContext> llvm_cx_;
        }; /*LlvmContext*/
    } /*namespace jit*/
} /*namespace xo*/


/** end LlvmContext.hpp **/
