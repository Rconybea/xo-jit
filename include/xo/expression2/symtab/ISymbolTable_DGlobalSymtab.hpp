/** @file ISymbolTable_DGlobalSymtab.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISymbolTable_DGlobalSymtab.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_repr.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISymbolTable_DGlobalSymtab.json5]
 **/

#pragma once

#include "SymbolTable.hpp"
#include "DGlobalSymtab.hpp"

namespace xo { namespace scm { class ISymbolTable_DGlobalSymtab; } }

namespace xo {
    namespace facet {
        template <>
        struct FacetImplementation<xo::scm::ASymbolTable,
                                   xo::scm::DGlobalSymtab>
        {
            using ImplType = xo::scm::ISymbolTable_Xfer
              <xo::scm::DGlobalSymtab,
               xo::scm::ISymbolTable_DGlobalSymtab>;
        };
    }
}

namespace xo {
    namespace scm {
        /** @class ISymbolTable_DGlobalSymtab
         **/
        class ISymbolTable_DGlobalSymtab {
        public:
            /** @defgroup scm-symboltable-dglobalsymtab-type-traits **/
            ///@{
            using Copaque = xo::scm::ASymbolTable::Copaque;
            using Opaque = xo::scm::ASymbolTable::Opaque;
            ///@}
            /** @defgroup scm-symboltable-dglobalsymtab-methods **/
            ///@{
            // const methods
            /** true iff this is toplevel (global) symbol table. **/
            static bool is_global_symtab(const DGlobalSymtab & self) noexcept;
            /** report ingredients needed to address variable at runtime. **/
            static Binding lookup_binding(const DGlobalSymtab & self, const DUniqueString * sym) noexcept;

            // non-const methods
            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end */