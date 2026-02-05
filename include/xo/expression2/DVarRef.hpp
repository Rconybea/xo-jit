/** @file DVarRef.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "Variable.hpp"

namespace xo {
    namespace scm {

        /** @class DVarRef
         *  @brief syntax for a variable reference
         *
         *  Reference to a known variable possibly
         *  defined in another scope.
         *  For non-local non-global variables,
         **/
        class DVarRef {
        public:
            using ppindentinfo = xo::print::ppindentinfo;
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using TypeDescr = xo::reflect::TypeDescr;

        public:
            DVarRef(DVariable * vardef,
                    Binding path);

            /** create instance
             *  @p mm      memory allocator
             *  @p vardef  variable definition (name, typeref, binding)
             *  @p link    number of lexical scope boundaries we must cross
             *             to reach scope containing @p vardef.
             *             Only relevant for non-global vardef
             **/
            static DVarRef * make(obj<AAllocator> mm,
                                  DVariable * vardef,
                                  int32_t link);

            const DUniqueString * name() const;
            Binding path() const { return path_; }

            /** @defgroup scm-variable-expression-facet **/
            ///@{

            exprtype extype() const noexcept { return exprtype::varref; }
            TypeRef typeref() const noexcept;
            TypeDescr valuetype() const noexcept;
            void assign_valuetype(TypeDescr td) noexcept;

            ///@}
            /** @defgroup scm-variable-gcobject-facet **/
            ///@{

            size_t shallow_size() const noexcept;
            DVarRef * shallow_copy(obj<AAllocator> mm) const noexcept;
            size_t forward_children(obj<ACollector> gc) noexcept;

            ///@}
            /** @defgroup scm-variable-printable-facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** variable definition. Created in the sccope where variable introduced.
             *  Has an associated @ref Binding, but that binding is only correct
             *  around any nested scopes.
             **/
            DVariable * vardef_ = nullptr;

            /** at runtime: navigate environemnt via this path to get
             *  runtime memory location for this variable
             **/
            Binding path_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DVarRef.hpp */
