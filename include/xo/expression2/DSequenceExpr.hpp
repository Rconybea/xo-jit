/** @file DSequenceExpr.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Expression.hpp"
#include "TypeRef.hpp"
#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {

        /** syntax for a sequence of expressions
         *   {
         *      expr(1);
         *      expr(2);
         *      ...
         *      expr(n);
         *   }
         **/
        class DSequenceExpr {
        public:
            using AAllocator = xo::mm::AAllocator;
            using TypeDescr = xo::reflect::TypeDescr;
            using size_type = DArray::size_type;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            DSequenceExpr() = default;
            DSequenceExpr(DArray * xv) : expr_v_{xv} {}

            /** create empty sequence using memory from @p mm **/
            static obj<AExpression,DSequenceExpr> make_empty(obj<AAllocator> mm);

            /** create empty sequence expression using mmeory from @p mm **/
            static DSequenceExpr * _make_empty(obj<AAllocator> mm);

            size_type size() const noexcept;
            obj<AExpression> operator[](std::size_t i) const;

            /** append @p expr to the end of this sequence;
             *  use memory from @p mm if need to expand storage
             **/
            void push_back(obj<AAllocator> mm,
                           obj<AExpression> expr);

            // get_free_variables();
            // visit_preorder();
            // visit_layer();
            // xform_layer()
            // attach_envs()

            /** @defgroup scm-ifelseexpr-expression-facets **/
            ///@{

            exprtype extype() const noexcept { return exprtype::sequence; }
            TypeRef typeref() const noexcept { return typeref_; }
            TypeDescr valuetype() const noexcept { return typeref_.td(); }
            void assign_valuetype(TypeDescr td) noexcept;

            ///@}
            /** @defgroup scm-sequenceexpr-printable-facet printable facet methods **/
            ///@{

            /** pretty-printing driver; combine layout+printing **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** expression value always has type consistent with this description
             **/
            TypeRef typeref_;
            /** array of expressions **/
            DArray * expr_v_ = nullptr;
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DSequenceExpr.hpp */
