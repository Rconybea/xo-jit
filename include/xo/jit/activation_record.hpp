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
            activation_record() = default;

            llvm::AllocaInst * lookup_var(const std::string & var_name) const;

            llvm::AllocaInst * alloc_var(const std::string & var_name,
                                         llvm::AllocaInst * alloca);

        private:
            /** maps named slots in a stack frame to logical addresses **/
            std::map<std::string, llvm::AllocaInst*> frame_; /* <-> kaleidoscope NamedValues */
        }; /*activation_record*/

    } /*namespace jit*/
} /*namespace xo*/


/** end activation_record.hpp **/
