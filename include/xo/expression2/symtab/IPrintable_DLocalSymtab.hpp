/** @file IPrintable_DLocalSymtab.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DLocalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DLocalSymtab.json5]
 **/

#pragma once

#include "Printable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/printable2/detail/IPrintable_Xfer.hpp>
#include "DLocalSymtab.hpp"

namespace xo { namespace scm { class IPrintable_DLocalSymtab; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::print::APrintable,
                                   xo::scm::DLocalSymtab>
        {
            using ImplType = xo::print::IPrintable_Xfer
              <xo::scm::DLocalSymtab,
               xo::scm::IPrintable_DLocalSymtab>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IPrintable_DLocalSymtab
         **/
        class IPrintable_DLocalSymtab {
        public:
            /** @defgroup scm-printable-dlocalsymtab-type-traits **/
            ///@{
            using ppindentinfo = xo::print::APrintable::ppindentinfo;
            using Copaque = xo::print::APrintable::Copaque;
            using Opaque = xo::print::APrintable::Opaque;
            ///@}
            /** @defgroup scm-printable-dlocalsymtab-methods **/
            ///@{
            // const methods
            /** Pretty-printing support for this object.
See [xo-indentlog/xo/indentlog/pretty.hpp] **/
            static bool pretty(const DLocalSymtab & self, const ppindentinfo & ppii);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */