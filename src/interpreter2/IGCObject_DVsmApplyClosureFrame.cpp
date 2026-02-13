/** @file IGCObject_DVsmApplyClosureFrame.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmApplyClosureFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmApplyClosureFrame.json5]
**/

#include "detail/IGCObject_DVsmApplyClosureFrame.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVsmApplyClosureFrame::shallow_size(const DVsmApplyClosureFrame & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DVsmApplyClosureFrame::shallow_copy(const DVsmApplyClosureFrame & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DVsmApplyClosureFrame::forward_children(DVsmApplyClosureFrame & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVsmApplyClosureFrame.cpp */
