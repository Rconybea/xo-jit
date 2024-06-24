/* @file activation_record.cpp */

#include "activation_record.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <iostream>

namespace xo {
    namespace jit {
        using std::cerr;
        using std::endl;

        llvm::AllocaInst *
        activation_record::lookup_var(const std::string & x) const {

            auto ix = frame_.find(x);

            if (ix == frame_.end()) {
                cerr << "activation_record::lookup_var: no binding for variable x"
                     << xtag("x", x)
                     << endl;
                return nullptr;
            }

            return ix->second;
        } /*lookup_var*/

        llvm::AllocaInst *
        activation_record::alloc_var(const std::string & x,
                                     llvm::AllocaInst * alloca)
        {
            if (frame_.find(x) != frame_.end()) {
                cerr << "activation_record::alloc_var: variable x already present in frame"
                     << xtag("x", x)
                     << endl;
                return nullptr;
            }

            frame_[x] = alloca;
            return alloca;
        } /*alloc_var*/
    } /*namespace jit*/
} /*namespace xo*/


/* end activation_record.cpp */
