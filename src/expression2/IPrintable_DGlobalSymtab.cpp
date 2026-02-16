/** @file IPrintable_DGlobalSymtab.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DGlobalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DGlobalSymtab.json5]
**/

#include "symtab/IPrintable_DGlobalSymtab.hpp"

namespace xo {
    namespace scm {
        auto
        IPrintable_DGlobalSymtab::pretty(const DGlobalSymtab & self, const ppindentinfo & ppii) -> bool
        {
            return self.pretty(ppii);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end IPrintable_DGlobalSymtab.cpp */
