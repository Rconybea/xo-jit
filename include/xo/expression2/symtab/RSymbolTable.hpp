/** @file RSymbolTable.hpp
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

namespace xo {
namespace scm {

/** @class RSymbolTable
 **/
template <typename Object>
class RSymbolTable : public Object {
private:
    using O = Object;

public:
    /** @defgroup scm-symboltable-router-type-traits **/
    ///@{
    using ObjectType = Object;
    using DataPtr = Object::DataPtr;
    using typeseq = xo::reflect::typeseq;
    ///@}

    /** @defgroup scm-symboltable-router-ctors **/
    ///@{
    RSymbolTable() {}
    RSymbolTable(Object::DataPtr data) : Object{std::move(data)} {}
    RSymbolTable(const ASymbolTable * iface, void * data)
      requires std::is_same_v<typename Object::DataType, xo::facet::DVariantPlaceholder>
      : Object(iface, data) {}

    ///@}
    /** @defgroup scm-symboltable-router-methods **/
    ///@{

    // const methods
    typeseq _typeseq() const noexcept { return O::iface()->_typeseq(); }
    bool is_global_symtab()  const  noexcept {
        return O::iface()->is_global_symtab(O::data());
    }
    Binding lookup_binding(const DUniqueString * sym)  const  noexcept {
        return O::iface()->lookup_binding(O::data(), sym);
    }

    // non-const methods (still const in router!)

    ///@}
    /** @defgroup scm-symboltable-member-vars **/
    ///@{

    static bool _valid;

    ///@}
};

template <typename Object>
bool
RSymbolTable<Object>::_valid = xo::facet::valid_object_router<Object>();

} /*namespace scm*/
} /*namespace xo*/

namespace xo { namespace facet {
    template <typename Object>
    struct RoutingFor<xo::scm::ASymbolTable, Object> {
        using RoutingType = xo::scm::RSymbolTable<Object>;
    };
} }

/* end RSymbolTable.hpp */