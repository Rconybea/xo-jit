/** @file DToplevelSeqSsm.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        enum class exprseqtype {
            /** toplevel interactive sequence.
             *  allows: rvalue expressions
             **/
            toplevel_interactive,
            /** toplevel non-interactive sequence.
             *  allows:
             **/
            toplevel_batch,
            /** counts number of valid enums **/
            N
        };

        const char * exprseqtype_descr(exprseqtype x);

        inline std::ostream & operator<<(std::ostream & os, exprseqtype x) {
            os << exprseqtype_descr(x);
            return os;
        }

        /** @class DToplevelSeqSsm
         *  @brief state machine for parsing a sequence of expression
         *
         *  Similar to exprseq_xs in xo-expresion
         **/
        class DToplevelSeqSsm : public DSyntaxStateMachine<DToplevelSeqSsm> {
        public:
            using Super = DSyntaxStateMachine<DToplevelSeqSsm>;
            using TypeDescr = xo::reflect::TypeDescr;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            explicit DToplevelSeqSsm(exprseqtype ty);

            /** start interactive top-level session **/
            static void establish_interactive(DArena & mm,
                                              ParserStateMachine * p_psm);
            /** start non-interactive top-level session **/
            static void establish_batch(DArena & mm,
                                        ParserStateMachine * p_psm);

        public:
            static const char * ssm_classname() { return "DToplevelSeqSsm"; }

            /** @defgroup scm-exprseq-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error mesages
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** operate state machine for this syntax on incoming symbol token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_symbol_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_def_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming lamdba token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_lambda_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_if_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming string token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_string_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming f64 token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_f64_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming i64 token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_i64_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming bool token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_bool_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming leftparen token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_leftparen_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on parsed expression @p expr
             *  from nested ssm.
             *  overall parser state in @p p_psm
             **/
            void on_parsed_expression(obj<AExpression> expr, ParserStateMachine * p_psm);

            /** update state for this syntax on parsed expression @p expr
             *  followed by token @p tk from nested ssm.
             *  overall parser state in @p p_psm
             **/
            void on_parsed_expression_with_token(obj<AExpression> expr,
                                                 const Token & tk,
                                                 ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-exprseq-printable-facet printable facet methods **/
            ///@{

            /** pretty-printing driver; combine layout+printing **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** sequence type. accept rvalue expressions when
             *  this is toplevel_interactive.
             *  Always accept definitions and declarations.
             **/
            exprseqtype seqtype_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DToplevelSeqSsm.hpp */
