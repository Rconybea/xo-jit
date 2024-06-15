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
#include "xo/expression/Lambda.hpp"
#include "xo/expression/Variable.hpp"

#include "KaleidoscopeJit.hpp"
#ifdef NOT_USING
/* stuff from KaleidoscopeJIT.hpp */
#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutorProcessControl.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/Shared/ExecutorSymbolDef.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include <memory>
#endif

/* stuff from kaleidoscope.cpp */
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
            /* tracking KaleidoscopeJIT::Create() here.. */
            static llvm::Expected<std::unique_ptr<Jit>> make_aux();
            static xo::ref::rp<Jit> make();

            // ----- module access -----

            /** target triple = string describing target host for codegen **/
            const std::string & target_triple() const;
            /** append function names defined in attached module to *p_v **/
            std::vector<std::string> get_function_name_v();

            /** write state of execution session (all the associated dynamic libraries) **/
            void dump_execution_session();

            // ----- jit code generation -----

            llvm::Value * codegen_constant(ref::brw<xo::ast::ConstantInterface> expr);
            llvm::Function * codegen_primitive(ref::brw<xo::ast::PrimitiveInterface> expr);
            llvm::Value * codegen_apply(ref::brw<xo::ast::Apply> expr);
            llvm::Function * codegen_lambda(ref::brw<xo::ast::Lambda> expr);
            llvm::Value * codegen_variable(ref::brw<xo::ast::Variable> var);

            llvm::Value * codegen(ref::brw<Expression> expr);

            virtual void display(std::ostream & os) const;
            virtual std::string display_string() const;

        private:
            Jit(
                std::unique_ptr<KaleidoscopeJIT> kal_jit
#ifdef NOT_USING
                std::unique_ptr<llvm::orc::ExecutionSession> es,
                llvm::orc::JITTargetMachineBuilder jtmb,
                llvm::DataLayout dl
#endif
                );

            /* iniitialize native builder (i.e. for platform we're running on) */
            static void init_once();

        private:
            // ----- this part adapted from LLVM 19.0 KaleidoscopeJIT.hpp [wip] -----

            std::unique_ptr<KaleidoscopeJIT> kal_jit_;

#ifdef NOT_USING
            std::unique_ptr<llvm::orc::ExecutionSession> jit_es_;
            llvm::DataLayout jit_data_layout_;
            llvm::orc::MangleAndInterner jit_mangle_;
            llvm::orc::RTDyldObjectLinkingLayer jit_object_layer_;
            llvm::orc::IRCompileLayer jit_compile_layer_;
            /** reference here.  looks like storage owned by .jit_es **/
            llvm::orc::JITDylib & jit_our_dynamic_lib_;
#endif

            // ----- this part adapted from kaleidoscope.cpp -----

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

            /** map global names to functions/variables **/
            std::map<std::string, xo::ref::rp<Expression>> global_env_;
            /** map variable names (formal parameters) to
             *  corresponding llvm interactor
             **/
            std::map<std::string, llvm::Value*> nested_env_;
        }; /*Jit*/

        inline std::ostream &
        operator<<(std::ostream & os, const Jit & x) {
            x.display(os);
            return os;
        }
    } /*namespace jit*/
} /*namespace xo*/


/** end Jit.hpp **/
