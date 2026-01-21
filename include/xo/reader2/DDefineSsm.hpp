/** @file DDefineSsm.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "ParserStateMachine.hpp"
//#include "SyntaxStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/expression2/detail/IExpression_DDefineExpr.hpp>
#include <xo/expression2/DDefineExpr.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        /**
         * @pre
         *
         *   def foo : f64 = 1 ;
         *  ^   ^   ^ ^   ^ ^ ^ ^
         *  |   |   | |   | | | (done)
         *  |   |   | |   | | def_6:expect_rhs_expression:expr_progress
         *  |   |   | |   | def_5:expect_rhs_expression
         *  |   |   | |   def_4
         *  |   |   | def_3:expect_type
         *  |   |   def_2
         *  |   def_1:expect_symbol
         *  def_0
         *  expect_toplevel_expression_sequence
         *
         *   def_0 --on_def_token()--> def_1
         *   def_1 --on_symbol()--> def_2
         *   def_2 --on_colon_token()--> def_3
         *         --on_singleassign_token()--> def_5
         *   def_3 --on_typedescr()--> def_4
         *   def_4 --on_singleassign_token()--> def_5
         *   def_5 --on_expr()--> def_6
         *   def_6 --on_semicolon_token()--> (done)
         *
         *   def_1:expect_symbol: got 'def' keyword, symbol to follow
         *   def_1: got symbol name
         *   def_3:expect_symbol got (optional) colon, type name to follow
         *   def_4: got symbol type
         *   def_6:expect_rhs_expression got (optional) equal sign, value to follow
         *   (done): definition complete,  pop exprstate from stack
         *
         * @endpre
         **/
        enum class defexprstatetype {
            invalid = -1,

            def_0,
            def_1,
            def_2,
            def_3,
            def_4,
            def_5,
            def_6,

            n_defexprstatetype,
        };

        extern const char * defexprstatetype_descr(defexprstatetype x);

        std::ostream &
        operator<<(std::ostream & os, defexprstatetype x);

        /** @class DDefineSsm
         *  @brief state machine for parsing a define expression
         **/
        class DDefineSsm {
        public:
            using TypeDescr = xo::reflect::TypeDescr;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-define-ssm-facet constructors **/
            ///@{

            /** constructor; using @p def_expr for initial expression scaffold **/
            explicit DDefineSsm(DDefineExpr * def_expr);

            /** create instance using memory from @p parser_mm.
             *  Initial expression scaffold @p def_expr
             **/
            static DDefineSsm * make(DArena & parser_mm,
                                     DDefineExpr * def_expr);

            /** start nested parser for a define-expression,
             *  on top of parser state machine @p p_psm
             *  Use @p parser_mm to allocate syntax state machines,
             *  and @p expr_mm to allocate expressions
             **/
            static void start(DArena & parser_mm,
                              obj<AAllocator> expr_mm,
                              ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-definessm-access-methods **/
            ///@{

            /** identify define-expression state **/
            defexprstatetype defstate() const noexcept { return defstate_; }

            ///@}
            /** @defgroup scm-define-ssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error mesages
             **/
            std::string_view get_expect_str() const noexcept;

            /** operate state machine for this syntax on incoming symbol-token @p tk
             *  with overall parser state in @p p_psm
             **/
            void on_symbol_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            /** update state for this syntax on incoming token @p tk,
             *  overall parser state in @p p_psm
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

            /** update state for this syntax after parsing a symbol @p sym;
             *  overall parser state in @p p_psm
             **/
            void on_parsed_symbol(std::string_view sym,
                                  ParserStateMachine * p_psm);

            /** update state for this syntax after parsing a type-description @p td,
             *  overall parser state in @p p_psm
             **/
            void on_parsed_typedescr(TypeDescr td,
                                     ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-define-printable-facet printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** identify define-expression state **/
            defexprstatetype defstate_;

            /** scaffolded define-expression.
             *  This will eventually be the output of this ssm
             **/
            obj<AExpression,DDefineExpr> def_expr_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DDefineSsm.hpp */
