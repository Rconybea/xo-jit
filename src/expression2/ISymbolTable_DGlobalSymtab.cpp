/** @file ISymbolTable_DGlobalSymtab.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISymbolTable_DGlobalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISymbolTable_DGlobalSymtab.json5]
**/

#include "symtab/ISymbolTable_DGlobalSymtab.hpp"

namespace xo {
    namespace scm {
        auto
        ISymbolTable_DGlobalSymtab::is_global_symtab(const DGlobalSymtab & self) noexcept -> bool
        {
            return self.is_global_symtab();
        }

        auto
        ISymbolTable_DGlobalSymtab::lookup_binding(const DGlobalSymtab & self, const DUniqueString * sym) noexcept -> Binding
        {
            return self.lookup_binding(sym);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end ISymbolTable_DGlobalSymtab.cpp */
