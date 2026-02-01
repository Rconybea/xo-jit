/** @file DSequenceSsm.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include <xo/expression2/DSequenceExpr.hpp>
#include "syntaxstatetype.hpp"
#include <xo/expression2/detail/IExpression_DSequenceExpr.hpp>
#include <xo/expression2/DSequenceExpr.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm { class Sequence; }
    namespace scm { class Lambda; }

    namespace scm {
        // TODO: need switching between 1a,1b states.
        // Allow
        //  { }
        //  { 1 }
        //  { 1; }
        // Reject
        //  { 1 2 }
        //

        class DSequenceSsm : public DSyntaxStateMachine<DSequenceSsm> {
        public:
            //using Sequence = xo::scm::Sequence;
            //using Lambda = xo::scm::Lambda;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            static const char * ssm_classname() { return "DSequenceSsm"; }

            /** start parsing a sequence-expr.
             *  input begins with first expression in the sequence.
             **/
            static void start(ParserStateMachine * p_psm);

            /** create instance using memory from @p parser_mm **/
            static obj<ASyntaxStateMachine,DSequenceSsm> make(DArena & parser_mm,
                                                              obj<AAllocator> expr_mm);

            /** create instance using memory from @p parser_mm **/
            static DSequenceSsm * _make(DArena & parser_mm,
                                        obj<AAllocator> expr_mm);

#ifdef NOT_YET
            virtual void on_expr_with_semicolon(bp<Expression> expr,
                                                parserstatemachine * p_psm) override;
#endif

            /** update ssm for incoming rightbrace token '}' **/
            void on_rightbrace_token(const Token & tk,
                                     ParserStateMachine * p_psm);

            /** @defgroup scm-sequencessm-syntaxstatemachine-facet ssm facet **/
            ///@{

            /** indentifies this state machine **/
            syntaxstatetype ssm_type() const noexcept;

            /** mnemonic for syntax sequence ssm expects given current state **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** consume expression @p expr produced by nested ssm; overall parser state in @p p_psm **/
            void on_parsed_expression(obj<AExpression> expr,
                                      ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-sequencessm-printable-facet printable facet **/
            ///@{

            /** pretty printing support **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            explicit DSequenceSsm(DSequenceExpr * seq_expr);

        private:
            /** scaffold sequence-expression here.
             *  This will eventually be the output of this ssm
             **/
            DSequenceExpr * seq_expr_ = nullptr;
        };
    } /*namespace scm*/
} /*namespace xo*/


/* end DSequenceSsm.hpp */
