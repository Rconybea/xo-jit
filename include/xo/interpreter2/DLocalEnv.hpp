/** @file DLocalEnv.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/expression2/LocalSymtab.hpp>
#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {

        /** @brief runtime bindings for arguments to a lambda
         **/
        class DLocalEnv {
        public:
            using DArray = xo::scm::DArray;
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::uint32_t;

        public:
            /** @defgroup scm-localenv-constructors constructors **/
            ///@{

            /** create instance with parent @p p for variables in @p symtab **/
            DLocalEnv(DLocalEnv * parent,
                      DLocalSymtab * symtab,
                      DArray * args);

            static DLocalEnv * _make(obj<AAllocator> mm,
                                     DLocalEnv * parent,
                                     DLocalSymtab * symtab,
                                     DArray * args);

            ///@}
            /** @defgroup scm-local-env-methods methods **/
            ///@{

            DLocalEnv * parent() const noexcept { return parent_; }
            size_type size() const noexcept { return symtab_->size(); }

            /** lookup current value associated with binding @p ix **/
            obj<AGCObject> lookup_value(Binding ix) const noexcept;

            /** assign value associated with binding @p ix to @p x **/
            void assign_value(Binding ix, obj<AGCObject> x);

            ///@}
            /** @defgroup scm-localenv-gcobject-facet **/
            ///@{

            std::size_t shallow_size() const noexcept;
            DLocalEnv * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            ///@}
            /** @defgroup scm-localenv-printable-facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const noexcept;

            ///@}

        private:
            /** parent environment (from closure) **/
            DLocalEnv * parent_ = nullptr;
            /** bind values for variables in this symbol table **/
            DLocalSymtab * symtab_ = nullptr;
            /** bindings.
             *  (*args)[i] associates a value with symtab->slots_[i]
             **/
            DArray * args_ = nullptr;
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalEnv.hpp */
