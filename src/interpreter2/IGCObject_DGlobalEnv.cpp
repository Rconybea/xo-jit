/** @file IGCObject_DGlobalEnv.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DGlobalEnv.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DGlobalEnv.json5]
**/

#include "env/IGCObject_DGlobalEnv.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DGlobalEnv::shallow_size(const DGlobalEnv & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DGlobalEnv::shallow_copy(const DGlobalEnv & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DGlobalEnv::forward_children(DGlobalEnv & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DGlobalEnv.cpp */
