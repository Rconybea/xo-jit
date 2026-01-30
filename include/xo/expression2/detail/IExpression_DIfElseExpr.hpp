/** @file IExpression_DIfElseExpr.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DIfElseExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DIfElseExpr.json5]
 **/

#pragma once

#include "Expression.hpp"
#include "Expression.hpp"
#include "DIfElseExpr.hpp"

namespace xo { namespace scm { class IExpression_DIfElseExpr; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::AExpression,
                                   xo::scm::DIfElseExpr>
        {
            using ImplType = xo::scm::IExpression_Xfer
              <xo::scm::DIfElseExpr,
               xo::scm::IExpression_DIfElseExpr>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IExpression_DIfElseExpr
         **/
        class IExpression_DIfElseExpr {
        public:
            /** @defgroup scm-expression-difelseexpr-type-traits **/
            ///@{
            using TypeDescr = xo::scm::AExpression::TypeDescr;
            using Copaque = xo::scm::AExpression::Copaque;
            using Opaque = xo::scm::AExpression::Opaque;
            ///@}
            /** @defgroup scm-expression-difelseexpr-methods **/
            ///@{
            // const methods
            /** expression type (constant | apply | ..) **/
            static exprtype extype(const DIfElseExpr & self) noexcept;
            /** placeholder for type giving possible values for this expression **/
            static TypeRef typeref(const DIfElseExpr & self) noexcept;
            /** type giving possible values for this expression. Maybe null before typecheck **/
            static TypeDescr valuetype(const DIfElseExpr & self) noexcept;

            // non-const methods
            /** assing to valuetype member. Useful when scaffolding expressions **/
            static void assign_valuetype(DIfElseExpr & self, TypeDescr td) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */