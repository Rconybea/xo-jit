/** @file DLocalSymtab.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Binding.hpp"
#include "DVariable.hpp"
#include "DUniqueString.hpp"
#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {
        /** @class DLocalSymtab
         *  @brief symbol table for a local stack frame
         **/
        struct DLocalSymtab {
        public:
            using DArray = xo::scm::DArray;
            using ppindentinfo = xo::print::ppindentinfo;
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            /* note: uint16_t would be fine too */
            using size_type = std::uint32_t;

            struct Slot {
                Slot() = default;
                explicit Slot(DVariable * var) : var_{var} {}

                /** variable representing a formal argument.
                 *  binding will be correct only within the same layer
                 *  as top-level lambda body
                 *  (i.e. up to the doorstep of each and every nested lambda)
                 **/
                DVariable * var_ = nullptr;
            };

        public:
            /** @defgroup scm-lambdaexpr-constructors **/
            ///@{

            /** empty instance with parent @p p, using arrays @p vars for variables
             *  and @p types for type definitions.
             **/
            DLocalSymtab(DLocalSymtab * p, DArray * nv, DArray * nt);

            /** scaffold empty symtab instance,
             *  capacity for @p nv vars and @p nt types,
             *  using memory from allocator @p mm.
             *  Symtab chains to parent @p p.
             **/
            static DLocalSymtab * _make_empty(obj<AAllocator> mm,
                                              DLocalSymtab * p,
                                              size_type nv,
                                              size_type nt);

            ///@}
            /** @defgroup scm-lambdaexpr-methods **/
            ///@{

            DLocalSymtab * parent() const noexcept { return parent_; }
            //size_type capacity() const noexcept { return capacity_; }
            size_type n_vars() const noexcept { return vars_->size(); }
            size_type n_types() const noexcept { return types_->size(); }

            DVariable * lookup_var(Binding ix) noexcept;

            /** increase slot size (provided below capacity) to append
             *  binding for one local variable.  Local variable will be allocated
             *  from @p mm, named @p name, with type described by @p typeref.
             **/
            Binding append_var(obj<AAllocator> mm,
                               const DUniqueString * name,
                               TypeRef typeref);

            /** increase slot size (provided below capacity) to append
             *  binding for one local type.  Local type will be allocated
             *  from @p mm, named @p name, with type described by @p type.
             **/
            void append_type(obj<AAllocator> mm,
                             const DUniqueString * name,
                             obj<AType> type);

            ///@}
            /** @defgroup xo-localsymtab-symboltable-facet symboltable facet**/
            ///@{

            /** true for global symbol table **/
            bool is_global_symtab() const noexcept { return false; }

            /** lookup binding for variable @p sym **/
            Binding lookup_binding(const DUniqueString * sym) const noexcept;

            ///@}
            /** @defgroup xo-localsymtab-gcobject-facet gcobject facet **/
            ///@{

            std::size_t shallow_size() const noexcept;
            DLocalSymtab * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            ///@}
            /** @defgroup xo-localsymtab-printable-facet printable facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** parent symbol table from scoping surrounding this one **/
            DLocalSymtab * parent_ = nullptr;
            /** variables owned by (declared in) this symbol table
             *   vars_[i] is convertible to obj<AGCObject>
             **/
            DArray * vars_ = nullptr;
            /** types owned by (defined in) this symbol table
             *   types_[i] is convertible to obj<AType>
             **/
            DArray * types_ = nullptr;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalSymtab.hpp */
