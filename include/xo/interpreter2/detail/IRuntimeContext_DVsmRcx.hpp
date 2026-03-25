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
            using ACollector = xo::scm::ARuntimeContext::ACollector;
            using MemorySizeVisitor = xo::scm::ARuntimeContext::MemorySizeVisitor;
            using Copaque = xo::scm::ARuntimeContext::Copaque;
            using Opaque = xo::scm::ARuntimeContext::Opaque;
            ///@}
            /** @defgroup scm-runtimecontext-dvsmrcx-methods **/
            ///@{
            // const methods
            /** default allocator to use for objects **/
            static obj<AAllocator> allocator(const DVsmRcx & self) noexcept;
            /** collector facet for allocator. If non-null, same data pointer as allocator **/
            static obj<ACollector> collector(const DVsmRcx & self) noexcept;
            /** stringtable for unique symbols **/
            static StringTable * stringtable(const DVsmRcx & self) noexcept;
            /** invoke visitor for each distinct memory pool **/
            static void visit_pools(const DVsmRcx & self, MemorySizeVisitor visitor);

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */