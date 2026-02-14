/** @file DGlobalSymtab.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Binding.hpp"
#include <xo/arena/DArenaHashMap.hpp>

namespace xo {
    namespace scm {
        class DUniqueString;

        /** @class DGlobalSymtab
         *  @brief symbol table for toplevel environment
         *
         *  We're using DArenaHashMap to store <key,binding> pairs.
         *  Both of these are outside GC-space, so we don't need collector
         *  to traverse these.
         **/
        class DGlobalSymtab {
        public:
            using key_type = const DUniqueString *;
            using value_type = Binding;
            using repr_type = xo::map::DArenaHashMap<key_type, value_type>;
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;

        public:
            /** visit symtab-owned memory pools; call visitor(info) for each **/
            void visit_pools(const MemorySizeVisitor & visitor) const;

        public:
            /** @defgroup xo-expression2-symboltable-facet symboltable facet**/
            ///@{

            /** true for global symbol table **/
            bool is_global_symtab() const noexcept { return true; }

            /** lookup binding for variable @p sym **/
            Binding lookup_binding(const DUniqueString * sym) const noexcept;

            ///@}

        private:
            /** next binding will use this global index. See DGlobalEnv **/
            uint32_t next_binding_ix_ = 0;

            /** map symbols -> bindings **/
            repr_type map_;
            
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalSymtab.hpp */
