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

            static runtime_binding_path local(int j_rt_slot) {
                return runtime_binding_path(0, j_rt_slot);
            }

        public:
            /** nnumber of parent runtime env links to traverse.  -1 if global.  -2 if sentinel **/
            int i_rt_link_ = -2;
            /** slot# within runtime environment where this variable bound.
             *  (local vars only -- ignored for global vars)
             **/
            int j_rt_slot_ = 0;
        };

        /**
         *  1. pattern for a stack frame associated with a user-defined function (some Lambda lm)
         *
         *  2. each function needs its own IR builder,  to keep track of things like insert point
         *
         *  3. simple case first.
         *     if lm->needs_closure_flag() is false, then
         *
         *     a. all formal parameters of lm
         *        are used only in the layer associated with that lambda's body;  in particular
         *        they aren't free in any nested lambda
         *     b. conversely,  the top layer of lm's body has no free variables.
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

        public:
            activation_record(const ref::rp<Lambda> & lm)
                : lambda_{lm} {}

            llvm::AllocaInst * lookup_var(const std::string & var_name) const;

            /**
             *  @p j_slot  index number (0-based) for var_name in formal parameter list for
             *             its originating lambda
             **/
            llvm::AllocaInst * alloc_var(std::size_t j_slot,
                                         const std::string & var_name,
                                         llvm::AllocaInst * alloca);

        private:
            /** this activation record created on behalf of a call to @ref lambda_.
             *  @ref Variable::path_ specifies a logical path to a variable,
             *  but does not distinguish stack-native variables from variables in explicit
             *  runtime environment records.
             *
             **/
            ref::rp<Lambda> lambda_;

            /** @c binding_v_[i] specifies how/where to get location for formal parameter number *i*
             *  of @ref lambda_.
             *
             **/
            std::vector<runtime_binding_path> binding_v_;

            /** maps named slots in a stack frame to logical addresses.
             *  Only applies to not-captured vars with i_rt_link_=0
             **/
            std::map<std::string, llvm::AllocaInst*> frame_; /* <-> kaleidoscope NamedValues */
        }; /*activation_record*/

    } /*namespace jit*/
} /*namespace xo*/


/** end activation_record.hpp **/
