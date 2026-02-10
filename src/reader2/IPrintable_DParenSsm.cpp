/** @file IPrintable_DParenSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DParenSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DParenSsm.json5]
**/

#include "ssm/IPrintable_DParenSsm.hpp"

namespace xo {
    namespace scm {
        auto
        IPrintable_DParenSsm::pretty(const DParenSsm & self, const ppindentinfo & ppii) -> bool
        {
            return self.pretty(ppii);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end IPrintable_DParenSsm.cpp */
