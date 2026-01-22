/** @file ISymbolTable_Xfer.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/SymbolTable.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/SymbolTable.json5]
 **/

#pragma once

#include "Binding.hpp"
#include "DUniqueString.hpp"

namespace xo {
namespace scm {
    /** @class ISymbolTable_Xfer
     **/
    template <typename DRepr, typename ISymbolTable_DRepr>
    class ISymbolTable_Xfer : public ASymbolTable {
    public:
        /** @defgroup scm-symboltable-xfer-type-traits **/
        ///@{
        /** actual implementation (not generated; often delegates to DRepr) **/
        using Impl = ISymbolTable_DRepr;
        /** integer identifying a type **/
        using typeseq = ASymbolTable::typeseq;
        ///@}

        /** @defgroup scm-symboltable-xfer-methods **/
        ///@{

        static const DRepr & _dcast(Copaque d) { return *(const DRepr *)d; }
        static DRepr & _dcast(Opaque d) { return *(DRepr *)d; }

        // from ASymbolTable

        // const methods
        typeseq _typeseq() const noexcept override { return s_typeseq; }
        bool is_global_symtab(Copaque data)  const  noexcept override {
            return I::is_global_symtab(_dcast(data));
        }
        Binding lookup_binding(Copaque data, const DUniqueString * sym)  const  noexcept override {
            return I::lookup_binding(_dcast(data), sym);
        }

        // non-const methods

        ///@}

    private:
        using I = Impl;

    public:
        /** @defgroup scm-symboltable-xfer-member-vars **/
        ///@{

        /** typeseq for template parameter DRepr **/
        static typeseq s_typeseq;
        /** true iff satisfies facet implementation **/
        static bool _valid;

        ///@}
    };

    template <typename DRepr, typename ISymbolTable_DRepr>
    xo::facet::typeseq
    ISymbolTable_Xfer<DRepr, ISymbolTable_DRepr>::s_typeseq
      = xo::facet::typeseq::id<DRepr>();

    template <typename DRepr, typename ISymbolTable_DRepr>
    bool
    ISymbolTable_Xfer<DRepr, ISymbolTable_DRepr>::_valid
      = xo::facet::valid_facet_implementation<ASymbolTable,
                                              ISymbolTable_Xfer>();

} /*namespace scm */
} /*namespace xo*/

/* end ISymbolTable_Xfer.hpp */