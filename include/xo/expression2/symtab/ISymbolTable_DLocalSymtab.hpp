/** @file ISymbolTable_DLocalSymtab.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISymbolTable_DLocalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISymbolTable_DLocalSymtab.json5]
 **/

#pragma once

#include "SymbolTable.hpp"
#include "DLocalSymtab.hpp"

namespace xo { namespace scm { class ISymbolTable_DLocalSymtab; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASymbolTable,
                                   xo::scm::DLocalSymtab>
        {
            using ImplType = xo::scm::ISymbolTable_Xfer
              <xo::scm::DLocalSymtab,
               xo::scm::ISymbolTable_DLocalSymtab>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISymbolTable_DLocalSymtab
         **/
        class ISymbolTable_DLocalSymtab {
        public:
            /** @defgroup scm-symboltable-dlocalsymtab-type-traits **/
            ///@{
            using Copaque = xo::scm::ASymbolTable::Copaque;
            using Opaque = xo::scm::ASymbolTable::Opaque;
            ///@}
            /** @defgroup scm-symboltable-dlocalsymtab-methods **/
            ///@{
            // const methods
            /** true iff this is toplevel (global) symbol table. **/
            static bool is_global_symtab(const DLocalSymtab & self) noexcept;
            /** report ingredients needed to address variable at runtime. **/
            static Binding lookup_binding(const DLocalSymtab & self, const DUniqueString * sym) noexcept;

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */