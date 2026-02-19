/** @file INumeric_DFloat.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/INumeric_DFloat.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/INumeric_DFloat.json5]
 **/

#pragma once

#include "Numeric.hpp"
#include "DFloat.hpp"

namespace xo { namespace scm { class INumeric_DFloat; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ANumeric,
                                   xo::scm::DFloat>
        {
            using ImplType = xo::scm::INumeric_Xfer
              <xo::scm::DFloat,
               xo::scm::INumeric_DFloat>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class INumeric_DFloat
         **/
        class INumeric_DFloat {
        public:
            /** @defgroup scm-numeric-dfloat-type-traits **/
            ///@{
            using Copaque = xo::scm::ANumeric::Copaque;
            using Opaque = xo::scm::ANumeric::Opaque;
            ///@}
            /** @defgroup scm-numeric-dfloat-methods **/
            ///@{
            // const methods

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */