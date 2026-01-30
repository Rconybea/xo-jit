/** @file DIfElseSsm.cpp
 *
 *  @author Roland Conybeare, Jul 2025
 **/

#include "DIfElseSsm.hpp"
#include "ssm/ISyntaxStateMachine_DIfElseSsm.hpp"
#include "ssm/IPrintable_DDefineSsm.hpp"
#include "DExpectExprSsm.hpp"
#include <xo/expression2/detail/IPrintable_DIfElseExpr.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
//#include "exprstatestack.hpp"
//#include "parserstatemachine.hpp"
//#include "expect_expr_xs.hpp"
//#include "xo/indentlog/print/ppdetail_atomic.hpp"

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::facet::with_facet;
    using xo::reflect::typeseq;

    namespace scm {
        // ----- ifexprstatetype -----

        const char *
        ifexprstatetype_descr(ifexprstatetype x) {
            switch (x) {
            case ifexprstatetype::invalid: return "invalid";
            case ifexprstatetype::if_0: return "if_0";
            case ifexprstatetype::if_1: return "if_1";
            case ifexprstatetype::if_2: return "if_2";
            case ifexprstatetype::if_3: return "if_3";
            case ifexprstatetype::if_4: return "if_4";
            case ifexprstatetype::if_5: return "if_5";
            case ifexprstatetype::if_6: return "if_6";
            case ifexprstatetype::N: break;
            }

            return "ifexprstatetype?";
        }

        std::ostream &
        operator<<(std::ostream & os, ifexprstatetype x) {
            os << ifexprstatetype_descr(x);
            return os;
        }

        // ----- DIfElseSsm -----

        DIfElseSsm::DIfElseSsm(DIfElseExpr * ifelse_expr) : ifstate_{ifexprstatetype::if_0},
                                                            if_expr_{ifelse_expr}
        {}

        DIfElseSsm *
        DIfElseSsm::_make(DArena & mm,
                          DIfElseExpr * ifelse_expr)
        {
            void * mem = mm.alloc(typeseq::id<DIfElseSsm>(),
                                  sizeof(DIfElseSsm));

            return new (mem) DIfElseSsm(ifelse_expr);
        }

        void
        DIfElseSsm::start(DArena & parser_mm,
                          obj<AAllocator> expr_mm,
                          ParserStateMachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            DIfElseExpr * if_expr = DIfElseExpr::_make_empty(expr_mm);
            DIfElseSsm * if_ssm = DIfElseSsm::_make(parser_mm, if_expr);

            obj<ASyntaxStateMachine> ssm
                = with_facet<ASyntaxStateMachine>::mkobj(if_ssm);

            p_psm->push_ssm(ssm);

            // note: triggers poly dispatch
            p_psm->on_token(Token::if_token());
        }

        syntaxstatetype
        DIfElseSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::ifelseexpr;
        }

        std::string_view
        DIfElseSsm::get_expect_str() const noexcept
        {
            /**
             *   if test-expr then then-expr else else-expr ;
             *  ^  ^         ^    ^         ^    ^         ^
             *  |  |         |    |         |    |         |
             *  |  if_1      if_2 if_3      if_4 if_5      if_6
             *  if_0
             *
             *  if_0 --on_if_token()--> if_1
             *  if_1 --on_expr()--> if_2
             *  if_2 --on_then_token()--> if_3
             *  if_3 --on_expr()--> if_4
             *  if_4 --on_else_token()--> if_5
             *       --on_semicolon_token()--> (done)
             *  if_5 --on_expr()-->if_6
             *  if_6 --on_semicolon_token()--> (done)
             **/
            switch (this->ifstate_) {
            case ifexprstatetype::invalid:
            case ifexprstatetype::N:
                assert(false); // unreachable
                break;
            case ifexprstatetype::if_0:
                return "if";
            case ifexprstatetype::if_1:
                return "expression";
            case ifexprstatetype::if_2:
                return "then";
            case ifexprstatetype::if_3:
                return "expression";
            case ifexprstatetype::if_4:
                return "else|semicolon";
            case ifexprstatetype::if_5:
                return "expression";
            case ifexprstatetype::if_6:
                return "semicolon";
            }

            return "?expect";
        }

        void
        DIfElseSsm::on_token(const Token & tk,
                             ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {
            case tokentype::tk_symbol:
            case tokentype::tk_def:
                break;
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
            case tokentype::tk_singleassign:
            case tokentype::tk_string:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_semicolon:
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
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                break;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DIfElseSsm::on_if_token(const Token & tk,
                                ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log("ifstate", ifstate_);

            if (ifstate_ == ifexprstatetype::if_0) {
                this->ifstate_ = ifexprstatetype::if_1;

                DExpectExprSsm::start(p_psm);
                return;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DIfElseSsm::on_then_token(const Token & tk,
                                  ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log("ifstate", ifstate_);

            if (ifstate_ == ifexprstatetype::if_2) {
                this->ifstate_ = ifexprstatetype::if_3;

                DExpectExprSsm::start(p_psm);
                return;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DIfElseSsm::on_else_token(const Token & tk,
                                  ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log("ifstate", ifstate_);

            if (ifstate_ == ifexprstatetype::if_4) {
                this->ifstate_ = ifexprstatetype::if_5;

                DExpectExprSsm::start(p_psm);
                return;
            }

            Super::on_token(tk, p_psm);
        }

#ifdef NOT_YET
        void
        if_else_xs::finish_and_continue(parserstatemachine * p_psm)
        {
            rp<IfExprAccess> if_expr = this->if_expr_;
            std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

            if (this->ifxs_type_ == ifexprstatetype::if_4) {
                /* if no else-branch, then if-expr can't have valuetype */
                if_expr->assign_valuetype(nullptr);
            }

            p_psm->top_exprstate().on_expr(if_expr, p_psm);
        }
#endif

        void
        DIfElseSsm::finish_and_continue(ParserStateMachine * p_psm)
        {
            p_psm->pop_ssm();

            //            rp<IfExprAccess> if_expr = this->if_expr_;
            //            std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

            p_psm->on_parsed_expression(if_expr_);
        }

#ifdef NOT_YET
        void
        if_else_xs::on_rightbrace_token(const token_type & tk,
                                        parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            this->finish_and_continue(p_psm);
            p_psm->on_rightbrace_token(tk);
        }

#endif

        void
        DIfElseSsm::on_semicolon_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log("ifstate", ifstate_);

            switch (ifstate_) {
            case ifexprstatetype::invalid:
            case ifexprstatetype::N:
                // unreachable
                assert(false);
                break;

            case ifexprstatetype::if_0:
            case ifexprstatetype::if_1:
            case ifexprstatetype::if_2:
            case ifexprstatetype::if_3:
            case ifexprstatetype::if_5:
                break;
            case ifexprstatetype::if_4:
            case ifexprstatetype::if_6:
                this->finish_and_continue(p_psm);
                return;
            }

            Super::on_token(tk, p_psm);
        }

#ifdef NOT_YET
        void
        if_else_xs::on_expr(bp<Expression> expr,
                            parserstatemachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            log && log(xtag("ifxs_type", ifxs_type_));

            switch (this->ifxs_type_) {
            case ifexprstatetype::invalid:
            case ifexprstatetype::if_0:
            case ifexprstatetype::n_ifexprstatetype:
                assert(false); // unreachable
                return;
            case ifexprstatetype::if_1:
                if_expr_->assign_test(expr.promote());
                ifxs_type_ = ifexprstatetype::if_2;
                return;
            case ifexprstatetype::if_2:
                /** error: expecting 'then' **/
                break;
            case ifexprstatetype::if_3:
                if_expr_->assign_when_true(expr.promote());
                ifxs_type_ = ifexprstatetype::if_4;
                return;
            case ifexprstatetype::if_4:
                /** error: expecting 'else' or ';' **/
                break;
            case ifexprstatetype::if_5:
                if_expr_->assign_when_false(expr.promote());
                ifxs_type_ = ifexprstatetype::if_6;
                return;
            case ifexprstatetype::if_6:
                /** error: expecting ';' **/
                break;
            }

            constexpr const char* c_self_name = "if_else_xs::on_expr";
            const char * exp = get_expect_str();

            this->illegal_input_on_expr(c_self_name, expr, exp, p_psm);
        }

#endif

        void
        DIfElseSsm::on_parsed_expression(obj<AExpression> expr,
                                         ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));
            log && log(xtag("ifstate", ifstate_));

            // if (ifstate_ == ...) { .... return; }

            switch (ifstate_) {
            case ifexprstatetype::invalid:
            case ifexprstatetype::N:
                assert(false);
                break;
            case ifexprstatetype::if_0:
                 // should be unreachable
                break;
            case ifexprstatetype::if_1:
                if_expr_.data()->assign_test(expr);
                this->ifstate_ = ifexprstatetype::if_2;
                return;
            case ifexprstatetype::if_2:
                // error: expecting "then" token here
                break;
            case ifexprstatetype::if_3:
                if_expr_.data()->assign_when_true(expr);
                this->ifstate_ = ifexprstatetype::if_4;
                return;
            case ifexprstatetype::if_4:
                // error:  expecting "else" or ";"
                break;
            case ifexprstatetype::if_5:
                if_expr_.data()->assign_when_false(expr);
                this->ifstate_ = ifexprstatetype::if_6;
                return;
            case ifexprstatetype::if_6:
                // error: expecting ";"
                break;
            }

            Super::on_parsed_expression(expr, p_psm);
        }

        void
        DIfElseSsm::on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                        ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            this->on_parsed_expression(expr, p_psm);
            this->on_semicolon_token(Token::semicolon_token(), p_psm);
        }

        bool
        DIfElseSsm::pretty(const ppindentinfo & ppii) const
        {
            auto expr
                = FacetRegistry::instance().variant<APrintable,
                                                    AExpression>(if_expr_);
            assert(expr.data());
            (void)expr;

            return ppii.pps()->pretty_struct
                       (ppii,
                        "DIfElseSsm",
                        refrtag("ifstate", ifstate_),
                        refrtag("if_expr", expr));
        }

    } /*namespace scm*/
} /*namespace xo*/
