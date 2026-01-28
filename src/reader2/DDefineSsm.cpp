 /** @file DDefineSsm.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DDefineSsm.hpp"
#include "DExpectSymbolSsm.hpp"
#include "DExpectTypeSsm.hpp"
#include "DExpectExprSsm.hpp"
#include "ssm/ISyntaxStateMachine_DDefineSsm.hpp"
#include "ssm/IPrintable_DDefineSsm.hpp"
#include <xo/expression2/detail/IPrintable_DDefineExpr.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>

#ifdef NOT_YET
#include "parserstatemachine.hpp"
#include "expect_symbol_xs.hpp"
#include "expect_expr_xs.hpp"
#include "expect_type_xs.hpp"
#include "pretty_expression.hpp"
#endif

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::facet::with_facet;
    using xo::facet::typeseq;

    namespace scm {
        // ----- defexprstatetype -----

        const char *
        defexprstatetype_descr(defexprstatetype x) {
            switch (x) {
            case defexprstatetype::invalid: return "invalid";
            case defexprstatetype::def_0: return "def_0";
            case defexprstatetype::def_1: return "def_1";
            case defexprstatetype::def_2: return "def_2";
            case defexprstatetype::def_3: return "def_3";
            case defexprstatetype::def_4: return "def_4";
            case defexprstatetype::def_5: return "def_5";
            case defexprstatetype::def_6: return "def_6";
            case defexprstatetype::n_defexprstatetype: break;
            }

            return "???defexprstatetype";
        }

        std::ostream &
        operator<<(std::ostream & os, defexprstatetype x) {
            os << defexprstatetype_descr(x);
            return os;
        }

        // ----- define_xs -----

        // DDefineSsm::make

        // DDefineSsm::start

#ifdef NOT_YET
        define_xs::define_xs(rp<DefineExprAccess> def_expr)
            : exprstate(exprstatetype::defexpr),
              defxs_type_{defexprstatetype::def_0},
              def_expr_{std::move(def_expr)}
        {}

        // const char *
        // define_xs::get_expect_str() const { ... }

        void
        define_xs::on_expr(bp<Expression> expr,
                           parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log(xtag("defxs_type", defxs_type_));

            if (this->defxs_type_ == defexprstatetype::def_5) {
                /* have all the ingredients to create an expression
                 * representing a definition
                 *
                 * 1. if ir_type is a symbol,  interpret as variable name.
                 *    Need to be able to locate variable by type
                 * 2. if ir_type is an expression,  adopt as rhs
                 */
                rp<Expression> rhs_value = expr.promote();

                if (this->cvt_expr_) {
                    this->cvt_expr_->assign_arg(rhs_value);
                } else {
                    /* note: establishes .def_expr_ valuetype */
                    this->def_expr_->assign_rhs(rhs_value);
                }

                rp<Expression> def_expr = this->def_expr_;

                this->defxs_type_ = defexprstatetype::def_6;
                return;
            }

            constexpr const char * c_self_name = "define_xs::on_expr";
            const char * exp = get_expect_str();

            this->illegal_input_on_expr(c_self_name, expr, exp, p_psm);
        }

        void
        define_xs::on_expr_with_semicolon(bp<Expression> expr,
                                          parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log(xtag("defxs_type", defxs_type_));

            this->on_expr(expr, p_psm);
            /* semicolon is allowed to terminate def expr */
            this->on_semicolon_token(token_type::semicolon(), p_psm);
        }

        void
        define_xs::on_symbol(const std::string & symbol_name,
                             parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log(xtag("defxs_type", defxs_type_), xtag("env_stack_size", p_psm->env_stack_size()));

            if (this->defxs_type_ == defexprstatetype::def_1) {
                this->defxs_type_ = defexprstatetype::def_2;
                this->def_expr_->assign_lhs_name(symbol_name);

                // if this is a genuine top-level define (i.e. nesting level = 0),
                // then we need to upsert so we can refer to rhs later.
                //
                // In other contexts (e.g. body-of-lambda) will be rewriting
                //    {
                //       def y = foo(x,x);
                //       bar(y,y);
                //    }
                // into something like
                //    {
                //       (lambda (y123) bar(y123,y123))(foo(x,x));
                //    }
                //
                // This works in the body of lambda, because we don't evaluate anything
                // until lambda definition is complete.
                //
                // For interactive top-level defs we want to evaluate as we go,
                // so need incremental bindings.

                if (p_psm->env_stack_size() == 1) {
                    /* remember variable binding in lexical context,
                     * so we can refer to it later
                     */
                    p_psm->upsert_var(this->def_expr_->lhs_variable());
                }

                return;
            }

            constexpr const char * c_self_name = "define_xs::on_symbol";
            const char * exp = this->get_expect_str();

            this->illegal_input_on_symbol(c_self_name, symbol_name, exp, p_psm);
        }

        void
        define_xs::on_typedescr(TypeDescr td,
                                parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log("defxs_type", defxs_type_);

            if (this->defxs_type_ == defexprstatetype::def_3) {
                this->defxs_type_ = defexprstatetype::def_4;
                this->cvt_expr_ = ConvertExprAccess::make(td /*dest_type*/,
                                                          nullptr /*source_expr*/);
                /* note: establishes .def_expr_ valuetype */
                this->def_expr_->assign_rhs(this->cvt_expr_);
                return;
            }

            constexpr const char * c_self_name = "define_xs::on_symbol";
            const char * exp = this->get_expect_str();

            this->illegal_input_on_type(c_self_name, td, exp, p_psm);
        }

        void
        define_xs::on_def_token(const token_type & tk,
                                parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            log && log("defxs_type", defxs_type_);

            if (this->defxs_type_ == defexprstatetype::def_0) {
                this->defxs_type_ = defexprstatetype::def_1;

                expect_symbol_xs::start(p_psm);
                return;
            }

            constexpr const char * c_self_name = "define_xs::on_def_token";
            const char * exp = this->get_expect_str();

            this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
        }

        void
        define_xs::on_colon_token(const token_type & tk,
                                  parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            log && log("defxs_type", defxs_type_);

            if (this->defxs_type_ == defexprstatetype::def_2) {
                this->defxs_type_ = defexprstatetype::def_3;

                expect_type_xs::start(p_psm);
                return;
            }

            constexpr const char * c_self_name = "define_xs::on_symbol";
            const char * exp = this->get_expect_str();

            this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
        }

        void
        define_xs::on_semicolon_token(const token_type & tk,
                                      parserstatemachine * p_psm)
        {
            /* def expr consumes semicolon */

            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log("defxs_type", defxs_type_);

            if (this->defxs_type_ == defexprstatetype::def_6) {
                rp<DefineExprAccess> def_expr = this->def_expr_;

                std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

                p_psm->top_exprstate().on_expr(def_expr, p_psm);
                return;
            }

            constexpr const char * c_self_name = "define_xs::on_symbol";
            const char * exp = this->get_expect_str();

            this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
        }

        void
        define_xs::on_singleassign_token(const token_type & tk,
                                         parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            log && log("defxs_type", defxs_type_);

            if ((this->defxs_type_ == defexprstatetype::def_2)
                || (this->defxs_type_ == defexprstatetype::def_4))
            {
                this->defxs_type_ = defexprstatetype::def_5;
                expect_expr_xs::start(p_psm);
                return;
            }

            constexpr const char * c_self_name = "define_xs::on_singleassign_token";
            const char * exp = get_expect_str();

            this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
        }

        void
        define_xs::on_rightparen_token(const token_type & tk,
                                       parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            constexpr const char * c_self_name = "define_xs::on_rightparen";
            const char * exp = get_expect_str();

            this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
        }

        void
        define_xs::on_i64_token(const token_type & tk,
                                parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            constexpr const char * c_self_name = "define_xs::on_i64";
            const char * exp = get_expect_str();

            this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
        }

        void
        define_xs::on_f64_token(const token_type & tk,
                                parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            constexpr const char * c_self_name = "define_xs::on_f64";
            const char * exp = get_expect_str();

            this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
        }

        void
        define_xs::print(std::ostream & os) const {
            os << "<define_xs"
               << xtag("defxs_type", defxs_type_);
            os << ">";
        }

        bool
        define_xs::pretty_print(const xo::print::ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii, "define_xs",
                                             refrtag("defxs_type", defxs_type_));
        }
#endif

        ////////////////////////////////////////////////////////////////

        DDefineSsm::DDefineSsm(DDefineExpr * def_expr)
          : defstate_{defexprstatetype::def_0},
            def_expr_{def_expr}
        {}

        DDefineSsm *
        DDefineSsm::make(DArena & mm,
                         DDefineExpr * def_expr)
        {
            void * mem = mm.alloc(typeseq::id<DDefineSsm>(),
                                  sizeof(DDefineSsm));

            return new (mem) DDefineSsm(def_expr);
        }

        void
        DDefineSsm::start(DArena & mm,
                          obj<AAllocator> expr_mm,
                          ParserStateMachine * p_psm)
        {
            //scope log(XO_DEBUG(p_psm->debug_flag()));

            assert(p_psm->stack());

            DDefineExpr * def_expr = DDefineExpr::make_empty(expr_mm);

            DDefineSsm * define_ssm = DDefineSsm::make(mm, def_expr);

            obj<ASyntaxStateMachine> ssm
                = with_facet<ASyntaxStateMachine>::mkobj(define_ssm);

            p_psm->push_ssm(ssm);

            // note: triggers poly dispatch
            p_psm->on_token(Token::def_token());
        }

        syntaxstatetype
        DDefineSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::defexpr;
        }

        std::string_view
        DDefineSsm::get_expect_str() const noexcept
        {
            /*
             *   def foo       = 1 ;
             *   def foo : f64 = 1 ;
             *  ^   ^   ^ ^   ^ ^ ^ ^
             *  |   |   | |   | | | (done)
             *  |   |   | |   | | def_6
             *  |   |   | |   | def_5:expect_rhs_expression
             *  |   |   | |   def_4
             *  |   |   | def_3:expect_type
             *  |   |   def_2
             *  |   def_1:expect_symbol
             *  expect_toplevel_expression_sequence
             *
             * note that we skip from def_2 -> def_5 if '=' instead of ':'
             */
            switch (this->defstate_) {
            case defexprstatetype::invalid:
            case defexprstatetype::def_0:
            case defexprstatetype::n_defexprstatetype:
                assert(false);  // impossible
                return "impossible!?";
            case defexprstatetype::def_1:
                return "symbol";
            case defexprstatetype::def_2:
                return "singleassign|colon";
            case defexprstatetype::def_4:
                return "singleassign";
            case defexprstatetype::def_3:
                return "type";
            case defexprstatetype::def_5:
                return "expression";
            case defexprstatetype::def_6:
                return "semicolon";
            }

            return "?expect";
        }

        void
        DDefineSsm::on_parsed_symbol(std::string_view sym_name,
                                     ParserStateMachine * p_psm)
        {
            if (defstate_ == defexprstatetype::def_1) {
                this->defstate_ = defexprstatetype::def_2;

                const DUniqueString * sym
                    = p_psm->intern_string(sym_name);

                def_expr_.data()->assign_lhs_name(sym);

                // if this is a genuine top-level define (i.e. nesting level = 0),
                // then we need to upsert so we can refer to rhs later.
                //
                // In other contexts (e.g. body-of-lambda) will be rewriting
                //    {
                //       def y = foo(x,x);
                //       bar(y,y);
                //    }
                // into something like
                //    {
                //       (lambda (y123) bar(y123,y123))(foo(x,x));
                //    }
                //
                // This works in the body of lambda, because we don't evaluate anything
                // until lambda definition is complete.
                //
                // For interactive top-level defs we want to evaluate as we go,
                // so need incremental bindings.

                if (p_psm->is_at_toplevel()) {
                    /** remember variable binding in current lexical context **/
                    p_psm->upsert_var(def_expr_.data()->lhs());
                }

                return;
            }

            p_psm->illegal_input_on_symbol("DDefineSsm::on_parsed_symbol",
                                           sym_name,
                                           this->get_expect_str());
        }

        void
        DDefineSsm::on_parsed_typedescr(TypeDescr td,
                                        ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(true), xtag("td", td));

            if (defstate_ == defexprstatetype::def_3) {
                this->defstate_ = defexprstatetype::def_4;

                // note: not present in x0-reader/ version
                def_expr_.assign_valuetype(td);

#ifdef NOT_YET
                this->cvt_expr_ = ConvertExpr::make(td, nullptr);
                def_expr_->assign_rhs(cvt_expr_);
#endif
                log && log("STUB: ConvertExpr not implemented, TypeDescr not captured");

                return;
            }

            p_psm->illegal_input_on_typedescr("DDefineSsm::on_parsed_typedescr",
                                              td,
                                              this->get_expect_str());
        }

        void
        DDefineSsm::on_token(const Token & tk,
                             ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {
            case tokentype::tk_symbol:
                this->on_symbol_token(tk, p_psm);
                return;

            case tokentype::tk_def:
                this->on_def_token(tk, p_psm);
                return;

            case tokentype::tk_if:
                this->on_if_token(tk, p_psm);
                return;

            case tokentype::tk_colon:
                this->on_colon_token(tk, p_psm);
                return;

            case tokentype::tk_singleassign:
                this->on_singleassign_token(tk, p_psm);
                return;

            case tokentype::tk_string:
                this->on_string_token(tk, p_psm);
                return;

            case tokentype::tk_f64:
                this->on_f64_token(tk, p_psm);
                return;

            case tokentype::tk_i64:
                this->on_i64_token(tk, p_psm);
                return;

            case tokentype::tk_bool:
                this->on_bool_token(tk, p_psm);
                return;

            case tokentype::tk_semicolon:
                this->on_semicolon_token(tk, p_psm);
                return;

            // all the not-yet handled cases
            case tokentype::tk_invalid:
            case tokentype::tk_leftparen:
            case tokentype::tk_rightparen:
            case tokentype::tk_leftbracket:
            case tokentype::tk_rightbracket:
            case tokentype::tk_leftbrace:
            case tokentype::tk_rightbrace:
            case tokentype::tk_leftangle:
            case tokentype::tk_rightangle:
            case tokentype::tk_lessequal:
            case tokentype::tk_greatequal:
            case tokentype::tk_dot:
            case tokentype::tk_comma:
            case tokentype::tk_doublecolon:
            case tokentype::tk_assign:
            case tokentype::tk_yields:
            case tokentype::tk_plus:
            case tokentype::tk_minus:
            case tokentype::tk_star:
            case tokentype::tk_slash:
            case tokentype::tk_cmpeq:
            case tokentype::tk_cmpne:
            case tokentype::tk_type:
            case tokentype::tk_lambda:
            case tokentype::tk_then:
            case tokentype::tk_else:
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                p_psm->illegal_input_on_token("DDefineSsm::on_token",
                                              tk,
                                              this->get_expect_str());
                return;
            }
        }

        void
        DDefineSsm::on_symbol_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            // note:
            // in state def_1, DDefineSsm learns symbol via .on_parsed_symbol().
            // symbol token arriving here means encountered symbol while
            // in some other, which can't happen for valid Schematika input

            p_psm->illegal_input_on_token("DDefineSssm::on_symbol_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_def_token(const Token & tk,
                                 ParserStateMachine * p_psm)
        {
            if (defstate_ == defexprstatetype::def_0) {
                this->defstate_ = defexprstatetype::def_1;

                DExpectSymbolSsm::start(p_psm->parser_alloc(), p_psm);
                return;
            }

            p_psm->illegal_input_on_token("DDefineSsm::on_define_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_if_token(const Token & tk,
                                ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DDefineSsm::on_if_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_colon_token(const Token & tk,
                                   ParserStateMachine * p_psm)
        {
            if (defstate_ == defexprstatetype::def_2) {
                this->defstate_ = defexprstatetype::def_3;

                DExpectTypeSsm::start(p_psm->parser_alloc(),
                                      p_psm);

                return;
            }

            p_psm->illegal_input_on_token("DDefineSsm::on_colon_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_singleassign_token(const Token & tk,
                                          ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("defstate", defstate_));

            if ((defstate_ == defexprstatetype::def_2)
                || (defstate_ == defexprstatetype::def_4))
            {
                this->defstate_ = defexprstatetype::def_5;

                DExpectExprSsm::start(p_psm->parser_alloc(),
                                      p_psm);
                return;
            }

            p_psm->illegal_input_on_token("DDefineSsm::on_singleassign_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_string_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DDefineSsm::on_string_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_f64_token(const Token & tk,
                                 ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DDefineSsm::on_f64_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_i64_token(const Token & tk,
                                 ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DDefineSsm::on_i64_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_bool_token(const Token & tk,
                                  ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DDefineSsm::on_bool_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_semicolon_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            if (defstate_ == defexprstatetype::def_6) {
                p_psm->pop_ssm();
                p_psm->on_parsed_expression_with_semicolon(def_expr_);
                return;
            }

            p_psm->illegal_input_on_token("DDefineSsm::on_semicolon_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DDefineSsm::on_parsed_expression(obj<AExpression> expr,
                                         ParserStateMachine * p_psm)
        {
            if (defstate_ == defexprstatetype::def_5)
            {
                this->defstate_ = defexprstatetype::def_6;

                def_expr_.data()->assign_rhs(expr);
                return;
            }

            p_psm->illegal_parsed_expression("DDefineSsm::on_parsed_expression",
                                             expr,
                                             this->get_expect_str());
        }

        void
        DDefineSsm::on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                        ParserStateMachine * p_psm)
        {
            this->on_parsed_expression(expr, p_psm);
            this->on_semicolon_token(Token::semicolon_token(), p_psm);
        }

        bool
        DDefineSsm::pretty(const ppindentinfo & ppii) const
        {
            auto expr
                = FacetRegistry::instance().variant<APrintable,
                                                    AExpression>(def_expr_);
            assert(expr.data());
            (void)expr;

            return ppii.pps()->pretty_struct(ppii,
                                             "DDefineSsm",
                                             refrtag("defstate", defstate_),
                                             refrtag("def_expr", expr));
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DDefineSsm.cpp */
