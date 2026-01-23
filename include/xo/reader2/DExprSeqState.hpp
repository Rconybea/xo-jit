/** @file DExprSeqState.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "ParserStateMachine.hpp"
#include "SyntaxStateMachine.hpp"
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

        /** @class DExprSeqState
         *  @brief state machine for parsing a sequence of expression
         *
         *  Similar to exprseq_xs in xo-expresion
         **/
        class DExprSeqState {
        public:
            using TypeDescr = xo::reflect::TypeDescr;
            using AAllocator = xo::mm::AAllocator;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            explicit DExprSeqState(exprseqtype ty);

            /** start interactive top-level session **/
            static void establish_interactive(obj<AAllocator> mm,
                                              ParserStateMachine * p_psm);
            /** start non-interactive top-level session **/
            static void establish_batch(obj<AAllocator> mm,
                                        ParserStateMachine * p_psm);

        public:
            /** @defgroup scm-exprseq-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error mesages
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming symbol token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_symbol_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_def_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_if_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming colon token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_colon_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on incoming single-assign token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_singleassign_token(const Token & tk, ParserStateMachine * p_psm);

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

            /** update state for this syntax on incoming semicolon token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_semicolon_token(const Token & tk, ParserStateMachine * p_psm);

            /** update state for this syntax on parsed symbol @p sym
             *  from immediately-downstream ssm.
             *  overall parser state in @p p_psm
             **/
            void on_parsed_symbol(std::string_view sym, ParserStateMachine * p_psm);

            /** update state for this syntax on parsed type-description @p td
             *  from nested ssm.
             *  overall parser state in @p p_psm
             **/
            void on_parsed_typedescr(TypeDescr td, ParserStateMachine * p_psm);

            /** update state for this syntax on parsed expression @p expr
             *  from nested ssm.
             *  overall parser state in @p p_psm
             **/
            void on_parsed_expression(obj<AExpression> expr, ParserStateMachine * p_psm);

            /** update state for this syntax on parsed expression @p expr
             *  followed by semicolon from nested ssm.
             *  overall parser state in @p p_psm
             **/
            void on_parsed_expression_with_semicolon(obj<AExpression> expr, ParserStateMachine * p_psm);

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

/* end DExprSeqState.hpp */
