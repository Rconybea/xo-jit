/** @file IPrintable_DVsmApplyFrame.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DVsmApplyFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DVsmApplyFrame.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DVsmApplyFrame.hpp"

namespace xo { namespace scm { class IPrintable_DVsmApplyFrame; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DVsmApplyFrame>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DVsmApplyFrame,
               xo::scm::IPrintable_DVsmApplyFrame>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DVsmApplyFrame
         **/
        class IPrintable_DVsmApplyFrame {
        public:
            /** @defgroup scm-printable-dvsmapplyframe-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-dvsmapplyframe-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DVsmApplyFrame & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */