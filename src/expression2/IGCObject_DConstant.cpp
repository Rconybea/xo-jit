/** @file IGCObject_DConstant.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DConstant.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DConstant.json5]
**/

#include "detail/IGCObject_DConstant.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DConstant::shallow_size(const DConstant & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DConstant::shallow_copy(const DConstant & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DConstant::forward_children(DConstant & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DConstant.cpp */
