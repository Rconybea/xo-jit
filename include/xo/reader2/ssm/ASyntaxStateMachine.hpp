/** @file ASyntaxStateMachine.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/SyntaxStateMachine.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [abstract_facet.hpp.j2]
 *  3. idl for facet methods
 *       [idl/SyntaxStateMachine.json5]
 **/

#pragma once

// includes (via {facet_includes})
#include "ParserStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/tokenizer2/Token.hpp>
#include <xo/facet/obj.hpp>
#include <xo/facet/facet_implementation.hpp>
#include <xo/facet/typeseq.hpp>

// {pretex} here

namespace xo {
namespace scm {

using Copaque = const void *;
using Opaque = void *;

/**
Assistant to schematika parser dedicated to particular syntax
**/
class ASyntaxStateMachine {
public:
    /** @defgroup scm-syntaxstatemachine-type-traits **/
    ///@{
    // types
    /** integer identifying a type **/
    using typeseq = xo::facet::typeseq;
    using Copaque = const void *;
    using Opaque = void *;
    ///@}

    /** @defgroup scm-syntaxstatemachine-methods **/
    ///@{
    // const methods
    /** RTTI: unique id# for actual runtime data representation **/
    virtual typeseq _typeseq() const noexcept = 0;
    /** identify a type of syntax state machine **/
    virtual syntaxstatetype ssm_type(Copaque data)  const  noexcept = 0;
    /** text describing expected/allowed input to this ssm in current state **/
    virtual std::string_view get_expect_str(Copaque data)  const  noexcept = 0;

    // nonconst methods
    /** update state machine for incoming define-keyworkd-token @p tk **/
    virtual void on_def_token(Opaque data, const Token & tk, ParserStateMachine * ps_psm)  = 0;
    /** update state machine for incoming if-keyword-token @p tk **/
    virtual void on_if_token(Opaque data, const Token & tk, ParserStateMachine * p_psm)  = 0;
    ///@}
}; /*ASyntaxStateMachine*/

/** Implementation ISyntaxStateMachine_DRepr of ASyntaxStateMachine for state DRepr
 *  should provide a specialization:
 *
 *    template <>
 *    struct xo::facet::FacetImplementation<ASyntaxStateMachine, DRepr> {
 *        using Impltype = ISyntaxStateMachine_DRepr;
 *    };
 *
 *  then ISyntaxStateMachine_ImplType<DRepr> --> ISyntaxStateMachine_DRepr
 **/
template <typename DRepr>
using ISyntaxStateMachine_ImplType = xo::facet::FacetImplType<ASyntaxStateMachine, DRepr>;

} /*namespace scm*/
} /*namespace xo*/

/* ASyntaxStateMachine.hpp */