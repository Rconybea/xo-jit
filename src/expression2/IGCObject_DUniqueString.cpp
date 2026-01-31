/** @file IGCObject_DUniqueString.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DUniqueString.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DUniqueString.json5]
**/

#include "detail/IGCObject_DUniqueString.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DUniqueString::shallow_size(const DUniqueString & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DUniqueString::shallow_copy(const DUniqueString & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DUniqueString::forward_children(DUniqueString & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DUniqueString.cpp */