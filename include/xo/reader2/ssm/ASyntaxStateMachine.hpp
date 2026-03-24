/** @file ASyntaxStateMachine.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
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
#include <xo/type/Type.hpp>
#include <xo/tokenizer2/Token.hpp>
#include <xo/reflect/TypeDescr.hpp>
#include <xo/alloc2/Collector.hpp>
#include <xo/facet/obj.hpp>
#include <xo/facet/facet_implementation.hpp>
#include <xo/facet/typeseq.hpp>

// {pretext} here

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
    /** reflected c++ type **/
    using TypeDescr = xo::reflect::TypeDescr;
    /** gc interface **/
    using ACollector = xo::mm::ACollector;
    /** gc-aware object **/
    using AGCObject = xo::mm::AGCObject;
    ///@}

    /** @defgroup scm-syntaxstatemachine-methods **/
    ///@{
    // const methods
    /** An uninitialized ASyntaxStateMachine instance will have zero vtable pointer (per {linux,osx} abi).
     *  Use case for this is narrow. We go to some lengths to avoid null vtable pointers. For example
     *  obj<AFacet> will have non-null vtable (via IFacet_Any) with all methods terminating.
     **/
    bool _has_null_vptr() const noexcept { return *reinterpret_cast<const void * const *>(this) == nullptr; }
    /** RTTI: unique id# for actual runtime data representation **/
    virtual typeseq _typeseq() const noexcept = 0;
    /** destroy instance @p d; calls c++ dtor only for actual runtime type; does not recover memory **/
    virtual void _drop(Opaque d) const noexcept = 0;
    /** identify a type of syntax state machine **/
    virtual syntaxstatetype ssm_type(Copaque data)  const  noexcept = 0;
    /** text describing expected/allowed input to this ssm in current state **/
    virtual std::string_view get_expect_str(Copaque data)  const  noexcept = 0;

    // nonconst methods
    /** operate state machine for incoming token @p tk **/
    virtual void on_token(Opaque data, const Token & tk, ParserStateMachine * p_psm)  = 0;
    /** update  stat machine for incoming parsed symbol @p sym **/
    virtual void on_parsed_symbol(Opaque data, std::string_view sym, ParserStateMachine * p_psm)  = 0;
    /** operate state machine for incoming type description @p td **/
    virtual void on_parsed_typedescr(Opaque data, TypeDescr td, ParserStateMachine * p_psm)  = 0;
    /** update state machine for type emitted by nested ssm **/
    virtual void on_parsed_type(Opaque data, obj<AType> type, ParserStateMachine * p_psm)  = 0;
    /** operate state machine for formal emitted by nested ssm **/
    virtual void on_parsed_formal(Opaque data, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm)  = 0;
    /** operate state machine for formal emitted by nested ssm **/
    virtual void on_parsed_formal_with_token(Opaque data, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm)  = 0;
    /** consume formal arglist emitted by nested ssm **/
    virtual void on_parsed_formal_arglist(Opaque data, DArray * arglist, ParserStateMachine * p_psm)  = 0;
    /** update state machine for nested parsed expression @p expr **/
    virtual void on_parsed_expression(Opaque data, obj<AExpression> expr, ParserStateMachine * p_psm)  = 0;
    /** update state machine @p p_psm for incoming parsed expression @p expr followed by token @p tk **/
    virtual void on_parsed_expression_with_token(Opaque data, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm)  = 0;
    /** update state machine for nested quoted literal @p lit **/
    virtual void on_quoted_literal(Opaque data, obj<AGCObject> lit, ParserStateMachine * p_psm)  = 0;
    /** gc support: move immediate children to to-space and sub forwarding pointer **/
    virtual void forward_children(Opaque data, obj<ACollector> gc)  = 0;
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
