/** @file IPrintable_DIfElseSsm.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DIfElseSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DIfElseSsm.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DIfElseSsm.hpp"

namespace xo { namespace scm { class IPrintable_DIfElseSsm; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DIfElseSsm>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DIfElseSsm,
               xo::scm::IPrintable_DIfElseSsm>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DIfElseSsm
         **/
        class IPrintable_DIfElseSsm {
        public:
            /** @defgroup scm-printable-difelsessm-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-difelsessm-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DIfElseSsm & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */