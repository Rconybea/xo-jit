/** @file IExpression_DApplyExpr.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DApplyExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DApplyExpr.json5]
 **/

#pragma once

#include "Expression.hpp"
#include "Expression.hpp"
#include "DApplyExpr.hpp"

namespace xo { namespace scm { class IExpression_DApplyExpr; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::AExpression,
                                   xo::scm::DApplyExpr>
        {
            using ImplType = xo::scm::IExpression_Xfer
              <xo::scm::DApplyExpr,
               xo::scm::IExpression_DApplyExpr>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IExpression_DApplyExpr
         **/
        class IExpression_DApplyExpr {
        public:
            /** @defgroup scm-expression-dapplyexpr-type-traits **/
            ///@{
            using TypeDescr = xo::scm::AExpression::TypeDescr;
            using Copaque = xo::scm::AExpression::Copaque;
            using Opaque = xo::scm::AExpression::Opaque;
            ///@}
            /** @defgroup scm-expression-dapplyexpr-methods **/
            ///@{
            // const methods
            /** expression type (constant | apply | ..) **/
            static exprtype extype(const DApplyExpr & self) noexcept;
            /** placeholder for type giving possible values for this expression **/
            static TypeRef typeref(const DApplyExpr & self) noexcept;
            /** type giving possible values for this expression. Maybe null before typecheck **/
            static TypeDescr valuetype(const DApplyExpr & self) noexcept;

            // non-const methods
            /** assing to valuetype member. Useful when scaffolding expressions **/
            static void assign_valuetype(DApplyExpr & self, TypeDescr td) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */