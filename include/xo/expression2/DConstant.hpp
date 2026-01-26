/** @file DConstant.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Expression.hpp"
#include "TypeRef.hpp"
#include "exprtype.hpp"
#include <xo/reflect/TaggedPtr.hpp>
#include <xo/gc/GCObject.hpp>

namespace xo {
    namespace scm {
        /** @class DConstant
         *  @brief Schematika expression respresenting a literal constant
         **/
        struct DConstant {
        public:
            using TaggedPtr = xo::reflect::TaggedPtr;
            using TypeDescr = xo::reflect::TypeDescr;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using typeseq = xo::reflect::typeseq;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            explicit DConstant(obj<AGCObject> value) noexcept;

            /** create isntance
             *  @p mm     memory allocator
             *  @p value  literal constant
             **/
            static obj<AExpression,DConstant> make(obj<AAllocator> mm,
                                                   obj<AGCObject> value);

            /** create instance
             *  @p mm     memory allocator
             *  @p value  literal constant
             **/
            static DConstant * _make(obj<AAllocator> mm,
                                     obj<AGCObject> value);

            bool is_resolved() const noexcept { return typeref_.is_resolved(); }

            exprtype extype() const noexcept { return exprtype::constant; }
            TypeDescr value_td() const noexcept { return typeref_.td(); }
            TaggedPtr value_tp() const noexcept { return TaggedPtr(typeref_.td(), value_.data()); }

            TypeRef typeref() const noexcept { return typeref_; }
            TypeDescr valuetype() const noexcept { return typeref_.td(); }
            obj<AGCObject> value() const noexcept { return value_; }

            void assign_valuetype(TypeDescr td) noexcept { typeref_.resolve(td); }

            bool pretty(const ppindentinfo & ppii) const;

        private:
            static TypeDescr _lookup_td(typeseq tseq);

        private:
            /** type for value of this expression
             *  or unification breadcrumb before unification
             **/
            TypeRef typeref_;
            /** literal value **/
            obj<AGCObject> value_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DConstant.hpp */
