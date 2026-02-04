/** @file IGCObject_DVsmApplyFrame.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmApplyFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmApplyFrame.json5]
**/

#include "detail/IGCObject_DVsmApplyFrame.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVsmApplyFrame::shallow_size(const DVsmApplyFrame & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DVsmApplyFrame::shallow_copy(const DVsmApplyFrame & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DVsmApplyFrame::forward_children(DVsmApplyFrame & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVsmApplyFrame.cpp */
