/** @file IPrintable_DUniqueString.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DUniqueString.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DUniqueString.json5]
**/

#include "detail/IPrintable_DUniqueString.hpp"

namespace xo {
    namespace scm {
        auto
        IPrintable_DUniqueString::pretty(const DUniqueString & self, const ppindentinfo & ppii) -> bool
        {
            return self.pretty(ppii);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end IPrintable_DUniqueString.cpp */