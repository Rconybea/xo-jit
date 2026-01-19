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

        /** @class DExprSeqState
         *  @brief state machine for parsing a sequence of expression
         *
         *  Similar to exprseq_xs in xo-expresion
         **/
        class DExprSeqState {
        public:
            using AAllocator = xo::mm::AAllocator;

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

            /** update state for this syntax on incoming token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_if_token(const Token & tk, ParserStateMachine * p_psm);

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
