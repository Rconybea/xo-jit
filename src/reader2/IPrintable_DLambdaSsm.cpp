/** @file IPrintable_DLambdaSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DLambdaSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DLambdaSsm.json5]
**/

#include "ssm/IPrintable_DLambdaSsm.hpp"

namespace xo {
    namespace scm {
        auto
        IPrintable_DLambdaSsm::pretty(const DLambdaSsm & self, const ppindentinfo & ppii) -> bool
        {
            return self.pretty(ppii);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end IPrintable_DLambdaSsm.cpp */
