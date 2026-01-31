/** @file ASymbolTable.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/SymbolTable.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [abstract_facet.hpp.j2]
 *  3. idl for facet methods
 *       [idl/SymbolTable.json5]
 **/

#pragma once

// includes (via {facet_includes})
#include "Binding.hpp"
#include "DUniqueString.hpp"
#include <xo/facet/obj.hpp>
#include <xo/facet/facet_implementation.hpp>
#include <xo/facet/typeseq.hpp>

// {pretext} here

namespace xo {
namespace scm {

using Copaque = const void *;
using Opaque = void *;

/**
Map symbols to schematika expressions. Output of schematika parser
**/
class ASymbolTable {
public:
    /** @defgroup scm-symboltable-type-traits **/
    ///@{
    // types
    /** integer identifying a type **/
    using typeseq = xo::facet::typeseq;
    using Copaque = const void *;
    using Opaque = void *;
    ///@}

    /** @defgroup scm-symboltable-methods **/
    ///@{
    // const methods
    /** RTTI: unique id# for actual runtime data representation **/
    virtual typeseq _typeseq() const noexcept = 0;
    /** true iff this is toplevel (global) symbol table. **/
    virtual bool is_global_symtab(Copaque data)  const  noexcept = 0;
    /** report ingredients needed to address variable at runtime. **/
    virtual Binding lookup_binding(Copaque data, const DUniqueString * sym)  const  noexcept = 0;

    // nonconst methods
    ///@}
}; /*ASymbolTable*/

/** Implementation ISymbolTable_DRepr of ASymbolTable for state DRepr
 *  should provide a specialization:
 *
 *    template <>
 *    struct xo::facet::FacetImplementation<ASymbolTable, DRepr> {
 *        using Impltype = ISymbolTable_DRepr;
 *    };
 *
 *  then ISymbolTable_ImplType<DRepr> --> ISymbolTable_DRepr
 **/
template <typename DRepr>
using ISymbolTable_ImplType = xo::facet::FacetImplType<ASymbolTable, DRepr>;

} /*namespace scm*/
} /*namespace xo*/

/* ASymbolTable.hpp */