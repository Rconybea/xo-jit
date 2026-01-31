/** @file IPrintable_DExpectSymbolSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DExpectSymbolSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DExpectSymbolSsm.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DExpectSymbolSsm.hpp"

namespace xo { namespace scm { class IPrintable_DExpectSymbolSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DExpectSymbolSsm>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DExpectSymbolSsm,
               xo::scm::IPrintable_DExpectSymbolSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DExpectSymbolSsm
         **/
        class IPrintable_DExpectSymbolSsm {
        public:
            /** @defgroup scm-printable-dexpectsymbolssm-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-dexpectsymbolssm-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DExpectSymbolSsm & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */