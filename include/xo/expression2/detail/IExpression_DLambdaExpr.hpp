/** @file IExpression_DLambdaExpr.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DLambdaExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DLambdaExpr.json5]
 **/

#pragma once

#include "Expression.hpp"
#include "Expression.hpp"
#include "DLambdaExpr.hpp"

namespace xo { namespace scm { class IExpression_DLambdaExpr; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::AExpression,
                                   xo::scm::DLambdaExpr>
        {
            using ImplType = xo::scm::IExpression_Xfer
              <xo::scm::DLambdaExpr,
               xo::scm::IExpression_DLambdaExpr>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IExpression_DLambdaExpr
         **/
        class IExpression_DLambdaExpr {
        public:
            /** @defgroup scm-expression-dlambdaexpr-type-traits **/
            ///@{
            using TypeDescr = xo::scm::AExpression::TypeDescr;
            using Copaque = xo::scm::AExpression::Copaque;
            using Opaque = xo::scm::AExpression::Opaque;
            ///@}
            /** @defgroup scm-expression-dlambdaexpr-methods **/
            ///@{
            // const methods
            /** expression type (constant | apply | ..) **/
            static exprtype extype(const DLambdaExpr & self) noexcept;
            /** placeholder for type giving possible values for this expression **/
            static TypeRef typeref(const DLambdaExpr & self) noexcept;
            /** type giving possible values for this expression. Maybe null before typecheck **/
            static TypeDescr valuetype(const DLambdaExpr & self) noexcept;

            // non-const methods
            /** assing to valuetype member. Useful when scaffolding expressions **/
            static void assign_valuetype(DLambdaExpr & self, TypeDescr td) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */