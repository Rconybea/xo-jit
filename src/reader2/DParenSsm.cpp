/** @file DParenSsm.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "ParenSsm.hpp"
#include "syntaxstatetype.hpp"
#include <string_view>

namespace xo {
    using xo::facet::with_facet;
    using xo::facet::typeseq;

    namespace scm {

        extern const char *
        parenexprstatetype_descr(parenexprstatetype x)
        {
            switch(x) {
            case parenexprstatetype::invalid: return "invalid";
            case parenexprstatetype::lparen_0: return "lparen_0";
            case parenexprstatetype::lparen_1: return "lparen_1";
            case parenexprstatetype::lparen_2: return "lparen_2";
            case parenexprstatetype::N: break;
            }

            return "???parenexprstatetype";
        }

        std::ostream &
        operator<<(std::ostream & os, parenexprstatetype x) {
            os << parenexprstatetype_descr(x);
            return os;
        }

        DParenSsm::DParenSsm()
            : parenstate_(parenexprstatetype::lparen_0),
              expr_{}
        {}

        DParenSsm *
        DParenSsm::make(DArena & mm)
        {
            void * mem = mm.alloc(typeseq::id<DParenSsm>(),
                                  sizeof(DParenSsm));

            return new (mem) DParenSsm();
        }

        void
        DParenSsm::start(ParserStateMachine * p_psm)
        {
            DParenSsm * paren_ssm = DParenSsm::make(p_psm->parser_alloc());

            auto ssm = with_facet<ASyntaxStateMachine>::mkobj(paren_ssm);

            p_psm->push_ssm(ssm);
        }

        syntaxstatetype
        DParenSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::paren;
        }

        std::string_view
        DParenSsm::get_expect_str() const noexcept
        {
            switch (this->parenstate_) {
            case parenexprstatetype::invalid:
            case parenexprstatetype::N:
                break;
            case parenexprstatetype::lparen_0: return "lparen_0";
            case parenexprstatetype::lparen_1: return "lparen_1";
            case parenexprstatetype::lparen_2: return "lparen_2";
            }

            return "???parenexprstatetype";
        }

        void
        DParenSsm::on_token(const Token & tk,
                            ParserStateMachine * p_psm)
        {
            switch (tk.tk_type()) {
            case tokentype::tk_leftparen:
                this->on_leftparen_token(tk, p_psm);
                return;
            // all the not-yet handled cases
            case tokentype::tk_symbol:
            case tokentype::tk_def:
            case tokentype::tk_colon:
            case tokentype::tk_singleassign:
            case tokentype::tk_semicolon:
            case tokentype::tk_invalid:
            case tokentype::tk_string:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_if:
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
                break;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DParenSsm::on_leftparen_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            Super::on_token(tk, p_psm);
        }

#ifdef OBSOLETE
        void
        paren_xs::start(parserstatemachine * p_psm)
        {
            p_psm->push_exprstate(paren_xs::make());
            expect_expr_xs::start(p_psm);
        }

        bool
        paren_xs::admits_rightparen() const {
            switch (parenxs_type_) {
            case parenexprstatetype::lparen_0:
                /* unreachable */
                assert(false);
                return false;

            case parenexprstatetype::lparen_1:
                return true;

            case parenexprstatetype::invalid:
            case parenexprstatetype::n_parenexprstatetype:
                /* unreachable */
                assert(false);
                return false;
            }

            return false;
        }

        bool
        paren_xs::admits_f64() const {
            switch (parenxs_type_) {
            case parenexprstatetype::lparen_0:
                return true;

            case parenexprstatetype::lparen_1:
                return false;

            case parenexprstatetype::invalid:
            case parenexprstatetype::n_parenexprstatetype:
                /* unreachable */
                assert(false);
                return false;
            }

            return false;
        }

        void
        paren_xs::on_def_token(const token_type & tk,
                               parserstatemachine * /*p_psm*/)
        {
            constexpr const char * c_self_name = "paren_xs::on_def";

            this->illegal_input_error(c_self_name, tk);
        }

        void
        paren_xs::on_symbol_token(const token_type & /*tk*/,
                                  parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            log && log(xtag("exstype", p_psm->top_exprstate().exs_type()));

            //constexpr const char * self_name = "paren_xs::on_symbol";

            /* TODO: lparen_0: treat as variable reference */

            assert(false);
        }

        void
        paren_xs::on_typedescr(TypeDescr /*td*/,
                               parserstatemachine * /*p_psm*/)
        {
            assert(false);
            return;
        }

        void
        paren_xs::on_colon_token(const token_type & tk,
                                 parserstatemachine * /*p_psm*/)
        {
            constexpr const char * c_self_name = "paren_xs::on_colon";

            this->illegal_input_error(c_self_name, tk);
        }

        void
        paren_xs::on_semicolon_token(const token_type & tk,
                                     parserstatemachine * /*p_psm*/)
        {
            constexpr const char * c_self_name = "paren_xs::on_semicolon";

            this->illegal_input_error(c_self_name, tk);
        }

        void
        paren_xs::on_singleassign_token(const token_type & tk,
                                        parserstatemachine * /*p_psm*/)
        {
            constexpr const char * c_self_name = "paren_xs::on_singleassign";

            this->illegal_input_error(c_self_name, tk);
        }

        void
        paren_xs::on_leftparen_token(const token_type & tk,
                                     parserstatemachine * /*p_psm*/)
        {
            constexpr const char * c_self_name = "paren_xs::on_leftparen";

            this->illegal_input_error(c_self_name, tk);
        }

        void
        paren_xs::on_rightparen_token(const token_type & tk,
                                      parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            constexpr const char * c_self_name = "paren_xs::on_rightparen";

            if (!this->admits_rightparen())
            {
                this->illegal_input_error(c_self_name, tk);
            }

            if (this->parenxs_type_ == parenexprstatetype::lparen_1) {
                rp<Expression> expr = this->gen_expr_;

                std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

                p_psm->top_exprstate().on_expr(expr, p_psm);
            }
        }

        void
        paren_xs::on_i64_token(const token_type & tk,
                               parserstatemachine * /*p_psm*/)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            constexpr const char * c_self_name = "paren_xs::on_i64";

            this->illegal_input_error(c_self_name, tk);
        }

        void
        paren_xs::on_f64_token(const token_type & tk,
                               parserstatemachine * /*p_psm*/)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            constexpr const char * c_self_name = "paren_xs::on_f64";

            this->illegal_input_error(c_self_name, tk);
        }

        void
        paren_xs::on_expr(bp<Expression> expr,
                          parserstatemachine * p_psm)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            log && log(xtag("exstype", this->exs_type_),
                       xtag("expr", expr));

            switch (this->parenxs_type_) {
            case parenexprstatetype::lparen_0: {
                this->parenxs_type_ = parenexprstatetype::lparen_1; /* wants on_rightparen */
                progress_xs::start(expr.promote(), p_psm);

                return;
            }

            case parenexprstatetype::lparen_1: {
                this->gen_expr_ = expr.promote();

                /* expect immediate incoming call, this time to on_rightparen() */
                return;
            }

            default:
                /* unreachable */
                assert(false);
                return;
            }
        } /*on_expr*/

        void
        paren_xs::on_symbol(const std::string & /*symbol_name*/,
                            parserstatemachine * /*p_psm*/)
        {
            switch(this->parenxs_type_) {
            case parenexprstatetype::lparen_0:
            case parenexprstatetype::lparen_1:
                /* NOT IMPLEMENTED */
                assert(false);
                return;

            default:
                /* unreachable */
                assert(false);
                return;
            }
        }

        void
        paren_xs::print(std::ostream & os) const {
            os << "<paren_xs"
               << xtag("this", (void*)this)
                //<< xtag("type", exs_type_);
               << xtag("parenxs_type", parenxs_type_);

            if (gen_expr_)
                os << xtag("gen_expr", gen_expr_);

            os << ">";
        }
#endif

        bool
        DParenSsm::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DParenSsm",
                                             refrtag("parenstate", parenstate_),
                                             refrtag("expect", this->get_expect_str()));
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DParenSsm.cpp */
