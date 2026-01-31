/** @file IExpression_DVariable.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DVariable.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DVariable.json5]
 **/

#pragma once

#include "Expression.hpp"
#include "Expression.hpp"
#include "DVariable.hpp"

namespace xo { namespace scm { class IExpression_DVariable; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::AExpression,
                                   xo::scm::DVariable>
        {
            using ImplType = xo::scm::IExpression_Xfer
              <xo::scm::DVariable,
               xo::scm::IExpression_DVariable>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IExpression_DVariable
         **/
        class IExpression_DVariable {
        public:
            /** @defgroup scm-expression-dvariable-type-traits **/
            ///@{
            using TypeDescr = xo::scm::AExpression::TypeDescr;
            using Copaque = xo::scm::AExpression::Copaque;
            using Opaque = xo::scm::AExpression::Opaque;
            ///@}
            /** @defgroup scm-expression-dvariable-methods **/
            ///@{
            // const methods
            /** expression type (constant | apply | ..) **/
            static exprtype extype(const DVariable & self) noexcept;
            /** placeholder for type giving possible values for this expression **/
            static TypeRef typeref(const DVariable & self) noexcept;
            /** type giving possible values for this expression. Maybe null before typecheck **/
            static TypeDescr valuetype(const DVariable & self) noexcept;

            // non-const methods
            /** assing to valuetype member. Useful when scaffolding expressions **/
            static void assign_valuetype(DVariable & self, TypeDescr td) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */