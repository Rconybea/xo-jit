/** @file IExpression_DDefineExpr.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DDefineExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DDefineExpr.json5]
 **/

#pragma once

#include "Expression.hpp"
#include "Expression.hpp"
#include "DDefineExpr.hpp"

namespace xo { namespace scm { class IExpression_DDefineExpr; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::AExpression,
                                   xo::scm::DDefineExpr>
        {
            using ImplType = xo::scm::IExpression_Xfer
              <xo::scm::DDefineExpr,
               xo::scm::IExpression_DDefineExpr>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IExpression_DDefineExpr
         **/
        class IExpression_DDefineExpr {
        public:
            /** @defgroup scm-expression-ddefineexpr-type-traits **/
            ///@{
            using TypeDescr = xo::scm::AExpression::TypeDescr;
            using Copaque = xo::scm::AExpression::Copaque;
            using Opaque = xo::scm::AExpression::Opaque;
            ///@}
            /** @defgroup scm-expression-ddefineexpr-methods **/
            ///@{
            // const methods
            /** expression type (constant | apply | ..) **/
            static exprtype extype(const DDefineExpr & self) noexcept;
            /** placeholder for type giving possible values for this expression **/
            static TypeRef typeref(const DDefineExpr & self) noexcept;
            /** type giving possible values for this expression. Maybe null before typecheck **/
            static TypeDescr valuetype(const DDefineExpr & self) noexcept;

            // non-const methods
            /** assing to valuetype member. Useful when scaffolding expressions **/
            static void assign_valuetype(DDefineExpr & self, TypeDescr td) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */