/** @file ISymbolTable_DLocalSymtab.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISymbolTable_DLocalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISymbolTable_DLocalSymtab.json5]
**/

#include "symtab/ISymbolTable_DLocalSymtab.hpp"

namespace xo {
    namespace scm {
        auto
        ISymbolTable_DLocalSymtab::is_global_symtab(const DLocalSymtab & self) noexcept -> bool
        {
            return self.is_global_symtab();
        }

        auto
        ISymbolTable_DLocalSymtab::lookup_binding(const DLocalSymtab & self, const DUniqueString * sym) noexcept -> Binding
        {
            return self.lookup_binding(sym);
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end ISymbolTable_DLocalSymtab.cpp */
