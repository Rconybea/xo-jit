/** @file IRuntimeContext_DVsmRcx.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IRuntimeContext_DVsmRcx.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IRuntimeContext_DVsmRcx.json5]
 **/

#pragma once

#include "RuntimeContext.hpp"
#include "DVsmRcx.hpp"

namespace xo { namespace scm { class IRuntimeContext_DVsmRcx; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ARuntimeContext,
                                   xo::scm::DVsmRcx>
        {
            using ImplType = xo::scm::IRuntimeContext_Xfer
              <xo::scm::DVsmRcx,
               xo::scm::IRuntimeContext_DVsmRcx>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class IRuntimeContext_DVsmRcx
         **/
        class IRuntimeContext_DVsmRcx {
        public:
            /** @defgroup scm-runtimecontext-dvsmrcx-type-traits **/
            ///@{
            using AAllocator = xo::scm::ARuntimeContext::AAllocator;
            using Copaque = xo::scm::ARuntimeContext::Copaque;
            using Opaque = xo::scm::ARuntimeContext::Opaque;
            ///@}
            /** @defgroup scm-runtimecontext-dvsmrcx-methods **/
            ///@{
            // const methods
            /** default allocator to use for objects **/
            static obj<AAllocator> allocator(const DVsmRcx & self) noexcept;

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */