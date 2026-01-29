/** @file DProgressSsm.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DProgressSsm.hpp"
#include "ssm/ISyntaxStateMachine_DProgressSsm.hpp"

#include "DExpectExprSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectExprSsm.hpp"

#include <xo/expression2/DApplyExpr.hpp>
#include <xo/expression2/detail/IExpression_DApplyExpr.hpp>

#include <xo/expression2/DConstant.hpp>
#include <xo/expression2/detail/IExpression_DConstant.hpp>

#include <xo/procedure2/init_primitives.hpp>  // for xo::scm::Primitives
#include <xo/procedure2/detail/IGCObject_DPrimitive_gco_2_gco_gco.hpp>

#ifdef NOT_YET
#include "DApplySsm.hpp"
#include "ssm/ISyntaxStateMachine_DApplySsm.hpp"
#endif

#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/reflectutil/typeseq.hpp>
#include <xo/indentlog/print/cond.hpp>

#ifdef NOT_YET
#include "apply_xs.hpp"
#include "exprstatestack.hpp"
#include "expect_expr_xs.hpp"
#include "parserstatemachine.hpp"
#include "pretty_exprstatestack.hpp"
#include "xo/expression/AssignExpr.hpp"
#include "xo/expression/Apply.hpp"
#include "xo/expression/pretty_expression.hpp"
#endif

namespace xo {
#ifdef NOT_YET
    using xo::scm::Expression;
    using xo::scm::AssignExpr;
    using xo::scm::Variable;
    using xo::scm::Apply;
#endif
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::facet::with_facet;
    using xo::reflect::typeseq;

    namespace scm {
        const char *
        optype_descr(optype x) {
            switch (x) {
            case optype::invalid:
                return "?optype";
            case optype::op_assign:
                return "op:=";
            case optype::op_less:
                return "op<";
            case optype::op_less_equal:
                return "op<=";
            case optype::op_equal:
                return "op==";
            case optype::op_not_equal:
                return "op!=";
            case optype::op_great:
                return "op>";
            case optype::op_great_equal:
                return "op>=";
            case optype::op_add:
                return "op+";
            case optype::op_subtract:
                return "op-";
            case optype::op_multiply:
                return "op*";
            case optype::op_divide:
                return "op/";
            case optype::n_optype:
                break;
            }
            return "???";
        }

        /** higher-precedence operators bind before lower-preference operators **/
        int
        precedence(optype x) {
            switch (x) {
            case optype::invalid:
            case optype::n_optype:
                return 0;

            case optype::op_assign:
                return 1;

            case optype::op_less:
            case optype::op_less_equal:
            case optype::op_equal:
            case optype::op_not_equal:
            case optype::op_great:
            case optype::op_great_equal:
                return 2;

            case optype::op_add:
            case optype::op_subtract:
                return 3;

            case optype::op_multiply:
            case optype::op_divide:
                return 4;
            }

            return 0;
        }

        namespace {
            optype
            tk2op(const tokentype & tktype) {
                switch (tktype) {
                case tokentype::tk_assign:
                    return optype::op_assign;
                case tokentype::tk_plus:
                    return optype::op_add;
                case tokentype::tk_minus:
                    return optype::op_subtract;
                case tokentype::tk_star:
                    return optype::op_multiply;
                case tokentype::tk_slash:
                    return optype::op_divide;
                case tokentype::tk_cmpeq:
                    return optype::op_equal;
                case tokentype::tk_cmpne:
                    return optype::op_not_equal;
                case tokentype::tk_leftangle:
                    return optype::op_less;
                case tokentype::tk_lessequal:
                    return optype::op_less_equal;
                case tokentype::tk_rightangle:
                    return optype::op_great;
                case tokentype::tk_greatequal:
                    return optype::op_great_equal;
                default:
                    assert(false);
                    return optype::invalid;
                }
                return optype::invalid;
            }
        }

        DProgressSsm *
        DProgressSsm::make(DArena & mm,
                           obj<AExpression> lhs,
                           optype op)
        {
            void * mem = mm.alloc(typeseq::id<DProgressSsm>(),
                                  sizeof(DProgressSsm));

            return new (mem) DProgressSsm(lhs, op);

            //return std::make_unique<progress_xs>(progress_xs(std::move(valex), op));
        }

        void
        DProgressSsm::start(DArena & parser_mm,
                            obj<AExpression> lhs,
                            optype op,
                            ParserStateMachine * p_psm)
        {
            DProgressSsm * progress_ssm
                = DProgressSsm::make(parser_mm, lhs, op);

            obj<ASyntaxStateMachine,DProgressSsm> ssm(progress_ssm);

            p_psm->push_ssm(ssm);
        }

        void
        DProgressSsm::start(DArena & parser_mm,
                            obj<AExpression> lhs,
                            ParserStateMachine * p_psm)
        {
            start(parser_mm, lhs, optype::invalid, p_psm);
        }

        DProgressSsm::DProgressSsm(obj<AExpression> valex,
                                   optype op)
          : lhs_{valex},
            op_type_{op}
        {}

        syntaxstatetype
        DProgressSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::progress;
        }

#ifdef NOT_YET
        bool
        progress_xs::admits_f64() const { return false; }
#endif

        std::string_view
        DProgressSsm::get_expect_str() const noexcept {
            if (op_type_ == optype::invalid) {
                return "oper|semicolon|rightparen";
            } else {
                return "expr|leftparen";
            }
        }

        void
        DProgressSsm::on_token(const Token & tk,
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

            case tokentype::tk_then:
                this->on_then_token(tk, p_psm);
                return;

            case tokentype::tk_else:
                this->on_else_token(tk, p_psm);
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
                break;

            case tokentype::tk_star:
                this->on_operator_token(tk, p_psm);
                return;

            case tokentype::tk_slash:
            case tokentype::tk_cmpeq:
            case tokentype::tk_cmpne:
            case tokentype::tk_type:
            case tokentype::tk_lambda:
                break;
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                break;
            }

            p_psm->illegal_input_on_token("DProgressSsm::on_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_symbol_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DProgressSsm::on_symbol_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_def_token(const Token & tk,
                                   ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DProgressSsm::on_def_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_if_token(const Token & tk,
                                  ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DProgressSsm::on_if_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_then_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            (void)tk;

            obj<AExpression> expr = this->assemble_expr(p_psm);

            p_psm->pop_ssm();  // completes self

            // TODO: perhaps need to generalize on_parsed_expression_with_semicolon() ..?
            p_psm->on_parsed_expression(expr);
            p_psm->on_token(tk);
        }

        void
        DProgressSsm::on_else_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            // note: common with .on_then_token()

            scope log(XO_DEBUG(p_psm->debug_flag()));

            (void)tk;

            obj<AExpression> expr = this->assemble_expr(p_psm);

            p_psm->pop_ssm();  // completes self

            // TODO: perhaps need to generalize on_parsed_expression_with_semicolon() ..?
            p_psm->on_parsed_expression(expr);
            p_psm->on_token(tk);
        }

        void
        DProgressSsm::on_colon_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DProgressSsm::on_colon_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_singleassign_token(const Token & tk,
                                            ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DProgressSsm::on_singleassign_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_operator_token(const Token & tk,
                                        ParserStateMachine * p_psm)
        {
            if (op_type_ == optype::invalid) {
                // tk is the operator this instance was waiting for
                this->op_type_ = tk2op(tk.tk_type());

                DExpectExprSsm::start(p_psm->parser_alloc(), p_psm);
                return;
            } else if (rhs_) {
                optype op_type2 = tk2op(tk.tk_type());

                /* already have {lhs_, op_type_, rhs_},
                 * with incoming op_type2 operator decides whether to parse like
                 *    (lhs_, op_type_, rhs_) op_type2 ...
                 * or
                 *    (lhs_, op_type_, (rhs_ op_type2 ...))
                 */

                if (precedence(op_type_) >= precedence(op_type2)) {
                    /* associate to the left
                     *
                     * parse              like
                     *   a + b - ...        (a + b) - ...
                     *   a * b - ...        (a * b) - ...
                     */

                    auto lhs2 = this->assemble_expr(p_psm);

                    p_psm->pop_ssm();

                    DProgressSsm::start(p_psm->parser_alloc(),
                                        lhs2,
                                        op_type2,
                                        p_psm);
                    return;
                } else {
                    /* associate to the right
                     *
                     * parse             like
                     *  a + b * ...        (a + (b * ...))
                     */

                    p_psm->pop_ssm();

                    /* (a + ..) */
                    DProgressSsm::start(p_psm->parser_alloc(),
                                        lhs_,
                                        op_type_,
                                        p_psm);
                    DExpectExprSsm::start(p_psm->parser_alloc(),
                                          p_psm);
                    /* (b * ..) */
                    DProgressSsm::start(p_psm->parser_alloc(),
                                        rhs_,
                                        op_type2,
                                        p_psm);
                    DExpectExprSsm::start(p_psm->parser_alloc(),
                                          p_psm);
                    return;
                }
            }

            p_psm->illegal_input_on_token("DProgressSsm::on_operator_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_string_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DProgressSsm::on_string_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_f64_token(const Token & tk,
                                   ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DProgressSsm::on_f64_token",
                                         tk,
                                         this->get_expect_str());
        }

        void
        DProgressSsm::on_i64_token(const Token & tk,
                                   ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DProgressSsm::on_i64_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_bool_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DProgressSsm::on_bool_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_semicolon_token(const Token & tk,
                                         ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            /* note: implementation should parallel .on_rightparen_token() */

            (void)tk;

            obj<AExpression> expr = this->assemble_expr(p_psm);

            {
                obj<APrintable> expr_pr = FacetRegistry::instance().variant<APrintable,AExpression>(expr);
                assert(expr_pr);
                log && log(xtag("expr", expr_pr));
            }

            p_psm->pop_ssm();
            p_psm->on_parsed_expression_with_semicolon(expr);

            /* control here on input like:
             *   (1.234;
             *
             * a. '(' sets up stack [lparen_0:expect_rhs_expression]
             *     (see exprstate::on_leftparen())
             * b. 1.234 pushes (in case operators) [lparen_0:expect_rhs_expression:expr_progress]
             *     (see exprstate::on_f64())
             * c. semicolon completes expr_progress [lparen_0:expect_rhs_expression]
             *     deliver expresssion to expect_rhs_expression.on_expr_with_semicolon()
             *     (see exprstate::on_expr_with_semicolon())
             * d. expr_rhs_expression forwards expression to [lparen_0]
             * e. lparen_0 would advance to [lparen_1],  but rejects semicolon
             */

#ifdef OBSOLETE
            p_psm->illegal_input_on_token("DProgressSsm::on_semicolon_token",
                                          tk,
                                          this->get_expect_str());
#endif
        }

        void
        DProgressSsm::on_parsed_symbol(std::string_view sym,
                                       ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("DProgressSsm::on_parsed_symbol",
                                          sym,
                                          this->get_expect_str());
        }

        void
        DProgressSsm::on_parsed_typedescr(TypeDescr td,
                                          ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_typedescr("DProgressSsm::on_parsed_typedescr",
                                              td,
                                              this->get_expect_str());
        }

        void
        DProgressSsm::on_parsed_formal(const DUniqueString * param_name,
                                       TypeDescr param_type,
                                       ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_formal("DProgressSsm::on_parsed_formal",
                                         param_name,
                                         param_type,
                                         this->get_expect_str());
        }

        void
        DProgressSsm::on_parsed_expression(obj<AExpression> expr,
                                           ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_expression("DProgressSsm::on_parsed_expression",
                                             expr,
                                             this->get_expect_str());
        }

        void
        DProgressSsm::on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                          ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()),
                      xtag("expr", expr));

            if (op_type_ == optype::invalid) {
                p_psm->illegal_parsed_expression
                    ("DProgressSsm::on_parsed_expression_with_semicolon",
                     expr,
                     this->get_expect_str());
                return;
            }

            this->rhs_ = expr;

            obj<AExpression> expr2 = this->assemble_expr(p_psm);

            if (expr2) {
                p_psm->pop_ssm();
                p_psm->on_parsed_expression_with_semicolon(expr2);
            }
        }

#ifdef NOT_YET
        void
        progress_xs::apply_type_error(const char * self_name,
                                      optype op,
                                      bp<Expression> expr1,
                                      bp<Expression> expr2,
                                      parserstatemachine * p_psm) const
        {
            std::string errmsg = tostr("incompatible argument types T1,T2 to op",
                                       xtag("op", op),
                                       xtag("T1", expr1->valuetype()),
                                       xtag("T2", expr2->valuetype()));

            p_psm->on_error(self_name, std::move(errmsg));
        }

        rp<Expression>
        progress_xs::assemble_expr(parserstatemachine * p_psm) {
            /* need to defer building Apply incase expr followed by higher-precedence operator:
             * consider input like
             *   3.14 + 2.0 * ...
             */

            constexpr const char * c_self_name = "progress_xs::assemble_expr";

            if ((op_type_ != optype::invalid) && (rhs_.get() == nullptr)) {
                std::string errmsg = tostr("expected expression on rhs of operator op",
                                           xtag("lhs", lhs_),
                                           xtag("op", op_type_));

                p_psm->on_error(c_self_name, errmsg);
            }

            /* consecutive expressions not legal, e.g:
             *   3.14 6.28
             * but expressions surrounding an infix operators is:
             *   3.14 / 6.28
             */
            switch (op_type_) {
            case optype::invalid:
                return this->lhs_;

            case optype::op_assign:
            {
                bp<Variable> lhs = Variable::from(this->lhs_);

                if (!lhs) {
                    throw std::runtime_error
                        (tostr("progress_xs::assemble_expr",
                               " expect variable on lhs of assignment operator :=",
                               xtag("lhs", lhs_),
                               xtag("rhs", rhs_)));
                }

                return AssignExpr::make(lhs.promote(),
                                        this->rhs_);
            }

            case optype::op_equal:
                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_cmp_eq_i64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }
                break;

            case optype::op_not_equal:
                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_cmp_ne_i64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }
                break;

            case optype::op_less:
                // TODO: floating-point less-than

                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_cmp_lt_i64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }
                break;

            case optype::op_less_equal:
                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_cmp_le_i64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }
                break;

            case optype::op_great:
                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_cmp_gt_i64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }
                break;

            case optype::op_great_equal:
                // TODO: upconvert integer->double
                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_cmp_ge_i64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }

                assert(false);

            case optype::op_add:
                // TODO: upconvert integer->double
                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_add2_i64(lhs_, rhs_);
                } else if (lhs_->valuetype()->is_f64() && rhs_->valuetype()->is_f64()) {
                    return Apply::make_add2_f64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }
                break;
            case optype::op_subtract:
                // TODO: upconvert integer->double
                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_sub2_i64(lhs_, rhs_);
                } else if (lhs_->valuetype()->is_f64() && rhs_->valuetype()->is_f64()) {
                    return Apply::make_sub2_f64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }
                break;

            case optype::op_multiply:
                // TODO: upconvert integer->double
                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_mul2_i64(lhs_, rhs_);
                } else if (lhs_->valuetype()->is_f64() && rhs_->valuetype()->is_f64()) {
                    return Apply::make_mul2_f64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }

                break;

            case optype::op_divide:
                // TODO: upconvert integer->double
                if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
                    return Apply::make_div2_i64(lhs_, rhs_);
                } else if (lhs_->valuetype()->is_f64() && rhs_->valuetype()->is_f64()) {
                    return Apply::make_div2_f64(lhs_, rhs_);
                } else {
                    this->apply_type_error(c_self_name,
                                           op_type_, lhs_, rhs_, p_psm);
                    return nullptr;
                }
                break;

            case optype::n_optype:
                /* unreachable */
                assert(false);
                return nullptr;
            }

            return nullptr;
        }

        void
        progress_xs::on_expr(bp<Expression> expr,
                             parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("expr", expr));

            /* note: previous token probably an operator,
             *       handled from progress_xs::on_operator_token(),
             *       which pushes expect_expr_xs::expect_rhs_expression()
             */

            constexpr const char * c_self_name = "progress_xs::on_expr";
            const char * exp = get_expect_str();

            if (lhs_) {
                if (op_type_ == optype::invalid) {
                    /* two consecutive expression without an operator */
                    this->illegal_input_on_expr(c_self_name, expr, exp, p_psm);
                }

#ifdef NOT_QUITE
                assert(result.get());

                /* this expression complete.. */
                std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

                /* ..but more operators could follow, so don't commit yet */
                p_stack->push_exprstate(progress_xs::make(result));
#endif

                this->rhs_ = expr.promote();
            } else {
                /* control here on input like
                 *   add(1,2)...
                 *
                 * add(1,2) needs to be handled inside a progress_xs
                 * instance because may be followed by an operator:
                 *   add(1,2) + ...
                 */
                this->lhs_ = expr.promote();
            }
        }

        void
        progress_xs::on_expr_with_semicolon(bp<Expression> expr,
                                            parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log(xtag("lhs", lhs_), xtag("op", op_type_), xtag("expr", expr));

            constexpr const char * c_self_name = "progress_xs::on_expr_with_semicolon";
            const char * exp = get_expect_str();

            if (op_type_ == optype::invalid) {
                this->illegal_input_on_expr(c_self_name, expr, exp, p_psm);
            }

            this->rhs_ = expr.promote();

            // FORBIDDEN, because .on_semicolon_token() destroys *this before returning
            //   this->on_semicolon_token(token_type::semicolon(), p_psm);
            // INSTEAD, spell out the body

            rp<Expression> expr2 = this->assemble_expr(p_psm);

            if (expr2) {
                std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

                p_psm->on_expr_with_semicolon(expr2);
            }
        }
#endif

#ifdef NOT_YET
        void
        progress_xs::on_comma_token(const token_type & tk,
                                    parserstatemachine * p_psm)
        {
            /* note: implementation parllels .on_semicolon_token(), .on_rightparen_token() */

            scope log(XO_DEBUG(p_psm->debug_flag()));

            constexpr const char * self_name = "progress::xs::on_comma_token";

            auto & xs_stack = p_psm->xs_stack_;

            /* stack may be something like
             *
             *   applyexpr
             *   expect_expr_xs
             *   progress_xs
             *                  <-- comma
             *
             * 1. comma completes expression-in-progress
             */

            /* comma confirms stack expression */
            rp<Expression> expr = this->assemble_expr(p_psm);

            std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

            if (xs_stack.empty()) {
                throw std::runtime_error(tostr(self_name,
                                               ": expected non-empty parsing state"));
            }

            log && log(xtag("stack", &xs_stack));

            p_psm->top_exprstate().on_expr(expr, p_psm);

            /* now deliver comma */
            p_psm->top_exprstate().on_comma_token(tk, p_psm);
        }

        void
        progress_xs::on_typedescr(TypeDescr /*td*/,
                                  parserstatemachine * /*p_psm*/)
        {
            /* unreachable */
            assert(false);
        }

        void
        progress_xs::on_semicolon_token(const token_type & /*tk*/,
                                        parserstatemachine * p_psm)
        {
            /* note: implementation parallels .on_rightparen_token() */

            scope log(XO_DEBUG(p_psm->debug_flag()));

            rp<Expression> expr = this->assemble_expr(p_psm);

            log && log(xtag("assembled-expr", expr));

            std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

            p_psm->on_expr_with_semicolon(expr);

            /* control here on input like:
             *   (1.234;
             *
             * a. '(' sets up stack [lparen_0:expect_rhs_expression]
             *     (see exprstate::on_leftparen())
             * b. 1.234 pushes (in case operators) [lparen_0:expect_rhs_expression:expr_progress]
             *     (see exprstate::on_f64())
             * c. semicolon completes expr_progress [lparen_0:expect_rhs_expression]
             *     deliver expresssion to expect_rhs_expression.on_expr_with_semicolon()
             *     (see exprstate::on_expr_with_semicolon())
             * d. expr_rhs_expression forwards expression to [lparen_0]
             * e. lparen_0 would advance to [lparen_1],  but rejects semicolon
             */
        }

        void
        progress_xs::on_assign_token(const token_type & tk,
                                     parserstatemachine * p_psm)
        {
            this->on_operator_token(tk, p_psm);
        }

        /* editor bait: on_lparen */
        void
        progress_xs::on_leftparen_token(const token_type & tk,
                                        parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            /* input like:
             *   'foo(' -> expect function call.  might continue 'foo(a,b,c)'
             *   'foo+(' -> expect parenthesized expression.  might continue 'foo+(bar/2)'
             */

            if (op_type_ == optype::invalid) {
                /* start function call */
                assert(rhs_.get() == nullptr);

                rp<Expression> fn_expr = lhs_;

                /* reset this progress_xs back to empty state;
                 * apply_xs will be responsible for lhs_.
                 */
                lhs_ = nullptr;

#ifdef OBSOLETE
                /* don't unwind! want to handle input like
                 *   f(x,y)+g(z)
                 */
                /* unwind this progress_xs + replace with function call */
                std::unique_ptr<exprstate> self = p_psm->pop_exprstate();
#endif

                apply_xs::start(fn_expr, p_psm);

                /* control will reenter progress_xs via .on_expr() */
                return;
            }

            constexpr const char * c_self_name = "exprstate::on_leftparen";
            const char * exp = get_expect_str();

            this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
        }

         void
         progress_xs::on_rightparen_token(const token_type & tk,
                                          parserstatemachine * p_psm)
         {
             /* note: implementation parallels .on_semicolon_token() */

             scope log(XO_DEBUG(p_psm->debug_flag()));

             constexpr const char * self_name = "progress_xs::on_rightparen";

             auto & xs_stack = p_psm->xs_stack_;

             /* stack may be something like:
              *
              *   lparen_0
              *   expect_expr_xs
              *   expr_progress
              *                   <-- rightparen
              *
              * 1. rightparen completes expression-in-progress
              * 2. rightparen must then match innermost waiting lparen_0
              */

             /* right paren confirms stack expression */
             rp<Expression> expr = this->assemble_expr(p_psm);

             log && log(xtag("expr", expr),
                        xtag("do", "pop self + send {expr, rparen} -> parent"));

             std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

             if (xs_stack.empty()) {
                 throw std::runtime_error(tostr(self_name,
                                                ": expected non-empty parsing stack"));
             }

             log && log(xtag("stack", &xs_stack));

             p_psm->top_exprstate().on_expr(expr, p_psm);

             /* now deliver rightparen */
             p_psm->top_exprstate().on_rightparen_token(tk, p_psm);
         }

        void
        progress_xs::on_else_token(const token_type & tk,
                                   parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            rp<Expression> expr = this->assemble_expr(p_psm);

            log && log(xtag("assembled-expr", expr));

            std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

            p_psm->on_expr(expr);
            p_psm->on_else_token(tk);

            /* control here on input like:
             *
             *   if a > b then c else..
             */
        }

        void
        progress_xs::on_rightbrace_token(const token_type & tk,
                                         parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            rp<Expression> expr = this->assemble_expr(p_psm);

            log && log(xtag("assembled-expr", expr));

            std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

            p_psm->on_expr(expr);
            p_psm->on_rightbrace_token(tk);

            /* control here on input like:
             *
             *   { n * n }
             */
        }
#endif

#ifdef OBSOLETE
        //void progress_xs::on_operator_token(const token_type & tk, parserstatemachine * p_psm)

        void
        progress_xs::on_bool_token(const token_type & tk,
                                   parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            constexpr const char * c_self_name = "progress_xs::on_bool_token";
            const char * exp = get_expect_str();

            if (this->op_type_ == optype::invalid) {
                this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
            } else {
                exprstate::on_bool_token(tk, p_psm);
            }
        }

        void
        progress_xs::on_i64_token(const token_type & tk,
                                  parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            constexpr const char * c_self_name = "progress_xs::on_i64_token";
            const char * exp = get_expect_str();

            if (this->op_type_ == optype::invalid) {
                this->illegal_input_on_token(c_self_name, tk, exp, p_psm);
            } else {
                exprstate::on_i64_token(tk, p_psm);
            }
        }

        void
        progress_xs::print(std::ostream & os) const {
            os << "<progress_xs"
               << xtag("this", (void*)this)
               << xtag("type", exs_type_);
            if (lhs_)
                os << xtag("lhs", lhs_);
            if (op_type_ != optype::invalid)
                os << xtag("op", op_type_);
            if (rhs_)
                os << xtag("rhs", rhs_);
            os << ">";
        }

        bool
        progress_xs::pretty_print(const xo::print::ppindentinfo & ppii) const
        {
            if (ppii.upto()) {
                return (ppii.pps()->print_upto("<progress_xs")
                        && (lhs_ ? ppii.pps()->print_upto(refrtag("lhs", lhs_)) : true)
                        && (op_type_ != optype::invalid ? ppii.pps()->print_upto(refrtag("op", op_type_)) : true)
                        && (rhs_ ? ppii.pps()->print_upto(refrtag("rhs", rhs_)) : true)
                        && ppii.pps()->print_upto(">"));
            } else {
                ppii.pps()->write("<progress_xs ");
                if (lhs_)
                    ppii.pps()->pretty(refrtag("lhs", lhs_));
                if (op_type_ != optype::invalid)
                    ppii.pps()->pretty(refrtag("op", op_type_));
                if (rhs_)
                    ppii.pps()->pretty(refrtag("rhs", rhs_));
                ppii.pps()->write(">");
                return false;
            }
        }

#endif
        bool
        DProgressSsm::pretty(const xo::print::ppindentinfo & ppii) const
        {
            scope log(XO_DEBUG(false));
            log && log(xtag("lhs_.tseq", lhs_._typeseq()));
            log && log(xtag("rhs_.tseq", rhs_._typeseq()));

            obj<APrintable> lhs
                = FacetRegistry::instance().variant<APrintable,AExpression>(lhs_);

            obj<APrintable> rhs;
            if (rhs_)
                rhs = FacetRegistry::instance().variant<APrintable,AExpression>(rhs_);

            (void)lhs;

            return ppii.pps()->pretty_struct
                       (ppii,
                        "DProgressSsm",
                        refrtag("lhs", lhs),
                        refrtag("op", op_type_),
                        cond(rhs, refrtag("rhs", rhs), "nullptr")
                           );

        }

        obj<AExpression>
        DProgressSsm::assemble_expr(ParserStateMachine * p_psm)
        {
            /* need to defer building Apply incase expr followed by higher-precedence operator:
             * consider input like
             *   3.14 + 2.0 * ...
             */

            constexpr const char * c_self_name = "DProgressSsm::assemble_expr";

            if ((op_type_ != optype::invalid) && rhs_) {
                std::string errmsg_string = tostr("expected expression on rhs of operator op",
                                                  xtag("lhs", lhs_),
                                                  xtag("op", op_type_));

                auto errmsg = DString::from_view(p_psm->expr_alloc(),
                                                 std::string_view(errmsg_string));

                p_psm->capture_error(c_self_name, errmsg);
            }

            /* consecutive expressions not legal, e.g:
             *   3.14 6.28
             * but expressions surrounding an infix operators is:
             *   3.14 / 6.28
             */
            switch (op_type_) {
            case optype::invalid:
                return this->lhs_;

            case optype::op_assign:
            case optype::op_equal:
            case optype::op_not_equal:
            case optype::op_less:
            case optype::op_less_equal:
            case optype::op_great:
            case optype::op_great_equal:
            case optype::op_add:
            case optype::op_subtract:
                break;

            case optype::op_multiply:
                {
                    auto pm_obj  = (with_facet<AGCObject>::mkobj
                                    (&Primitives::s_mul_gco_gco_pm));
                    auto fn_expr = (DConstant::make
                                    (p_psm->expr_alloc(), pm_obj));

                    /* note:
                     * 1. don't assume we know lhs_ / rhs_ value types yet.
                     *    perhaps have expression like
                     *      f(..) * g(..)
                     *    where f is the function that contains current ssm.
                     *
                     * 2. consequence: we need representation for
                     *    polymorphic multiply on unknown numeric arguments.
                     *
                     * 3. TypeRef::dwim(..) is a placeholder.
                     *    Plan to later provide abstract interpreter
                     *    (ie compiler pass :) to drive type inference/unification
                     *
                     * 4. Alternatively could supply type-annotation syntax
                     *    so human can assist inference; context here is we want
                     *    to automate the boring stuff
                     */

                    TypeRef tref = TypeRef::dwim
                                       (TypeRef::prefix_type::from_chars("_mul_gco"),
                                        nullptr);

                    return DApplyExpr::make2(p_psm->expr_alloc(),
                                             tref, fn_expr, lhs_, rhs_);
                }

                break;
            case optype::op_divide:
                // TODO: implement binary operator expression assembly
                break;

#ifdef NOT_YET
case optype::op_assign:
    {
        bp<Variable> lhs = Variable::from(this->lhs_);

        if (!lhs) {
            throw std::runtime_error
                      (tostr("progress_xs::assemble_expr",
                             " expect variable on lhs of assignment operator :=",
                             xtag("lhs", lhs_),
                             xtag("rhs", rhs_)));
        }

        return AssignExpr::make(lhs.promote(),
                                this->rhs_);
    }

case optype::op_equal:
    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_cmp_eq_i64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }
    break;

case optype::op_not_equal:
    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_cmp_ne_i64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }
    break;

case optype::op_less:
    // TODO: floating-point less-than

    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_cmp_lt_i64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }
    break;

case optype::op_less_equal:
    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_cmp_le_i64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }
    break;

case optype::op_great:
    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_cmp_gt_i64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }
    break;

case optype::op_great_equal:
    // TODO: upconvert integer->double
    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_cmp_ge_i64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }

    assert(false);

case optype::op_add:
    // TODO: upconvert integer->double
    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_add2_i64(lhs_, rhs_);
    } else if (lhs_->valuetype()->is_f64() && rhs_->valuetype()->is_f64()) {
        return Apply::make_add2_f64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }
    break;
case optype::op_subtract:
    // TODO: upconvert integer->double
    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_sub2_i64(lhs_, rhs_);
    } else if (lhs_->valuetype()->is_f64() && rhs_->valuetype()->is_f64()) {
        return Apply::make_sub2_f64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }
    break;

case optype::op_multiply:
    // TODO: upconvert integer->double
    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_mul2_i64(lhs_, rhs_);
    } else if (lhs_->valuetype()->is_f64() && rhs_->valuetype()->is_f64()) {
        return Apply::make_mul2_f64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }

    break;

case optype::op_divide:
    // TODO: upconvert integer->double
    if (lhs_->valuetype()->is_i64() && rhs_->valuetype()->is_i64()) {
        return Apply::make_div2_i64(lhs_, rhs_);
    } else if (lhs_->valuetype()->is_f64() && rhs_->valuetype()->is_f64()) {
        return Apply::make_div2_f64(lhs_, rhs_);
    } else {
        this->apply_type_error(c_self_name,
                               op_type_, lhs_, rhs_, p_psm);
        return nullptr;
    }
    break;
#endif

            case optype::n_optype:
                /* unreachable */
                assert(false);
                break;
            }

            return obj<AExpression>();
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DProgressSsm.cpp */
