/** @file IPrintable_DExprSeqState.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DExprSeqState.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DExprSeqState.json5]
**/

#include "ssm/IPrintable_DExprSeqState.hpp"

namespace xo {
    namespace scm {
        auto
        IPrintable_DExprSeqState::pretty(const DExprSeqState & self, const ppindentinfo & ppii) -> bool
        {
            return self.pretty(ppii);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end IPrintable_DExprSeqState.cpp */
