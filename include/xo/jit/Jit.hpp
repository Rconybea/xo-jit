/** @file Jit.hpp
 *
 *  Author: Roland Conybeare
 **/

#pragma once

//#include <cstdint>

#include "xo/refcnt/Refcounted.hpp"
#include "xo/expression/Expression.hpp"
#include "xo/expression/ConstantInterface.hpp"
#include "xo/expression/PrimitiveInterface.hpp"
#include "xo/expression/Apply.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

namespace xo {
    namespace jit {
        /** @class Jit
         *  @brief just-in-time compiler for EGAD
         *
         *  TODO: make module name a parameter?
        **/
        class Jit : public ref::Refcount {
        public:
            using Expression = xo::ast::Expression;
            //using ConstantInterface = xo::ast::ConstantInterface;

        public:
            static ref::rp<Jit> make() { return new Jit(); }

            llvm::Value * codegen_constant(ref::brw<xo::ast::ConstantInterface> expr);
            llvm::Function * codegen_primitive(ref::brw<xo::ast::PrimitiveInterface> expr);
            llvm::Value * codegen_apply(ref::brw<xo::ast::Apply> expr);

            llvm::Value * codegen(ref::brw<Expression> expr);

        private:
            Jit();

        private:
            /** owns + manages core "global" llvm data,
             *  including type- and constant- unique-ing tables.
             *
             *  Not threadsafe,  but ok to have multiple threads,
             *  each with its own LLVMContext
             **/
            std::unique_ptr<llvm::LLVMContext> llvm_cx_;
            /** builder for intermediate-representation objects **/
            std::unique_ptr<llvm::IRBuilder<>> llvm_ir_builder_;
            /** a module (aka library) being prepared by llvm.
             *  - function names are unique within a module.
             **/
            std::unique_ptr<llvm::Module> llvm_module_;
        };
    } /*namespace jit*/
} /*namespace xo*/


/** end Jit.hpp **/
