/** @file DGlobalSymtab.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Binding.hpp"
#include "DVariable.hpp"
#include "DTypename.hpp"
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

            DGlobalSymtab(dp<repr_type> var_map, DArray * vars,
                          dp<repr_type> type_map, DArray * types);

            /** create instance.
             *  Use memory from @p fixed_mm for @ref map_.
             *  Use memory from @p mm for DGlobalSymtab instance.
             *  Hashmap for variables per @p var_cfg; for types per @p type_cfg.
             **/
            static dp<DGlobalSymtab> make(obj<AAllocator> mm,
                                          obj<AAllocator> fixed_mm,
                                          const ArenaHashMapConfig & var_cfg,
                                          const ArenaHashMapConfig & type_cfg);

            /** non-trivial destructor for @ref map_ **/
            ~DGlobalSymtab() = default;

            ///@}
            /** @defgroup scm-globalsymtab-access-methods access methods **/
            ///@{

            size_type n_vars() const noexcept { return var_map_->size(); }
            size_type var_capacity() const noexcept { return var_map_->capacity(); }

            /** visit symtab-owned memory pools; call visitor(info) for each **/
            void visit_pools(const MemorySizeVisitor & visitor) const;

            /** lookup global symbol with name @p sym **/
            DVariable * lookup_variable(const DUniqueString * sym) const noexcept;

            /** lookup global typename with name @p sym **/
            DTypename * lookup_typename(const DUniqueString * sym) const noexcept;

            ///@}
            /** @defgroup scm-globalsymtab-general-methods general methods **/
            ///@{

            /** update this symtab to associate @p var with @c var->name().
             *  If there was a previous variable with the same name,
             *  replace it with @p var.
             **/
            void upsert_variable(obj<AAllocator> mm,
                                 DVariable * var);

            /** update this symtab to associate typename @p type with @c type->name().
             *  If there was a previous type with the same name, replace it with
             *  @p type.
             **/
            void upsert_typename(obj<AAllocator> mm,
                                 DTypename * type);

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
            /** map variable symbol -> index into @ref vars_.
             *  Minor point: storing offsets instead of Variables allows us to:
             *  omit hash-map iteration during GC.
             *  Savings when map_ slots sparsely populated.
             **/
            dp<repr_type> var_map_;

            /** array of variables.
             *  When S is a unique-string for a global symbol, then:
             *  1. var_map_[S] is unique global index i(S) for S.
             *  2. vars_[i(S)] is variable-expr var(S) for S
             *  3. var(S)->name == S
             **/
            DArray * vars_ = nullptr;

            /** map type name -> index values into @ref types_ **/
            dp<repr_type> type_map_;

            /** array of types.
             *  When T is a unique-string for a globally-defined type, then:
             *  1. type_map_[T] is unique global index i(T) for T.
             *  2. types_[i(T)] is type type(T) for T
             *  3. type(T)->name == T
             **/
            DArray * types_ = nullptr;
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalSymtab.hpp */
