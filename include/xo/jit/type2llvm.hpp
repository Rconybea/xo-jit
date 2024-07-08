/** @file type2llvm.hpp
 *
 *  Author: Roland Conybeare
 **/

#pragma once

#include "LlvmContext.hpp"
#include "xo/expression/Lambda.hpp"
#include "xo/reflect/TypeDescr.hpp"
#include <llvm/IR/DerivedTypes.h>
//#include <cstdint>

namespace xo {
    namespace jit {
        /**
         **/
        struct type2llvm {
        public:
            using Lambda = xo::ast::Lambda;
            using TypeDescr = xo::reflect::TypeDescr;

        public:
            /** establish suitable llvm representation for a c++ type (described by @p td)
             *  llvm types are unique'd, at least within @p llvm_cx
             **/
            static llvm::Type * td_to_llvm_type(xo::ref::brw<LlvmContext> llvm_cx,
                                                TypeDescr td);

            /** establish llvm representation for a function type
             *  described by @p fn_td
             *
             *  @param wrapper_flag  If true, create function type for a wrapper
             *                       to be associated with a closure.
             *  The wrapper accepts (and ignores) an envapi pointer as first argument.
             *  Necessary to (for example) support function pointers that may refer
             *  to either {primitive functions, functions-requiring-closures},
             *  with choice deferred until runtime
             **/
            static llvm::FunctionType * function_td_to_llvm_type(xo::ref::brw<LlvmContext> llvm_cx,
                                                                 TypeDescr fn_td,
                                                                 bool wrapper_flag = false);

            /** establish llvm concrete representation for a closure.
             *
             *                  +-------+
             *              [0] |   o-------> fnptr  T (*)(envptr, ...)
             *                  +-------+
             *              [1] |   o-------\
             *                  +-------+   |
             *                              |
             *                              |
             *                              v
             *                          +-------+
             *           parent_env [0] |   o-------> _env_api*
             *                          +-------+
             *           unwind_fn  [1] |   o-------> env * (*)(env*, ctl)
             *                          +-------+
             *
             * @return struct type.  typename will be @c c.foo for lambda with name @c foo
             **/
            static llvm::StructType *
            create_closure_lvtype(xo::ref::brw<LlvmContext> llvm_cx,
                                  xo::ref::brw<Lambda> lambda);

            /** establish llvm concrete representation for a particular lambda's
             *  runtime local environment:
             *
             *                              ^
             *                              | parent
             *                  +-------+   |
             *   parent_env [0] |   o-------/
             *                  +-------+
             *   unwind_fn  [1] |   o-------> env * (*)(env*, ctl)
             *                  +-------+
             *   arg[i]   [2+i] .  ...  .
             *                  .  ...  .
             *                  +-------+
             *
             *   ctl=0  unwind.  finalization for any arg[i] that requires it.
             *                   returns nullptr
             *   ctl=1  copy.    copy runtime environment to heap destination
             *                   and return address of the copy
             *
             *   arg[] comprises the subset of lambda arg names arg[j] for which
             *   lambda->is_captured(arg[j]) is true
             *
             * @return struct type.  typename will be @c e.foo for lambda with name @c foo
             **/
            static llvm::StructType *
            create_localenv_llvm_type(xo::ref::brw<LlvmContext> llvm_cx,
                                      xo::ref::brw<Lambda> lambda);

            /** establish llvm rep'n for a pointer to an abstract local environment:
             *
             *    +-------+
             *    |   o-------------\
             *    +-------+         |
             *                      |
             *                      |
             *                      |
             *                      v
             *                  +-------+
             *   parent_env [0] |   o-------> _env_api*
             *                  +-------+
             *   unwind_fn  [1] |   o-------> env * (*)(env*, ctl)
             *                  +-------+
             **/
            static llvm::PointerType *
            env_api_llvm_ptr_type(xo::ref::brw<LlvmContext> llvm_cx);

            /** function type:
             *  @code
             *    env_api_* (env_api* env, int ctl);
             *  @endcode
             *
             *   ctl=0  unwind.  finalization for any arg[i] that requires it.
             *                   returns nullptr
             *   ctl=1  copy.    copy runtime environment to heap destination
             *                   and return address of the copy
             *
             * returns function-pointer type
             **/
            static llvm::PointerType *
            require_localenv_unwind_llvm_fnptr_type(xo::ref::brw<LlvmContext> llvm_cx,
                                                    llvm::PointerType * hint_envptr_llvm_type = nullptr);

        private:
            /** establish llvm representation for a function-pointer type
             *  described by @p fn_td
             **/
            static llvm::PointerType * function_td_to_llvm_fnptr_type(xo::ref::brw<LlvmContext> llvm_cx,
                                                                      TypeDescr fn_td);

            /** establish llvm representation for a struct type described by @p struct_td
             **/
            static llvm::StructType * struct_td_to_llvm_type(xo::ref::brw<LlvmContext> llvm_cx,
                                                             TypeDescr struct_td);

            /** establish llvm representation for a pointer type described by @p pointer_td **/
            static llvm::PointerType * pointer_td_to_llvm_type(xo::ref::brw<LlvmContext> llvm_cx,
                                                               TypeDescr pointer_td);

            /** establish llvm abstract representation for a local environment:
             *
             *                              ^
             *                              | parent
             *                  +-------+   |
             *   parent_env [0] |   o-------/
             *                  +-------+
             *   unwind_fn  [1] |   o-------> env * (*)(env*, ctl)
             *                  +-------+
             *
             *   ctl=0  unwind.  finalization for any arg[i] that requires it.
             *                   returns nullptr
             *   ctl=1  copy.    copy runtime environment to heap destination
             *                   and return address of the copy
             *
             * Concrete implementation will probably occupy additional memory,
             * to store captured lambda variables.
             *
             * @see type2llvm::function_td_to_llvm_closure_type
             **/
            static llvm::StructType *
            env_api_llvm_type(xo::ref::brw<LlvmContext> llvm_cx);

            /** establish llvm abstract representation for a closure:
             *  struct with
             *  - [0] function pointer
             *  - [1] runtime localenv pointer
             *
             *                  +-------+
             *                  |   o---------> native function
             *                  +-------+
             *                  |   o---------> runtime localenv
             *                  +-------+       (possibly nullptr)
             *
             *  1. for primitives, localenv will be null pointer
             *  2. for lambdas L with L->requires_closure_flag() = false,
             *     localenv will also be null pointer
             *  3. for lambdas with L->requires_closure_flag() = true,
             *
             *  localenv will (for lambdas requiring closures)
             *  in practice be struct:
             *
             *                              ^
             *                              | parent
             *                  +-------+   |
             *   parent_env [0] |   o-------/
             *                  +-------+
             *   unwind_fn  [1] |   o-------> env * (*)(env*, ctl)
             *                  +-------+
             *   arg[i]   [2+i] .  ...  .
             *                  .  ...  .
             *                  +-------+
             *
             *   ctl=0  unwind.  finalization for any arg[i] that requires it.
             *                   returns nullptr
             *   ctl=1  copy.    copy runtime environment to heap destination
             *                   and return address of the copy
             *
             *  Implementation here will just use generic pointer for runtime
             *  localenv.
             **/
            static llvm::StructType *
            function_td_to_llvm_closure_type(xo::ref::brw<LlvmContext> llvm_cx,
                                             TypeDescr fn_td);

        }; /*type2llvm*/
    } /*namespace jit*/
} /*namespace xo*/

/** end type2llvm.hpp **/
