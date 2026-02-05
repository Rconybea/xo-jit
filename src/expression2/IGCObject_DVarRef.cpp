/** @file IGCObject_DVarRef.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVarRef.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVarRef.json5]
**/

#include "detail/IGCObject_DVarRef.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVarRef::shallow_size(const DVarRef & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DVarRef::shallow_copy(const DVarRef & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DVarRef::forward_children(DVarRef & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVarRef.cpp */
