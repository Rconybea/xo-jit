/** @file DSequenceSsm.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include <xo/expression2/DSequenceExpr.hpp>

namespace xo {
    namespace scm { class Sequence; }
    namespace scm { class Lambda; }

    namespace scm {
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
            virtual void on_expr(bp<Expression> expr,
                                 parserstatemachine * p_psm) override;
            virtual void on_expr_with_semicolon(bp<Expression> expr,
                                                parserstatemachine * p_psm) override;

            virtual void on_rightbrace_token(const token_type & tk,
                                             parserstatemachine * p_psm) override;
#endif

            /** @defgroup scm-sequencessm-syntaxstatemachine-facet ssm facet **/
            ///@{

            /** indentifies this state machine **/
            syntaxstatetype ssm_type() const noexcept;

            /** mnemonic for syntax sequence ssm expects given current state **/
            std::string_view get_expect_str() const noexcept;

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
