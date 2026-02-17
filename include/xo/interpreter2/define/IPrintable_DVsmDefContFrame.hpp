/** @file IPrintable_DVsmDefContFrame.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DVsmDefContFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DVsmDefContFrame.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DVsmDefContFrame.hpp"

namespace xo { namespace scm { class IPrintable_DVsmDefContFrame; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DVsmDefContFrame>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DVsmDefContFrame,
               xo::scm::IPrintable_DVsmDefContFrame>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DVsmDefContFrame
         **/
        class IPrintable_DVsmDefContFrame {
        public:
            /** @defgroup scm-printable-dvsmdefcontframe-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-dvsmdefcontframe-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DVsmDefContFrame & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */