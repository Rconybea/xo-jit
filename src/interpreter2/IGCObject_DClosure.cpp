/** @file IGCObject_DClosure.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DClosure.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DClosure.json5]
**/

#include "detail/IGCObject_DClosure.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DClosure::shallow_size(const DClosure & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DClosure::shallow_copy(const DClosure & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DClosure::forward_children(DClosure & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DClosure.cpp */
