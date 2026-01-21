/** @file IPrintable_DExpectTypeSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DExpectTypeSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DExpectTypeSsm.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DExpectTypeSsm.hpp"

namespace xo { namespace scm { class IPrintable_DExpectTypeSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DExpectTypeSsm>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DExpectTypeSsm,
               xo::scm::IPrintable_DExpectTypeSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DExpectTypeSsm
         **/
        class IPrintable_DExpectTypeSsm {
        public:
            /** @defgroup scm-printable-dexpecttypessm-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-dexpecttypessm-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DExpectTypeSsm & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */