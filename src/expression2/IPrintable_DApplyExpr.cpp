/** @file IPrintable_DApplyExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DApplyExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DApplyExpr.json5]
**/

#include "detail/IPrintable_DApplyExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IPrintable_DApplyExpr::pretty(const DApplyExpr & self, const ppindentinfo & ppii) -> bool
        {
            return self.pretty(ppii);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end IPrintable_DApplyExpr.cpp */