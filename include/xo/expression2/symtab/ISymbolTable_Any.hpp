/** @file ISymbolTable_Any.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/SymbolTable.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/SymbolTable.json5]
 **/

#pragma once

#include "ASymbolTable.hpp"
#include <xo/facet/obj.hpp>

namespace xo { namespace scm { class ISymbolTable_Any; } }

namespace xo {
namespace facet {

template <>
struct FacetImplementation<xo::scm::ASymbolTable,
                           DVariantPlaceholder>
{
    using ImplType = xo::scm::ISymbolTable_Any;
};

}
}

namespace xo {
namespace scm {

    /** @class ISymbolTable_Any
     *  @brief ASymbolTable implementation for empty variant instance
     **/
    class ISymbolTable_Any : public ASymbolTable {
    public:
        /** @defgroup scm-symboltable-any-type-traits **/
        ///@{

        /** integer identifying a type **/
        using typeseq = xo::facet::typeseq;

        ///@}
        /** @defgroup scm-symboltable-any-methods **/
        ///@{

        const ASymbolTable * iface() const { return std::launder(this); }

        // from ASymbolTable

        // const methods
        typeseq _typeseq() const noexcept override { return s_typeseq; }
        [[noreturn]] bool is_global_symtab(Copaque)  const  noexcept override { _fatal(); }
        [[noreturn]] Binding lookup_binding(Copaque, const DUniqueString *)  const  noexcept override { _fatal(); }

        // nonconst methods

        ///@}

    private:
        /** @defgraoup scm-symboltable-any-private-methods **/
        ///@{

        [[noreturn]] static void _fatal();

        ///@}

    public:
        /** @defgroup scm-symboltable-any-member-vars **/
        ///@{

        static typeseq s_typeseq;
        static bool _valid;

        ///@}
    };

} /*namespace scm */
} /*namespace xo */

/* ISymbolTable_Any.hpp */