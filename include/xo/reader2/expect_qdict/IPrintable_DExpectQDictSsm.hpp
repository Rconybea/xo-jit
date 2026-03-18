/** @file IPrintable_DExpectQDictSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DExpectQDictSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DExpectQDictSsm.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DExpectQDictSsm.hpp"

namespace xo { namespace scm { class IPrintable_DExpectQDictSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DExpectQDictSsm>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DExpectQDictSsm,
               xo::scm::IPrintable_DExpectQDictSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DExpectQDictSsm
         **/
        class IPrintable_DExpectQDictSsm {
        public:
            /** @defgroup scm-printable-dexpectqdictssm-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-dexpectqdictssm-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DExpectQDictSsm & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */