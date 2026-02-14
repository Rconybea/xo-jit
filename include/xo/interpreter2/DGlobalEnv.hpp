/** @file DGlobalEnv.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/expression2/DGlobalSymtab.hpp>
#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {

        /** @brief runtime bindings for global variabels
         *
         *  Implementation here uses a DArenaHashMap to hold <key,value> pairs.
         *  The hash map has its own memory outside GC space.
         *  Keys are DUniqueStrings, also outside GC space.
         *  Values are regular gc-aware objects, generally will be in GC space.
         *
         *  We need collector to traverse all the values in a global env
         *  on each cycle.  Arrange that by having DGlobalEnv itself
         *  in GC space.
         *
         **/
        class DGlobalEnv {
        public:
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;

        public:
            DGlobalEnv() = default;

            /** visit env-owned memory pools; call visitor(info) for each **/
            void visit_pools(const MemorySizeVisitor & visitor) const;

        protected: // temporary, to appease compiler

            // absurd O(n) implementation for now
            // replace with gc-aware hashtable, when available.

            /** symbol table assigns a unique index for each symbol **/
            DGlobalSymtab * symtab_;

            /** value for a symbol S will be in values_[symtab->lookup_binding(S)] **/
            DArray * values_ = nullptr;
        };
    }
}
