/** @file IPrintable_DSequenceExpr.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DSequenceExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DSequenceExpr.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DSequenceExpr.hpp"

namespace xo { namespace scm { class IPrintable_DSequenceExpr; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DSequenceExpr>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DSequenceExpr,
               xo::scm::IPrintable_DSequenceExpr>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DSequenceExpr
         **/
        class IPrintable_DSequenceExpr {
        public:
            /** @defgroup scm-printable-dsequenceexpr-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-dsequenceexpr-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DSequenceExpr & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */