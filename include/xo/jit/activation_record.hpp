/** @file activation_record.hpp
 *
 *  Author: Roland Conybeare
 **/

#pragma once

#include "LlvmContext.hpp"
#include "xo/expression/Lambda.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
# include <llvm/IR/IRBuilder.h>
# include <llvm/IR/Instructions.h>
#pragma GCC diagnostic pop
#include <map>
//#include <cstdint>

namespace xo {
    namespace jit {
        /** analagous to xo::ast::binding_path,
         *  but with locations renumbered to include only vars that belong to an explict runtime
         *  environment object;  in other words we exclude vars with stack-only storage
         **/
        struct runtime_binding_path {
        public:
            runtime_binding_path() = default;
            runtime_binding_path(int i_rt_link,
                                 int j_rt_slot)
                : i_rt_link_{i_rt_link}, j_rt_slot_{j_rt_slot} {}

            static runtime_binding_path stackonly() {
                return runtime_binding_path(0, -1);
            }
            static runtime_binding_path local(int j_rt_slot) {
                return runtime_binding_path(0, j_rt_slot);
            }

            bool is_stackonly() const { return (i_rt_link_ == 0) && (j_rt_slot_ == -1); }
            bool is_captured() const { return !is_stackonly(); }

        public:
            /** nnumber of parent runtime env links to traverse.  -1 if global.  -2 if sentinel **/
            int i_rt_link_ = -2;
            /** >= 0: slot# within explicit runtime environment where this variable bound.
             *  (local vars only -- ignored for global vars)
             *  -1: stack-only parameter
             **/
            int j_rt_slot_ = 0;
        };

        struct runtime_binding_detail {
            /** Formal index position for this formal parameter.
             *  Index into @ref activation_record::binding_v_,
             *  also for @ref Lambda::fn_arg
             **/
            int i_argno_ = -1;

            /** instructions for establishing stack address of this variable
             *  In practice will be either an AllocaInst (for non-captured variables),
             *  or result of IRBuilder<>::CreateInBoundsGEP (for captured variables).
             **/
            llvm::Value * llvm_addr_ = nullptr;

            /** llvm type associated with stack-allocated variable.
             *  Determines (when combined with llvm::DataLayout) how much space
             *  will be required for this particular variable
             **/
            llvm::Type * llvm_type_ = nullptr;
        };

        /**
         *  1. pattern for a stack frame associated with a user-defined function (some Lambda lm)
         *
         *  2. each function needs its own IR builder,  to keep track of things like insert point
         *
         *  3. simple case first.
         *     if lm->needs_closure_flag() is false, then:
         *
         *     a. still need a closure-shaped object,  because when we invoke function,  we may
         *        not know until runtime whether it relies on closure.
         *        For such function we will generate a closure with empty environment pointer.
         *     b. all formal parameters of lm
         *        are used only in the layer associated with that lambda's body;  in particular
         *        they aren't free in any nested lambda
         *     c. conversely,  the top layer of lm's body has no free variables.
         *        The only variables that *do* appear are lm's formal parameters.
         *
         *     In this case, all of lm's formals will be allocated on the stack using regular
         *     allocInst,  and we don't need a closure for lm.
         *
         *  4. complex case second
         *     If lm->needs_closure_flag() is true,  then either:
         *
         *     a. at least one formal parameter of lm appears free in some nested lambda.
         *     b. lambda's top layer itself contains one or more free variables.
         *
         *     In either case we will create an explicit environment for lm,
         *     containing all the variables needed by some nested lambda
         **/
        class activation_record {
        public:
            using Lambda = xo::ast::Lambda;
            using TypeDescr = xo::reflect::TypeDescr;

        public:
            activation_record(const ref::rp<Lambda> & lm);

            const ref::rp<Lambda> lambda() const { return lambda_; }

            /** retrieve @c llvm::Value* representing the primary stack location
             *  for formal parameter @p var_name
             **/
            const runtime_binding_detail * lookup_var(const std::string & var_name) const;

            /** Remember allocation of a function variable on the stack
             *
             *  @param var_name.  formal parameter name
             *  @param binding.   address + supporting details for
             *                    primary (stack-allocated) storage for this variable
             **/
            const runtime_binding_detail * alloc_var(const std::string & var_name,
                                                     const runtime_binding_detail & binding);

#ifdef NOT_USING
            llvm::AllocaInst * create_runtime_localenv_alloca(ref::brw<LlvmContext> llvm_cx,
                                                              //const llvm::DataLayout & data_layout,
                                                              llvm::Function * llvm_fn,
                                                              llvm::IRBuilder<> & fn_ir_builder);
#endif

            runtime_binding_detail create_entry_block_alloca(ref::brw<LlvmContext> llvm_cx,
                                                             //const llvm::DataLayout & data_layout,
                                                             llvm::Function * llvm_fn,
                                                             llvm::IRBuilder<> & fn_ir_builder,
                                                             int i_arg,
                                                             const std::string & var_name,
                                                             TypeDescr var_td);

            /** generate instructions that establish stacck location for a local-environment slot
             *
             *  @param llvm_cx.  handle for context -- manages storage for llvm::Types + related
             *  @param localenv_llvm_type.  describes contents of local environment
             *         for a particular function.  Same as @c localenv_alloca->getAllocatedType()
             *  @param localenv_alloca.  stack location for local environment
             *  @param i_slot.  0-based slot number within local environment,
             *         for which address is required
             *  @param fn_ir_builder.  insertion point for generated instructions
             *         that compute target slot address (will be at/near top of function,
             *         since we will copy captured function arguments to localenv,
             *         then use the localenv copy exclusively.
             *  @return value representing localenv slot address
             **/
            llvm::Value * runtime_localenv_slot_addr(ref::brw<LlvmContext> llvm_cx,
                                                     llvm::StructType * localenv_llvm_type,
                                                     llvm::AllocaInst * localenv_alloca,
                                                     int i_slot,
                                                     llvm::IRBuilder<> & fn_ir_builder);

            /** establish storage for formal parameters on behalf of a new-but-empty
             *  llvm function @p llvm_fn.  Creates llvm IR instructions on function
             *  entry that
             *  1. allocates stack space for function parameters.
             *  2. stores incoming parameters in that stack space.
             *
             *  Strategy:
             *  - for stackonly parameters, use individual @c llvm::AllocaInst instances
             *  - create custom @c llvm::StructType for captured parameters,  also initially stack-allocated
             **/
            bool bind_locals(ref::brw<LlvmContext> llvm_cx,
                             //const llvm::DataLayout & data_layout,
                             llvm::Function * llvm_fn,
                             llvm::IRBuilder<> & ir_builder);

        private:
            /** this activation record created on behalf of a call to @ref lambda_.
             *  @ref Variable::path_ specifies a logical path to a variable,
             *  but does not distinguish stack-native variables from variables in explicit
             *  runtime environment records.
             *
             **/
            ref::rp<Lambda> lambda_;

            /** @c binding_v_[i] specifies how/where we mean to navigate to
             *  location for formal parameter number *i* of @ref lambda_.
             **/
            std::vector<runtime_binding_path> binding_v_;

            /** if this function requires an explicit environment,
             *  gives stack location for that environment.
             **/
            llvm::AllocaInst * localenv_alloca_ = nullptr;

            /** maps named slots in a stack frame to logical addresses.
             *
             *  - For captured arguments: will refer to slot within stack-allocated local environment
             *    (an llvm::StructType, created by type2llvm::create_localenv_llvm_type())
             *
             *  - For non-captured arguments: will refer to stack-allocated argument copy
             *
             *  In either case using copy-to-stack to evade directly confronting
             *  so we don't have to comply with llvm IR's SSA requirement.
             **/
            std::map<std::string, runtime_binding_detail> frame_; /* <-> kaleidoscope NamedValues */
        }; /*activation_record*/

    } /*namespace jit*/
} /*namespace xo*/


/** end activation_record.hpp **/
