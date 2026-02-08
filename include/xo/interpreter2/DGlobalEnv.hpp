/** @file DGlobalEnv.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/expression2/DGlobalSymtab.hpp>
#include <xo/object2/DList.hpp>
#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {


        /** @brief runtime bindings for global variabels
         **/
        class DGlobalEnv {
        public:
            DGlobalEnv() = default;

        protected: // temporary, to appease compiler

            // absurd O(n) implementation for now
            // replace with gc-aware hashtable, when available.

            /** globals.  Slots in @ref global_v_ are numbered in DLocalSymtab **/
            DArray * global_v_ = nullptr;
        };
    }
}
