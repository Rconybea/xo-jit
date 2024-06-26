/** @file activation_record.hpp
 *
 *  Author: Roland Conybeare
 **/

#pragma once

#include "LlvmContext.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
# include <llvm/IR/IRBuilder.h>
# include <llvm/IR/Instructions.h>
#pragma GCC diagnostic pop
#include <map>
//#include <cstdint>

namespace xo {
    namespace jit {
        /** scope for a stack frame associated with a user-defined function
         *
         *  each function needs its own IR builder,  to keep track of things like insert point
         **/
        class activation_record {
        public:
            activation_record(llvm::Function * llvm_fn,
                              llvm::AllocaInst * frame) : frame_{frame} {
                int i_arg = 0;
                for (auto & arg : llvm_fn->args()) {
                    std::string arg_name = std::string(arg.getName());

                    name2ix_map_[arg_name] = 2 + i_arg;
                }
            }

            std::int32_t lookup_var(const std::string & var_name) const;

#ifdef OBSOLETE
            llvm::AllocaInst * lookup_var(const std::string & var_name) const;

            llvm::AllocaInst * alloc_var(const std::string & var_name,
                                         llvm::AllocaInst * alloca);
#endif

        private:
            /** stack frame for a user-defined function (lambda) **/
            llvm::AllocaInst * frame_ = nullptr;

            /** for each formal parameter,
             *  reports its position in stack frame.
             *  This is the position to use with getelementptr,
             *  i.e. +2 to skip first two slots, that are reserved
             *  for nextframe pointer (slot 0) + unwind pointer (slot 1)
             **/
            std::map<std::string, std::int32_t> name2ix_map_;

#ifdef OBSOLETE
            /** maps named slots in a stack frame to logical addresses **/
            std::map<std::string, llvm::AllocaInst*> frame_; /* <-> kaleidoscope NamedValues */
#endif
        }; /*activation_record*/

    } /*namespace jit*/
} /*namespace xo*/


/** end activation_record.hpp **/
