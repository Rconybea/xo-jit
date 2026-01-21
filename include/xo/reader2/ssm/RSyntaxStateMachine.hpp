/** @file RSyntaxStateMachine.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/SyntaxStateMachine.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/SyntaxStateMachine.json5]
 **/

#pragma once

#include "ASyntaxStateMachine.hpp"

namespace xo {
namespace scm {

/** @class RSyntaxStateMachine
 **/
template <typename Object>
class RSyntaxStateMachine : public Object {
private:
    using O = Object;

public:
    /** @defgroup scm-syntaxstatemachine-router-type-traits **/
    ///@{
    using ObjectType = Object;
    using DataPtr = Object::DataPtr;
    using typeseq = xo::reflect::typeseq;
    using TypeDescr = ASyntaxStateMachine::TypeDescr;
    ///@}

    /** @defgroup scm-syntaxstatemachine-router-ctors **/
    ///@{
    RSyntaxStateMachine() {}
    RSyntaxStateMachine(Object::DataPtr data) : Object{std::move(data)} {}
    RSyntaxStateMachine(const ASyntaxStateMachine * iface, void * data)
      requires std::is_same_v<typename Object::DataType, xo::facet::DVariantPlaceholder>
      : Object(iface, data) {}

    ///@}
    /** @defgroup scm-syntaxstatemachine-router-methods **/
    ///@{

    // const methods
    typeseq _typeseq() const noexcept { return O::iface()->_typeseq(); }
    syntaxstatetype ssm_type()  const  noexcept {
        return O::iface()->ssm_type(O::data());
    }
    std::string_view get_expect_str()  const  noexcept {
        return O::iface()->get_expect_str(O::data());
    }

    // non-const methods (still const in router!)
    void on_symbol_token(const Token & tk, ParserStateMachine * p_psm)  {
        return O::iface()->on_symbol_token(O::data(), tk, p_psm);
    }
    void on_def_token(const Token & tk, ParserStateMachine * p_psm)  {
        return O::iface()->on_def_token(O::data(), tk, p_psm);
    }
    void on_if_token(const Token & tk, ParserStateMachine * p_psm)  {
        return O::iface()->on_if_token(O::data(), tk, p_psm);
    }
    void on_colon_token(const Token & tk, ParserStateMachine * p_psm)  {
        return O::iface()->on_colon_token(O::data(), tk, p_psm);
    }
    void on_singleassign_token(const Token & tk, ParserStateMachine * p_psm)  {
        return O::iface()->on_singleassign_token(O::data(), tk, p_psm);
    }
    void on_parsed_symbol(std::string_view sym, ParserStateMachine * p_psm)  {
        return O::iface()->on_parsed_symbol(O::data(), sym, p_psm);
    }
    void on_parsed_typedescr(TypeDescr td, ParserStateMachine * p_psm)  {
        return O::iface()->on_parsed_typedescr(O::data(), td, p_psm);
    }

    ///@}
    /** @defgroup scm-syntaxstatemachine-member-vars **/
    ///@{

    static bool _valid;

    ///@}
};

template <typename Object>
bool
RSyntaxStateMachine<Object>::_valid = xo::facet::valid_object_router<Object>();

} /*namespace scm*/
} /*namespace xo*/

namespace xo { namespace facet {
    template <typename Object>
    struct RoutingFor<xo::scm::ASyntaxStateMachine, Object> {
        using RoutingType = xo::scm::RSyntaxStateMachine<Object>;
    };
} }

/* end RSyntaxStateMachine.hpp */