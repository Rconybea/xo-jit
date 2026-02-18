/** @file ANumeric.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/Numeric.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [abstract_facet.hpp.j2]
 *  3. idl for facet methods
 *       [idl/Numeric.json5]
 **/

#pragma once

// includes (via {facet_includes})
#include <xo/facet/obj.hpp>
#include <xo/facet/facet_implementation.hpp>
#include <xo/facet/typeseq.hpp>


namespace xo {
namespace scm {

using Copaque = const void *;
using Opaque = void *;

/**
Abstraction for a schematika numeric
**/
class ANumeric {
public:
    /** @defgroup scm-numeric-type-traits **/
    ///@{
    // types
    /** integer identifying a type **/
    using typeseq = xo::facet::typeseq;
    using Copaque = const void *;
    using Opaque = void *;
    ///@}

    /** @defgroup scm-numeric-methods **/
    ///@{
    // const methods
    /** RTTI: unique id# for actual runtime data representation **/
    virtual typeseq _typeseq() const noexcept = 0;
    /** destroy instance @p d; calls c++ dtor only for actual runtime type; does not recover memory **/
    virtual void _drop(Opaque d) const noexcept = 0;

    // nonconst methods
    ///@}
}; /*ANumeric*/

/** Implementation INumeric_DRepr of ANumeric for state DRepr
 *  should provide a specialization:
 *
 *    template <>
 *    struct xo::facet::FacetImplementation<ANumeric, DRepr> {
 *        using Impltype = INumeric_DRepr;
 *    };
 *
 *  then INumeric_ImplType<DRepr> --> INumeric_DRepr
 **/
template <typename DRepr>
using INumeric_ImplType = xo::facet::FacetImplType<ANumeric, DRepr>;

} /*namespace scm*/
} /*namespace xo*/

/* ANumeric.hpp */
