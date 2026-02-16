/** @file DParenSsm.hpp
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
         *   ( x * x )
         *  ^ ^     ^ ^
         *  | |     | (done)
         *  | |     |
         *  | |     lparen_2
         *  | lparen_1:expect expr
         *  lparen_0
         *
         *   lparen_0 --on_leftparen_token()--> lparen_1
         *   lparen_1 --on_parsed_expression()--> lparen_2
         *   lparen_2 --on_rightparen_token()--> (done)
         *
         * @endpre
         **/

        enum class parenexprstatetype {
            invalid = -1,

            lparen_0,
            lparen_1,
            lparen_2,

            N,
        };

        extern const char * parenexprstatetype_descr(parenexprstatetype x);

        std::ostream &
        operator<<(std::ostream & os, parenexprstatetype x);

        class DParenSsm : public DSyntaxStateMachine<DParenSsm> {
        public:
            using Super = DSyntaxStateMachine<DParenSsm>;
            using TypeDescr = xo::reflect::TypeDescr;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-parenssm-ctors **/
            ///@{

            DParenSsm();

            /** create instance using memory from @p parser_mm
             **/
            static DParenSsm * _make(DArena & parser_mm);

            /** create fop pointing with new DParenSsm using memory from @p parser_mm **/
            static obj<ASyntaxStateMachine,DParenSsm> make(DArena & parser_mm);

            /** push DParenSsm instance onto @p p_psm stack **/
            static void start(ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-parenssm-access-methods **/
            ///@{

            /** identify this nested state machine **/
            static const char * ssm_classname() { return "DParenSsm"; }

            /** internal state **/
            parenexprstatetype parenstate() const noexcept { return parenstate_; }

            ///@}
            /** @defgroup scm-parenssm-admin-methods admin methods **/
            ///@{

            /** update ssm state for incoming leftparen token @p tk,
             *  with overall parser state in @p p_psm
             **/
            void on_leftparen_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** update ssm state for incoming rightparen token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_rightparen_token(const Token & tk,
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
            void on_parsed_expression(obj<AExpression> expr,
                                      ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-parenssm-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** @defgroup scm-parenssm-member-vars **/
            ///@{

            /** identify paren-expression state **/
            parenexprstatetype parenstate_;
            /** scaffold expression (representing parenthesized value) here **/
            obj<AExpression> expr_;

            ///@}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DParenSsm.hpp */
