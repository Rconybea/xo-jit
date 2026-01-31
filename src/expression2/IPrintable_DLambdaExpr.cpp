/** @file IPrintable_DLambdaExpr.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/IPrintable_DLambdaExpr.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/IPrintable_DLambdaExpr.json5]
**/

#include "detail/IPrintable_DLambdaExpr.hpp"

namespace xo {
    namespace scm {
        auto
        IPrintable_DLambdaExpr::pretty(const DLambdaExpr & self, const ppindentinfo & ppii) -> bool
        {
            return self.pretty(ppii);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end IPrintable_DLambdaExpr.cpp */