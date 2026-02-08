/** @file IGCObject_DLocalSymtab.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DLocalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DLocalSymtab.json5]
**/

#include "symtab/IGCObject_DLocalSymtab.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DLocalSymtab::shallow_size(const DLocalSymtab & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DLocalSymtab::shallow_copy(const DLocalSymtab & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DLocalSymtab::forward_children(DLocalSymtab & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DLocalSymtab.cpp */
