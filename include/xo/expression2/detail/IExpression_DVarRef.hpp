/** @file IExpression_DVarRef.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IExpression_DVarRef.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IExpression_DVarRef.json5]
 **/

#pragma once

#include "Expression.hpp"
#include "Expression.hpp"
#include "DVarRef.hpp"

namespace xo { namespace scm { class IExpression_DVarRef; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::AExpression,
                                   xo::scm::DVarRef>
        {
            using ImplType = xo::scm::IExpression_Xfer
              <xo::scm::DVarRef,
               xo::scm::IExpression_DVarRef>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IExpression_DVarRef
         **/
        class IExpression_DVarRef {
        public:
            /** @defgroup scm-expression-dvarref-type-traits **/
            ///@{
            using TypeDescr = xo::scm::AExpression::TypeDescr;
            using Copaque = xo::scm::AExpression::Copaque;
            using Opaque = xo::scm::AExpression::Opaque;
            ///@}
            /** @defgroup scm-expression-dvarref-methods **/
            ///@{
            // const methods
            /** expression type (constant | apply | ..) **/
            static exprtype extype(const DVarRef & self) noexcept;
            /** placeholder for type giving possible values for this expression **/
            static TypeRef typeref(const DVarRef & self) noexcept;
            /** type giving possible values for this expression. Maybe null before typecheck **/
            static TypeDescr valuetype(const DVarRef & self) noexcept;

            // non-const methods
            /** assing to valuetype member. Useful when scaffolding expressions **/
            static void assign_valuetype(DVarRef & self, TypeDescr td) noexcept;
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */