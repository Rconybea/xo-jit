/* file DExpectSymbolSsm.hpp
 *
 * author: Roland Conybeare, Aug 2024
 */

#pragma once

#include "ParserStateMachine.hpp"
//#include "SyntaxStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/indentlog/print/ppindentinfo.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        /** @class DExpectSymbolSsm
         *  @brief state machine to expect + capture a symbol
         *
         *  For example:
         *  -  lhs in a define-expression
         **/
        class DExpectSymbolSsm {
        public:
            using DArena = xo::mm::DArena;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            DExpectSymbolSsm();

            /** create instance using memory from @p parser_mm **/
            static DExpectSymbolSsm * make(DArena & parser_mm);

            /** start nested parser expecting a symbol,
             *  on top of parser state machine @p p_psm.
             *  On success will deliver symbol by invoking
             *    .on_symbol(sym, p_psm)
             *  to the state machine on top of the stack
             *  as of when this start() method invoked
             **/
            static void start(DArena & parser_mm,
                              ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  with overall parser state in @p p_psm
             **/
            static void on_symbol_token(const Token & tk,
                                        ParserStateMachine * p_psm);

            /** @defgroup scm-expectsymbol-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error mesages
             **/
            std::string_view get_expect_str() const noexcept;

            /** update state for this syntax after parsing a symbol @p sym;
             *  overall parser state in @p p_psm.
             *
             *  NOTE:
             *  might not be obvious that this is unreachable.
             *  DExpectSymbolSsm converts a symbol token,
             *  and delivers it to parent ssm using this entry point.
             *  This method would only be called if consecutive
             *  DExpectSymbolSsm instances on parser stack;
             *  which scenario never occurs in Schematika syntax
             **/
            void on_parsed_symbol(std::string_view sym,
                                  ParserStateMachine * p_psm);

            /** update state for this syntax after parsing a type-description @p td
             *  in nested state machine.
             *  (provided to satisfy ASyntaxStateMachine api. not reachable)
             **/
            void on_parsed_typedescr(TypeDescr td,
                                     ParserStateMachine * p_psm);

            /** update state for this syntax after parsing an expression @p expr
             *  in nested state machine.
             *  (provided to satisfy ASyntaxStateMachine api. not reachable)
             **/
            void on_parsed_expression(obj<AExpression> expr,
                                      ParserStateMachine * p_psm);

            /** update state for this syntax after parsing an expression @p expr
             *  followed by semicolon in nested state machine.
             *  (provided to satisfy ASyntaxStateMachine api. not reachable)
             **/
            void on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                     ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  overall parser state in @p p_psm.
             **/
            void on_def_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_if_token(const Token & tk,
                             ParserStateMachine * p_psm);

            /** update state for this syntax on incoming colon token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_colon_token(const Token & tk,
                                ParserStateMachine * p_psm);

            /** update state for this syntax on incoming singleassign token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_singleassign_token(const Token & tk,
                                       ParserStateMachine * p_psm);

            /** update state for this syntax on incoming f64 token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_f64_token(const Token & tk,
                              ParserStateMachine * p_psm);

            /** update state for this syntax on incoming semicolon token @p tk,
             *  overall parser state in @p p_psm
             **/
            void on_semicolon_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-expectsymbol-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectSymbolSsm.hpp */
