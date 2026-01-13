/** @file RExpression.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/Expression.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/Expression.json5]
 **/

#pragma once

#include "AExpression.hpp"

namespace xo {
namespace scm {

/** @class RExpression
 **/
template <typename Object>
class RExpression : public Object {
private:
    using O = Object;

public:
    /** @defgroup scm-expression-router-type-traits **/
    ///@{
    using ObjectType = Object;
    using DataPtr = Object::DataPtr;
    using typeseq = xo::reflect::typeseq;
    using TypeDescr = AExpression::TypeDescr;
    ///@}

    /** @defgroup scm-expression-router-ctors **/
    ///@{
    RExpression() {}
    RExpression(Object::DataPtr data) : Object{std::move(data)} {}

    ///@}
    /** @defgroup scm-expression-router-methods **/
    ///@{

    // const methods
    typeseq _typeseq() const noexcept { return O::iface()->_typeseq(); }
    exprtype extype()  const  noexcept {
        return O::iface()->extype(O::data());
    }
    TypeRef typeref()  const  noexcept {
        return O::iface()->typeref(O::data());
    }
    TypeDescr valuetype()  const  noexcept {
        return O::iface()->valuetype(O::data());
    }

    // non-const methods
    // << do something for non-const methods >>
    //

    ///@}
    /** @defgroup scm-expression-member-vars **/
    ///@{

    static bool _valid;

    ///@}
};

template <typename Object>
bool
RExpression<Object>::_valid = xo::facet::valid_object_router<Object>();

} /*namespace scm*/
} /*namespace xo*/

namespace xo { namespace facet {
    template <typename Object>
    struct RoutingFor<xo::scm::AExpression, Object> {
        using RoutingType = xo::scm::RExpression<Object>;
    };
} }

/* end RExpression.hpp */