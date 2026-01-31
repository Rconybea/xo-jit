/** @file IPrintable_DIfElseExpr.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DIfElseExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DIfElseExpr.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DIfElseExpr.hpp"

namespace xo { namespace scm { class IPrintable_DIfElseExpr; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DIfElseExpr>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DIfElseExpr,
               xo::scm::IPrintable_DIfElseExpr>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DIfElseExpr
         **/
        class IPrintable_DIfElseExpr {
        public:
            /** @defgroup scm-printable-difelseexpr-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-difelseexpr-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DIfElseExpr & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */