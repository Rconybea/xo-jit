/** @file IPrintable_DVsmSeqContFrame.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DVsmSeqContFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DVsmSeqContFrame.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DVsmSeqContFrame.hpp"

namespace xo { namespace scm { class IPrintable_DVsmSeqContFrame; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DVsmSeqContFrame>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DVsmSeqContFrame,
               xo::scm::IPrintable_DVsmSeqContFrame>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DVsmSeqContFrame
         **/
        class IPrintable_DVsmSeqContFrame {
        public:
            /** @defgroup scm-printable-dvsmseqcontframe-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-dvsmseqcontframe-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DVsmSeqContFrame & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */