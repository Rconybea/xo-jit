/** @file IPrintable_DToplevelSeqSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DToplevelSeqSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DToplevelSeqSsm.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DToplevelSeqSsm.hpp"

namespace xo { namespace scm { class IPrintable_DToplevelSeqSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DToplevelSeqSsm>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DToplevelSeqSsm,
               xo::scm::IPrintable_DToplevelSeqSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DToplevelSeqSsm
         **/
        class IPrintable_DToplevelSeqSsm {
        public:
            /** @defgroup scm-printable-dexprseqstate-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-dexprseqstate-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DToplevelSeqSsm & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */