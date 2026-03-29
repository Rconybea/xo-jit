/** @file IRuntimeContext_DVsmRcx.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IRuntimeContext_DVsmRcx.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IRuntimeContext_DVsmRcx.json5]
**/

#include "detail/IRuntimeContext_DVsmRcx.hpp"

namespace xo {
    namespace scm {
        auto
        IRuntimeContext_DVsmRcx::allocator(const DVsmRcx & self) noexcept -> obj<AAllocator>
        {
            return self.allocator();
        }

        auto
        IRuntimeContext_DVsmRcx::collector(const DVsmRcx & self) noexcept -> obj<ACollector>
        {
            return self.collector();
        }

        auto
        IRuntimeContext_DVsmRcx::error_allocator(const DVsmRcx & self) noexcept -> obj<AAllocator>
        {
            return self.error_allocator();
        }

        auto
        IRuntimeContext_DVsmRcx::stringtable(const DVsmRcx & self) noexcept -> StringTable *
        {
            return self.stringtable();
        }

        auto
        IRuntimeContext_DVsmRcx::visit_pools(const DVsmRcx & self, MemorySizeVisitor visitor) -> void
        {
            self.visit_pools(visitor);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end IRuntimeContext_DVsmRcx.cpp */
