/** @file IGCObject_DVsmDefContFrame.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmDefContFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmDefContFrame.json5]
**/

#include "define/IGCObject_DVsmDefContFrame.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVsmDefContFrame::shallow_size(const DVsmDefContFrame & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DVsmDefContFrame::shallow_copy(const DVsmDefContFrame & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DVsmDefContFrame::forward_children(DVsmDefContFrame & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVsmDefContFrame.cpp */
