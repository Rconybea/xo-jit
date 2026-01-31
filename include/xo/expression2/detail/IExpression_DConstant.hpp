/** @file IExpression_DConstant.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DConstant.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DConstant.json5]
 **/

#pragma once

#include "Expression.hpp"
#include "Expression.hpp"
#include "DConstant.hpp"

namespace xo { namespace scm { class IExpression_DConstant; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::AExpression,
                                   xo::scm::DConstant>
        {
            using ImplType = xo::scm::IExpression_Xfer
              <xo::scm::DConstant,
               xo::scm::IExpression_DConstant>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IExpression_DConstant
         **/
        class IExpression_DConstant {
        public:
            /** @defgroup scm-expression-dconstant-type-traits **/
            ///@{
            using TypeDescr = xo::scm::AExpression::TypeDescr;
            using Copaque = xo::scm::AExpression::Copaque;
            using Opaque = xo::scm::AExpression::Opaque;
            ///@}
            /** @defgroup scm-expression-dconstant-methods **/
            ///@{
            // const methods
            /** expression type (constant | apply | ..) **/
            static exprtype extype(const DConstant & self) noexcept;
            /** placeholder for type giving possible values for this expression **/
            static TypeRef typeref(const DConstant & self) noexcept;
            /** type giving possible values for this expression. Maybe null before typecheck **/
            static TypeDescr valuetype(const DConstant & self) noexcept;

            // non-const methods
            /** assing to valuetype member. Useful when scaffolding expressions **/
            static void assign_valuetype(DConstant & self, TypeDescr td) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */