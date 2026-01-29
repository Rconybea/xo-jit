/** @file DLambdaSsm.hpp
 *
 *  Author: Roland Conybeare
 **/

#pragma once

#include "SyntaxStateMachine.hpp"
#include <xo/expression2/DLambdaExpr.hpp>
#include <xo/expression2/SymbolTable.hpp>
//#include <xo/expression2/DLocalSymtab.hpp>

namespace xo {
    namespace scm {
        class ParserStateMachine;

        /**
         * @text
         *
         *   lambda ( name(1) : type(1), ..., ) : type body-expr ;
         *  ^      ^                           ^ ^    ^         ^
         *  |      |                           | |    lm_4      lm_5
         *  |      |                           | lm_3
         *  lm_0   lm_1                        lm_2
         *
         *  lm_0 --on_lambda_token()--> lm_1
         *  lm_1 --on_formal_arglist()--> lm_2
         *  lm_2 --on_expr()--> lm_3
         *  lm_5 --on_semicolon_token()--> (done)
         *
         * @endtext
         **/
        enum class lambdastatetype {
            invalid = -1,

            lm_0,
            lm_1,
            lm_2,
            lm_3,
            lm_4,
            lm_5,

            n_lambdastatetype
        };

        extern const char *
        lambdastatetype_descr(lambdastatetype x);

        inline std::ostream &
        operator<< (std::ostream & os, lambdastatetype x) {
            os << lambdastatetype_descr(x);
            return os;
        }

        /** @class DLambdaSsm
         *  @brief parsing state-machine for a lambda-expression
         **/
        class DLambdaSsm {
        public:
            //using DSymbolTable = xo::scm::DSymbolTable;
            using DLocalSymtab = xo::scm::DLocalSymtab;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            /** @defgroup scm-lambdassm-ctors **/
            ///@{

            DLambdaSsm();

            /** create instance using memory from @p parser_mm **/
            static obj<ASyntaxStateMachine,DLambdaSsm> make(DArena & parser_mm);

            /** create instance using memory from @p parser_mm **/
            static DLambdaSsm * _make(DArena & parser_mm);

            ///@}
            /** @defgroup scm-lambdassm-methods **/
            ///@{

            static void start(ParserStateMachine * p_psm);

            /** update ssm on lambda keyword token @p tk,
             *  with overall parser state in @p p_psm
             **/
            void on_lambda_token(const Token & tk,
                                 ParserStateMachine * p_psm);

            ///@}
            /** @defgroup scm-lambdassm-syntaxstatemachine-facet **/
            ///@{

            /** identify this nested state machine **/
            syntaxstatetype ssm_type() const noexcept;

            /** text describing expected/allowed input to this ssm in current state.
             *  Intended to drive error messages
             **/
            std::string_view get_expect_str() const noexcept;

            /** update this ssm for incoming token @p tk **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            /** update this ssm when nested parser
             *  emits @p td.
             **/
            void on_parsed_symbol(std::string_view sym,
                                  ParserStateMachine * p_psm);

            /** update this ssm when nested parser
             *  emits @p td.
             **/
            void on_parsed_typedescr(TypeDescr td,
                                     ParserStateMachine * p_psm);

            /** update this ssm to consume parsed formal (name,value)
             *  from nested (and now expired) ssm
             **/
            void on_parsed_formal(const DUniqueString * sym,
                                  TypeDescr td,
                                  ParserStateMachine * p_psm);

            /** update this ssm when nested parser
             *  emits expression @p expr
             **/
            void on_parsed_expression(obj<AExpression> expr,
                                      ParserStateMachine * p_psm);

            /** update this ssm when nested parser
             *  emits expression @p expr
             **/
            void on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                     ParserStateMachine * p_psm);

#ifdef NOT_YET
            virtual const char * get_expect_str() const override;

            virtual void on_formal_arglist(const std::vector<rp<Variable>> & argl,
                                           parserstatemachine * p_psm) override;
            virtual void on_leftbrace_token(const token_type & tk,
                                            parserstatemachine * p_psm) override;
            virtual void on_colon_token(const token_type & tk,
                                        parserstatemachine * p_psm) override;
            virtual void on_semicolon_token(const token_type & tk,
                                            parserstatemachine * p_psm) override;
            virtual void on_f64_token(const token_type & tk,
                                      parserstatemachine * p_psm) final override;

            virtual void print(std::ostream & os) const override;
            virtual bool pretty_print(const print::ppindentinfo & ppii) const override;
#endif

            ///@}
            /** @defgroup scm-lambdassm-printable-facet **/
            ///@{

            /** pretty-printing support **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}


        private:
            /** parsing state-machine state **/
            lambdastatetype lmstate_ = lambdastatetype::lm_0;

#ifdef NOT_YET
            /** lambda environment (for formal parameters) **/
            DLocalSymtab * local_symtab_ = nullptr;

            /** explicit return type (if supplied) **/
            TypeDescr explicit_return_td_ = nullptr;

            /** lambda signature (when known) **/
            TypeDescr lambda_td_ = nullptr;
#endif

            /** body expression **/
            obj<AExpression> body_;

            /** parent environment **/
            obj<ASymbolTable> parent_symtab_;

        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DLambdaSsm.hpp */
