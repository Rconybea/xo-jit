/** @file expect_type_xs.hpp
 *
 * @author Roland Conybeare, Aug 2024
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include <xo/indentlog/print/ppindentinfo.hpp>

namespace xo {
    namespace scm {
        /** @class DExpectTypeSsm
         *  @brief syntax state-machine for accepting a Schemtika typename-expression
         *
         *  Jan 2026
         *  Placeholder implementation at present.
         *  Only types are a handful of baked-in values
         *
         *  @pre
         *
         *  @endpre
         **/
        class DExpectTypeSsm {
        public:
            using TypeDescr = xo::reflect::TypeDescr;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            DExpectTypeSsm();

            static DExpectTypeSsm * make(DArena & parser_mm);

            static void start(ParserStateMachine * p_psm);

            /** @defgroup scm-expecttype-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error messages.
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** operate state machien for this syntax on incoming symbol-token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_symbol_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            /** operate state machine for this syntax on incoming define-token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_def_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** operate state machine for this syntax on incoming if-token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_if_token(const Token & tk,
                             ParserStateMachine * p_psm);

            /** operate state machine for this syntax on incoming colon-token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_colon_token(const Token & tk,
                                ParserStateMachine * p_psm);

            /** operate state machine for this syntax on incoming singleassign-token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_singleassign_token(const Token & tk,
                                       ParserStateMachine * p_psm);

            /** update state for this syntax on incoming string token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_string_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            /** update state for this syntax on incoming f64 token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_f64_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for this syntax on incoming i64 token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_i64_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for this syntax on incoming bool token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_bool_token(const Token & tk,
                               ParserStateMachine * p_psm);

            /** update state for this syntax on incoming semicolon token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_semicolon_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            /** (Never called).
             *  Operate state machine for this syntax after symbol
             *  emitted from nested ssm.
             *  Impossible path for DExpectTypeSsm until such time as it relies
             *  on nested ssms.  Currently using on_symbol_token
             *  entry point instead.
             **/
            void on_parsed_symbol(std::string_view sym,
                                  ParserStateMachine * p_psm);

            /** operate state machine for this syntax on receiving type-description
             *  from nested parser.
             *  Currently (jan 2026) impossible path for DExpectTypeSsm.
             *  Active path is via on_symbol_token()
             **/
            void on_parsed_typedescr(TypeDescr td,
                                     ParserStateMachine * p_psm);

            /** operate state machine for this syntax on receiving expression
             *  from nested parser.
             *  (provided to satisfy ASyntaxStateMachine api. not reachable)
             **/
            void on_parsed_expression(obj<AExpression> expr,
                                      ParserStateMachine * p_psm);

            /** operate state machine for this syntax on receiving expression
             *  followed by semicolon from nested parser.
             *  (provided to satisfy ASyntaxStateMachine api. not reachable)
             **/
            void on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                     ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expecttype-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        };
    } /*namespace scm*/
} /*namespace xo*/


/* end DExpectTypeSsm.hpp */
