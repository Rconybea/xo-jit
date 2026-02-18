/** @file INumeric_Xfer.hpp
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


namespace xo {
namespace scm {
    /** @class INumeric_Xfer
     **/
    template <typename DRepr, typename INumeric_DRepr>
    class INumeric_Xfer : public ANumeric {
    public:
        /** @defgroup scm-numeric-xfer-type-traits **/
        ///@{
        /** actual implementation (not generated; often delegates to DRepr) **/
        using Impl = INumeric_DRepr;
        /** integer identifying a type **/
        using typeseq = ANumeric::typeseq;
        ///@}

        /** @defgroup scm-numeric-xfer-methods **/
        ///@{

        static const DRepr & _dcast(Copaque d) { return *(const DRepr *)d; }
        static DRepr & _dcast(Opaque d) { return *(DRepr *)d; }

        // from ANumeric

        // builtin methods
        typeseq _typeseq() const noexcept override { return s_typeseq; }
        void _drop(Opaque d) const noexcept override { _dcast(d).~DRepr(); }

        // const methods

        // non-const methods

        ///@}

    private:
        using I = Impl;

    public:
        /** @defgroup scm-numeric-xfer-member-vars **/
        ///@{

        /** typeseq for template parameter DRepr **/
        static typeseq s_typeseq;
        /** true iff satisfies facet implementation **/
        static bool _valid;

        ///@}
    };

    template <typename DRepr, typename INumeric_DRepr>
    xo::facet::typeseq
    INumeric_Xfer<DRepr, INumeric_DRepr>::s_typeseq
      = xo::facet::typeseq::id<DRepr>();

    template <typename DRepr, typename INumeric_DRepr>
    bool
    INumeric_Xfer<DRepr, INumeric_DRepr>::_valid
      = xo::facet::valid_facet_implementation<ANumeric,
                                              INumeric_Xfer>();

} /*namespace scm */
} /*namespace xo*/

/* end INumeric_Xfer.hpp */
