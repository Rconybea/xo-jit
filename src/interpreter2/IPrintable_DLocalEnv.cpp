/** @file IPrintable_DLocalEnv.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DLocalEnv.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DLocalEnv.json5]
**/

#include "detail/IPrintable_DLocalEnv.hpp"

namespace xo {
    namespace scm {
        auto
        IPrintable_DLocalEnv::pretty(const DLocalEnv & self, const ppindentinfo & ppii) -> bool
        {
            return self.pretty(ppii);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end IPrintable_DLocalEnv.cpp */
