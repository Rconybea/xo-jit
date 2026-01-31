/** @file IExpression_DSequenceExpr.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DSequenceExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DSequenceExpr.json5]
 **/

#pragma once

#include "Expression.hpp"
#include "Expression.hpp"
#include "DSequenceExpr.hpp"

namespace xo { namespace scm { class IExpression_DSequenceExpr; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::AExpression,
                                   xo::scm::DSequenceExpr>
        {
            using ImplType = xo::scm::IExpression_Xfer
              <xo::scm::DSequenceExpr,
               xo::scm::IExpression_DSequenceExpr>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IExpression_DSequenceExpr
         **/
        class IExpression_DSequenceExpr {
        public:
            /** @defgroup scm-expression-dsequenceexpr-type-traits **/
            ///@{
            using TypeDescr = xo::scm::AExpression::TypeDescr;
            using Copaque = xo::scm::AExpression::Copaque;
            using Opaque = xo::scm::AExpression::Opaque;
            ///@}
            /** @defgroup scm-expression-dsequenceexpr-methods **/
            ///@{
            // const methods
            /** expression type (constant | apply | ..) **/
            static exprtype extype(const DSequenceExpr & self) noexcept;
            /** placeholder for type giving possible values for this expression **/
            static TypeRef typeref(const DSequenceExpr & self) noexcept;
            /** type giving possible values for this expression. Maybe null before typecheck **/
            static TypeDescr valuetype(const DSequenceExpr & self) noexcept;

            // non-const methods
            /** assing to valuetype member. Useful when scaffolding expressions **/
            static void assign_valuetype(DSequenceExpr & self, TypeDescr td) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */