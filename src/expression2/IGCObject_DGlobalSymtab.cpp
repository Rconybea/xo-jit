/** @file IGCObject_DGlobalSymtab.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IGCObject_DGlobalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IGCObject_DGlobalSymtab.json5]
**/

#include "symtab/IGCObject_DGlobalSymtab.hpp"

namespace xo {
    namespace scm {
        auto
        IGCObject_DGlobalSymtab::shallow_size(const DGlobalSymtab & self) noexcept -> size_type
        {
            return self.shallow_size();
        }

        auto
        IGCObject_DGlobalSymtab::shallow_copy(const DGlobalSymtab & self, obj<AAllocator> mm) noexcept -> Opaque
        {
            return self.shallow_copy(mm);
        }

        auto
        IGCObject_DGlobalSymtab::forward_children(DGlobalSymtab & self, obj<ACollector> gc) noexcept -> size_type
        {
            return self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end IGCObject_DGlobalSymtab.cpp */
