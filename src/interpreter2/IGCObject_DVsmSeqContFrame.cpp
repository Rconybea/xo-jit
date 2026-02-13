/** @file IGCObject_DVsmSeqContFrame.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DVsmSeqContFrame.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DVsmSeqContFrame.json5]
**/

#include "sequence/IGCObject_DVsmSeqContFrame.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DVsmSeqContFrame::shallow_size(const DVsmSeqContFrame & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DVsmSeqContFrame::shallow_copy(const DVsmSeqContFrame & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DVsmSeqContFrame::forward_children(DVsmSeqContFrame & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DVsmSeqContFrame.cpp */
