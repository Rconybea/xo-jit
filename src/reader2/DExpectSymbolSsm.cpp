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
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            DExpectSymbolSsm * sym_ssm
                = DExpectSymbolSsm::make(p_psm->parser_alloc());

            // note:
            // relying on [ISyntaxStateMachine_DExpectedSymbolSsm.hpp]
            //
            obj<ASyntaxStateMachine> ssm
                = with_facet<ASyntaxStateMachine>::mkobj(sym_ssm);

            p_psm->push_ssm(ckp, ssm);
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
        DExpectSymbolSsm::on_token(const Token & tk,
                                   ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {
            case tokentype::tk_symbol:
                this->on_symbol_token(tk, p_psm);
                return;

            // all the not-yet handled cases
            case tokentype::tk_invalid:
            case tokentype::tk_string:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_def:
            case tokentype::tk_if:
            case tokentype::tk_singleassign:
            case tokentype::tk_colon:
            case tokentype::tk_semicolon:
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
                break;
            }

            Super::on_token(tk, p_psm);
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
