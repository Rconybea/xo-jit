/** @file DGlobalSymtab.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Binding.hpp"
#include "DVariable.hpp"
#include <xo/object2/DArray.hpp>
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
            using ArenaHashMapConfig = xo::map::ArenaHashMapConfig;
            using repr_type = xo::map::DArenaHashMap<key_type, Binding::slot_type>;
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;

        public:
            /** @defgroup scm-globalsymtab-ctors constructors **/
            ///@{

            DGlobalSymtab(repr_type * map, DArray * vars);

            /** create instance.
             *  Use memory from @p fixed_mm for @ref map_.
             *  Use memory from @p mm for DGlobalSymtab instance.
             *  Hashmap configured per @p cfg.
             **/
            DGlobalSymtab * make(obj<AAllocator> fixed_mm,
                                 obj<AAllocator> mm,
                                 const ArenaHashMapConfig & cfg);

            ///@}
            /** @defgroup scm-globalsymtab-access-methods access methods **/
            ///@{

            /** visit symtab-owned memory pools; call visitor(info) for each **/
            void visit_pools(const MemorySizeVisitor & visitor) const;

            /** lookup global symbol with name @p sym **/
            DVariable * lookup_variable(const DUniqueString * sym) const noexcept;

            ///@}
            /** @defgroup scm-globalsymtab-general-methods general methods **/
            ///@{

            /** establish binding for @p sym, with type described by @p typeref,
             *  replacing existing global (if present) with the same name.
             *  Use memory from @p mm to create variable-expr
             **/
            DVariable * establish_variable(obj<AAllocator> mm,
                                           const DUniqueString * sym,
                                           TypeRef typeref);

            ///@}
            /** @defgroup scm-globalsymtab-symboltable-facet symboltable facet **/
            ///@{

            /** true for global symbol table **/
            bool is_global_symtab() const noexcept { return true; }

            /** lookup binding for variable @p sym **/
            Binding lookup_binding(const DUniqueString * sym) const noexcept;

            ///@}
            /** @defgroup scm-globalsymtab-gcobject-facet gcobject facet **/
            ///@{
            
            std::size_t shallow_size() const noexcept;
            DGlobalSymtab * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            ///@}
            
        private:
            /** map symbols -> bindings.
             *  Minor point: storing offsets instead of Variables allows us to omit
             *  iterating over map elements during GC. Possible savings if map_ slots
             *  sparsely populated.
             **/
            repr_type * map_ = nullptr;
            
            /** array of variables.
             *  When S is a unique-string for a global symbol, then:
             *  1. map_[S] is unique global index i(S) for S.
             *  2. vars_[i(S)] is variable-expr var(S) for S
             *  3. var(S)->name == S
             **/
            DArray * vars_ = nullptr;
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalSymtab.hpp */
