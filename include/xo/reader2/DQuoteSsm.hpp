/** @file DQuoteSsm.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/facet/obj.hpp>
#include <string_view>

namespace xo {
    namespace scm {

        /**
         * @pre
         *
         *   #q{ quote-expr }
         *  ^  ^ ^         ^ ^
         *  |  | |         | (done)
         *  |  | |         |
         *  |  | |         quote_3:expect_rightbrace
         *  |  | quote_2:expect_parsed_expression
         *  |  quote_1:expect_leftbrace
         *  quote_0
         *
         *   quote_0 --on_quote_token()--> quote_1
         *   quote_1 --on_leftbrace_token()--> quote_2
         *   quote_2 --on_parsed_expression()--> quote_3 [will be constant expression]
         *   quote_3 --on_rightparen_token()--> (done)
         *
         * @endpre
         **/
        class QuoteXst {
        public:
            enum class code {
                invalid = -1,

                quote_0,
                quote_1,
                quote_2,
                quote_3,

                N,
            };

            explicit QuoteXst(code x) : code_{x} {}

            /** @return string representation for enum @p x **/
            static const char * _descr(code x);

            code code() const noexcept { return code_; }

            enum code code_;
        };

        inline std::ostream &
        operator<<(std::ostream & os, QuoteXst x) {
            os << QuoteXst::_descr(x.code_);
            return os;
        }

        class DQuoteSsm : public DSyntaxStateMachine<DQuoteSsm> {
        public:
            using Super = DSyntaxStateMachine<DQuoteSsm>;
            using TypeDescr = xo::reflect::TypeDescr;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-parenssm-ctors **/
            ///@{

            DQuoteSsm();

            /** create instance using memory from @p parser_mm
             **/
            static DQuoteSsm * _make(DArena & parser_mm);

            /** create fop pointing with new DQuoteSsm using memory from @p parser_mm **/
            static obj<ASyntaxStateMachine,DQuoteSsm> make(DArena & parser_mm);

            /** push DQuoteSsm instance onto @p p_psm stack **/
            static void start(ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-parenssm-access-methods **/
            ///@{

            /** identify this nested state machine **/
            static const char * ssm_classname() { return "DQuoteSsm"; }

            /** internal state **/
            QuoteXst quotestate() const noexcept { return quote_xst_; }

            ///@}
            /** @defgroup scm-parenssm-admin-methods admin methods **/
            ///@{

            /** update ssm for incoming quote token @p tk.
             *  with overall parser state in @p p_psm.
             *  advances from quote_0 -> quote_1.  Otherwise error
             **/
            void on_quote_token(const Token & tk,
                                ParserStateMachine * p_psm);

            /** update ssm state for incoming leftparen token @p tk,
             *  with overall parser state in @p p_psm
             **/
            void on_leftbrace_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update ssm state for incoming rightparen token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_rightbrace_token(const Token & tk,
                                     ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-parenssm-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies this ssm **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state. **/
            std::string_view get_expect_str() const noexcept;

            /** update ssm for token @p tk, with overall state in @p p_psm **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** update ssm for expression @p expr (emitted by nested ssm),
             *  with overall parser state in @p p_psm
             **/
            void on_quoted_literal(obj<AGCObject> literal,
                                      ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-parenssm-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** @defgroup scm-parenssm-member-vars **/
            ///@{

            /** identify quote-expression state **/
            QuoteXst quote_xst_;
            /** scaffold expression (representing parenthesized value) here **/
            obj<AExpression> expr_;

            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DQuoteSsm.hpp */
