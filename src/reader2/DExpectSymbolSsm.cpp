/** @file DExpectSymbolSsm.cpp
 *
 *  @author Roland Conybeare, Aug 2024
 **/

#include "DExpectSymbolSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectSymbolSsm.hpp"
#include "SyntaxStateMachine.hpp"
#include "ParserStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <string_view>
//#include <regex>

namespace xo {
    using xo::facet::with_facet;
    using xo::facet::typeseq;

    namespace scm {
        DExpectSymbolSsm::DExpectSymbolSsm()
        {}

        DExpectSymbolSsm *
        DExpectSymbolSsm::make(DArena & mm)
        {
            void * mem = mm.alloc(typeseq::id<DExpectSymbolSsm>(),
                                  sizeof(DExpectSymbolSsm));

            return new (mem) DExpectSymbolSsm();
        }

        void
        DExpectSymbolSsm::start(ParserStateMachine * p_psm)
        {
            DExpectSymbolSsm * sym_ssm
                = DExpectSymbolSsm::make(p_psm->parser_alloc());

            // note:
            // relying on [ISyntaxStateMachine_DExpectedSymbolSsm.hpp]
            //
            obj<ASyntaxStateMachine> ssm
                = with_facet<ASyntaxStateMachine>::mkobj(sym_ssm);

            p_psm->push_ssm(ssm);
        }

        syntaxstatetype
        DExpectSymbolSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::expect_symbol;
        }

        std::string_view
        DExpectSymbolSsm::get_expect_str() const noexcept
        {
            return "symbol";
        }

        void
        DExpectSymbolSsm::on_parsed_symbol(std::string_view sym,
                                           ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("DExpectSymbolSsm::on_parsed_symbol",
                                           sym,
                                           this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_parsed_typedescr(TypeDescr td,
                                              ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_typedescr("DExpectSymbolSsm::on_parsed_typedescr",
                                              td,
                                              this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_parsed_formal(const DUniqueString * param_name,
                                           TypeDescr param_type,
                                           ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_formal("DExpectSymbolSsm::on_parsed_formal",
                                         param_name,
                                         param_type,
                                         this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_parsed_formal_arglist(DArray * arglist,
                                                   ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_formal_arglist("DExpectSymbolSsm::on_parsed_formal_arglist",
                                                 arglist,
                                                 this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_parsed_expression(obj<AExpression> expr,
                                               ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_expression("DExpectSymbolSsm::on_parsed_expression",
                                             expr,
                                             this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                              ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_expression("DExpectSymbolSsm::on_parsed_expression_with_semicolon",
                                             expr,
                                             this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_token(const Token & tk,
                                   ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {
            case tokentype::tk_symbol:
                this->on_symbol_token(tk, p_psm);
                break;

            case tokentype::tk_def:
                this->on_def_token(tk, p_psm);
                break;

            case tokentype::tk_if:
                this->on_if_token(tk, p_psm);
                break;

            case tokentype::tk_colon:
                this->on_colon_token(tk, p_psm);
                break;

            case tokentype::tk_singleassign:
                this->on_singleassign_token(tk, p_psm);
                break;

            case tokentype::tk_string:
                this->on_string_token(tk, p_psm);
                break;

            case tokentype::tk_f64:
                this->on_f64_token(tk, p_psm);
                break;

            case tokentype::tk_i64:
                this->on_i64_token(tk, p_psm);
                break;

            case tokentype::tk_bool:
                this->on_bool_token(tk, p_psm);
                break;

            case tokentype::tk_semicolon:
                this->on_semicolon_token(tk, p_psm);
                break;

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
                p_psm->illegal_input_on_token("DExpectSymbolSsm::on_token",
                                              tk,
                                              this->get_expect_str());
                break;
            }

        }

        void
        DExpectSymbolSsm::on_symbol_token(const Token & tk,
                                          ParserStateMachine * p_psm)
        {
#ifdef NOT_YET
            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag));

            log && log(xtag("tk", tk));

            assert(&p_psm->top_exprstate() == this);
#endif

            /* have to do pop first, before sending symbol to
             * the o.g. symbol-requester
             */
            p_psm->pop_ssm();

            p_psm->on_parsed_symbol(std::string_view(tk.text()));
        }

        void
        DExpectSymbolSsm::on_def_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectSymbolSsm::on_def_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_if_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectSymbolSsm::on_if_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_colon_token(const Token & tk,
                                         ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectSymbolSsm::on_colon_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_singleassign_token(const Token & tk,
                                                ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectSymbolSsm::on_singleassign_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_string_token(const Token & tk,
                                          ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectSymbolSsm::on_string_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_f64_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectSymbolSsm::on_f64_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_i64_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectSymbolSsm::on_i64_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_bool_token(const Token & tk,
                                        ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectSymbolSsm::on_bool_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectSymbolSsm::on_semicolon_token(const Token & tk,
                                             ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectSymbolSsm::on_semicolon_token",
                                          tk,
                                          this->get_expect_str());
        }

        bool
        DExpectSymbolSsm::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                (ppii,
                 "DExpectSymbolSsm"
                 //refrtag("member", member_)
                    );
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectSymbolSsm.cpp */
