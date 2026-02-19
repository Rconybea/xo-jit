/** @file INumeric_DInteger.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/INumeric_DInteger.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/INumeric_DInteger.json5]
 **/

#pragma once

#include "Numeric.hpp"
#include "DInteger.hpp"

namespace xo { namespace scm { class INumeric_DInteger; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ANumeric,
                                   xo::scm::DInteger>
        {
            using ImplType = xo::scm::INumeric_Xfer
              <xo::scm::DInteger,
               xo::scm::INumeric_DInteger>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class INumeric_DInteger
         **/
        class INumeric_DInteger {
        public:
            /** @defgroup scm-numeric-dinteger-type-traits **/
            ///@{
            using Copaque = xo::scm::ANumeric::Copaque;
            using Opaque = xo::scm::ANumeric::Opaque;
            ///@}
            /** @defgroup scm-numeric-dinteger-methods **/
            ///@{
            // const methods

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */