/** @file DGlobalSymtab.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Binding.hpp"
#include "DVariable.hpp"
#include <xo/object2/DArray.hpp>
#include <xo/alloc2/dp.hpp>
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
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::uint32_t;

        public:
            /** @defgroup scm-globalsymtab-ctors constructors **/
            ///@{

            DGlobalSymtab(dp<repr_type> map, DArray * vars);

            /** create instance.
             *  Use memory from @p fixed_mm for @ref map_.
             *  Use memory from @p mm for DGlobalSymtab instance.
             *  Hashmap configured per @p cfg.
             **/
            static dp<DGlobalSymtab> make(obj<AAllocator> mm,
                                          obj<AAllocator> fixed_mm,
                                          const ArenaHashMapConfig & cfg);

            /** non-trivial destructor for @ref map_ **/
            ~DGlobalSymtab() = default;

            ///@}
            /** @defgroup scm-globalsymtab-access-methods access methods **/
            ///@{

            size_type size() const noexcept { return map_->size(); }
            size_type capacity() const noexcept { return map_->capacity(); }

            /** visit symtab-owned memory pools; call visitor(info) for each **/
            void visit_pools(const MemorySizeVisitor & visitor) const;

            /** lookup global symbol with name @p sym **/
            DVariable * lookup_variable(const DUniqueString * sym) const noexcept;

            ///@}
            /** @defgroup scm-globalsymtab-general-methods general methods **/
            ///@{

            /** update this symtab to associate @p var with @c var->name().
             *  If there was a previous variable with the same name,
             *  replace it with @p var.
             **/
            void upsert_variable(obj<AAllocator> mm,
                                 DVariable * var);

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
            /** @defgroup scm-globalsymtab-printable-facet printable facet **/
            ///@{

            /** pretty-printing support **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** map symbols -> bindings.
             *  Minor point: storing offsets instead of Variables allows us to omit
             *  iterating over map elements during GC. Possible savings if map_ slots
             *  sparsely populated.
             **/
            dp<repr_type> map_;

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
