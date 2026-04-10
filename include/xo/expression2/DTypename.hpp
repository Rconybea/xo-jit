/** @file DTypename.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "DUniqueString.hpp"
#include <xo/type/Type.hpp>
#include <xo/alloc2/GCObjectVisitor.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    namespace scm {

        /** @class DTypename
         *  @brief Container for a named type
         *
         *  Represents the result of syntax like
         *  1. deftype Foo :: i64;
         *  2. deftype FooAlias :: Foo;
         **/
        class DTypename {
        public:
            using ppindentinfo = xo::print::ppindentinfo;
            using AGCObjectVisitor = xo::mm::AGCObjectVisitor;
            using VisitReason = xo::mm::VisitReason;
            using AGCObject = xo::mm::AGCObject;
            using AAllocator = xo::mm::AAllocator;

        public:
            DTypename(const DUniqueString * name,
                      obj<AType> type);

            /** create instance
             *  @p mm      memory allocator
             *  @p name    type name
             *  @p type    type definition
             **/
            static DTypename * _make(obj<AAllocator> mm,
                                     const DUniqueString * name,
                                     obj<AType> type);
            /** create fop for new instance **/
            static obj<AGCObject,DTypename> make(obj<AAllocator> mm,
                                                 const DUniqueString * name,
                                                 obj<AType> type);

            const DUniqueString * name() const noexcept { return name_; }
            obj<AType> type() const noexcept { return type_; }

            void assign_name(const DUniqueString * name) { this->name_ = name; }


            /** @defgroup scm-typename-gcobject-facet **/
            ///@{

            DTypename * gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept;
            void visit_gco_children(VisitReason reason, obj<AGCObjectVisitor> gc) noexcept;

            ///@}
            /** @defgroup scm-typename-printable-facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** symbol name **/
            const DUniqueString * name_ = nullptr;
            /** type defintion.  Everything but the name **/
            obj<AType> type_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DTypename.hpp */
