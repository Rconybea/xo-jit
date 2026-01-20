/** @file ISyntaxStateMachine_Xfer.hpp
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

#include "ParserStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/tokenizer2/Token.hpp>

namespace xo {
namespace scm {
    /** @class ISyntaxStateMachine_Xfer
     **/
    template <typename DRepr, typename ISyntaxStateMachine_DRepr>
    class ISyntaxStateMachine_Xfer : public ASyntaxStateMachine {
    public:
        /** @defgroup scm-syntaxstatemachine-xfer-type-traits **/
        ///@{
        /** actual implementation (not generated; often delegates to DRepr) **/
        using Impl = ISyntaxStateMachine_DRepr;
        /** integer identifying a type **/
        using typeseq = ASyntaxStateMachine::typeseq;
        ///@}

        /** @defgroup scm-syntaxstatemachine-xfer-methods **/
        ///@{

        static const DRepr & _dcast(Copaque d) { return *(const DRepr *)d; }
        static DRepr & _dcast(Opaque d) { return *(DRepr *)d; }

        // from ASyntaxStateMachine

        // const methods
        typeseq _typeseq() const noexcept override { return s_typeseq; }
        syntaxstatetype ssm_type(Copaque data)  const  noexcept override {
            return I::ssm_type(_dcast(data));
        }
        std::string_view get_expect_str(Copaque data)  const  noexcept override {
            return I::get_expect_str(_dcast(data));
        }

        // non-const methods
        void on_symbol_token(Opaque data, const Token & tk, ParserStateMachine * p_psm)  override {
            return I::on_symbol_token(_dcast(data), tk, p_psm);
        }
        void on_def_token(Opaque data, const Token & tk, ParserStateMachine * p_psm)  override {
            return I::on_def_token(_dcast(data), tk, p_psm);
        }
        void on_if_token(Opaque data, const Token & tk, ParserStateMachine * p_psm)  override {
            return I::on_if_token(_dcast(data), tk, p_psm);
        }
        void on_parsed_symbol(Opaque data, std::string_view sym, ParserStateMachine * p_psm)  override {
            return I::on_parsed_symbol(_dcast(data), sym, p_psm);
        }

        ///@}

    private:
        using I = Impl;

    public:
        /** @defgroup scm-syntaxstatemachine-xfer-member-vars **/
        ///@{

        /** typeseq for template parameter DRepr **/
        static typeseq s_typeseq;
        /** true iff satisfies facet implementation **/
        static bool _valid;

        ///@}
    };

    template <typename DRepr, typename ISyntaxStateMachine_DRepr>
    xo::facet::typeseq
    ISyntaxStateMachine_Xfer<DRepr, ISyntaxStateMachine_DRepr>::s_typeseq
      = xo::facet::typeseq::id<DRepr>();

    template <typename DRepr, typename ISyntaxStateMachine_DRepr>
    bool
    ISyntaxStateMachine_Xfer<DRepr, ISyntaxStateMachine_DRepr>::_valid
      = xo::facet::valid_facet_implementation<ASyntaxStateMachine,
                                              ISyntaxStateMachine_Xfer>();

} /*namespace scm */
} /*namespace xo*/

/* end ISyntaxStateMachine_Xfer.hpp */