/** @file IExpression_Xfer.hpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/Expression.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/Expression.json5]
 **/

#pragma once

#include "TypeRef.hpp"

namespace xo {
namespace scm {
    /** @class IExpression_Xfer
     **/
    template <typename DRepr, typename IExpression_DRepr>
    class IExpression_Xfer : public AExpression {
    public:
        /** @defgroup scm-expression-xfer-type-traits **/
        ///@{
        /** actual implementation (not generated; often delegates to DRepr) **/
        using Impl = IExpression_DRepr;
        /** integer identifying a type **/
        using typeseq = AExpression::typeseq;
        ///@}

        /** @defgroup scm-expression-xfer-methods **/
        ///@{

        static const DRepr & _dcast(Copaque d) { return *(const DRepr *)d; }
        static DRepr & _dcast(Opaque d) { return *(DRepr *)d; }

        // from AExpression

        // const methods
        typeseq _typeseq() const noexcept override { return s_typeseq; }
        TypeRef typeref(Copaque data)  const  noexcept override {
            return I::typeref(_dcast(data));
        }
        TypeDescr valuetype(Copaque data)  const  noexcept override {
            return I::valuetype(_dcast(data));
        }

        // non-const methods
        void assign_valuetype(Opaque data, TypeDescr td)  noexcept override {
            return I::assign_valuetype(_dcast(data), td);
        }

        ///@}

    private:
        using I = Impl;

    public:
        /** @defgroup scm-expression-xfer-member-vars **/
        ///@{

        /** typeseq for template parameter DRepr **/
        static typeseq s_typeseq;
        /** true iff satisfies facet implementation **/
        static bool _valid;

        ///@}
    };

    template <typename DRepr, typename IExpression_DRepr>
    xo::facet::typeseq
    IExpression_Xfer<DRepr, IExpression_DRepr>::s_typeseq
      = xo::facet::typeseq::id<DRepr>();

    template <typename DRepr, typename IExpression_DRepr>
    bool
    IExpression_Xfer<DRepr, IExpression_DRepr>::_valid
      = xo::facet::valid_facet_implementation<AExpression,
                                              IExpression_Xfer>();

} /*namespace scm */
} /*namespace xo*/

/* end IExpression_Xfer.hpp */