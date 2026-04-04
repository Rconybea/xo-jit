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

#include "env/IGCObject_DLocalEnv.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DLocalEnv::shallow_move(DLocalEnv & self, obj<ACollector> gc) noexcept -> Opaque
        {
            return self.shallow_move(gc);
        }
        auto
        IGCObject_DLocalEnv::forward_children(DLocalEnv & self, obj<ACollector> gc) noexcept -> void
        {
            self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DLocalEnv.cpp */
