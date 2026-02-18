/** @file RNumeric.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/Numeric.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/Numeric.json5]
 **/

#pragma once

#include "ANumeric.hpp"

namespace xo {
namespace scm {

/** @class RNumeric
 **/
template <typename Object>
class RNumeric : public Object {
private:
    using O = Object;

public:
    /** @defgroup scm-numeric-router-type-traits **/
    ///@{
    using ObjectType = Object;
    using DataPtr = Object::DataPtr;
    using typeseq = xo::reflect::typeseq;
    ///@}

    /** @defgroup scm-numeric-router-ctors **/
    ///@{
    RNumeric() {}
    RNumeric(Object::DataPtr data) : Object{std::move(data)} {}
    RNumeric(const ANumeric * iface, void * data)
      requires std::is_same_v<typename Object::DataType, xo::facet::DVariantPlaceholder>
      : Object(iface, data) {}

    ///@}
    /** @defgroup scm-numeric-router-methods **/
    ///@{

    // explicit injected content

    // builtin methods
    typeseq _typeseq() const noexcept { return O::iface()->_typeseq(); }
    void _drop() const noexcept { O::iface()->_drop(O::data()); }

    // const methods

    // non-const methods (still const in router!)

    ///@}
    /** @defgroup scm-numeric-member-vars **/
    ///@{

    static bool _valid;

    ///@}
};

template <typename Object>
bool
RNumeric<Object>::_valid = xo::facet::valid_object_router<Object>();

} /*namespace scm*/
} /*namespace xo*/

namespace xo { namespace facet {
    template <typename Object>
    struct RoutingFor<xo::scm::ANumeric, Object> {
        using RoutingType = xo::scm::RNumeric<Object>;
    };
} }

/* end RNumeric.hpp */
