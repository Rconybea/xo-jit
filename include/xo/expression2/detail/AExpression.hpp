/** @file AExpression.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/Expression.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [abstract_facet.hpp.j2]
 *  3. idl for facet methods
 *       [idl/Expression.json5]
 **/

#pragma once

// includes (via {facet_includes})
#include "TypeRef.hpp"
#include "exprtype.hpp"
#include <xo/reflect/TypeDescr.hpp>
#include <xo/facet/obj.hpp>
#include <xo/facet/facet_implementation.hpp>
#include <xo/facet/typeseq.hpp>

namespace xo {
namespace scm {

using Copaque = const void *;
using Opaque = void *;

/**
Representation for executable Schematika expressions
**/
class AExpression {
public:
    /** @defgroup scm-expression-type-traits **/
    ///@{
    // types
    /** integer identifying a type **/
    using typeseq = xo::facet::typeseq;
    /** struct describing a type **/
    using TypeDescr = xo::reflect::TypeDescr;
    ///@}

    /** @defgroup scm-expression-methods **/
    ///@{
    // const methods
    /** RTTI: unique id# for actual runtime data representation **/
    virtual typeseq _typeseq() const noexcept = 0;
    /** expression type (constant | apply | ..) **/
    virtual exprtype extype(Copaque data)  const  noexcept = 0;
    /** placeholder for type giving possible values for this expression **/
    virtual TypeRef typeref(Copaque data)  const  noexcept = 0;
    /** type giving possible values for this expression. Maybe null before typecheck **/
    virtual TypeDescr valuetype(Copaque data)  const  noexcept = 0;

    // nonconst methods
    /** assing to valuetype member. Useful when scaffolding expressions **/
    virtual void assign_valuetype(Opaque data, TypeDescr td)  noexcept = 0;
    ///@}
}; /*AExpression*/

/** Implementation IExpression_DRepr of AExpression for state DRepr
 *  should provide a specialization:
 *
 *    template <>
 *    struct xo::facet::FacetImplementation<AExpression, DRepr> {
 *        using Impltype = IExpression_DRepr;
 *    };
 *
 *  then IExpression_ImplType<DRepr> --> IExpression_DRepr
 **/
template <typename DRepr>
using IExpression_ImplType = xo::facet::FacetImplType<AExpression, DRepr>;

} /*namespace scm*/
} /*namespace xo*/

/*  */