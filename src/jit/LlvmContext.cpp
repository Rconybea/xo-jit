/* @file LlvmContext.cpp */

#include "LlvmContext.hpp"

namespace xo {
    namespace jit {
        rp<LlvmContext>
        LlvmContext::make() {
            return new LlvmContext();
        }

        LlvmContext::LlvmContext()
            : llvm_cx_{std::make_unique<llvm::LLVMContext>()}
        {}

    } /*namespace jit*/
} /*namespace xo*/


/* end LlvmContext.cpp */
