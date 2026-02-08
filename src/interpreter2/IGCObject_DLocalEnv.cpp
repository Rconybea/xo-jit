/** @file IGCObject_DLocalEnv.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DLocalEnv.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DLocalEnv.json5]
**/

#include "detail/IGCObject_DLocalEnv.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DLocalEnv::shallow_size(const DLocalEnv & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DLocalEnv::shallow_copy(const DLocalEnv & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DLocalEnv::forward_children(DLocalEnv & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DLocalEnv.cpp */
