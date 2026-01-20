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
        DExpectSymbolSsm::start(DArena & parser_alloc,
                                ParserStateMachine * p_psm)
        {
            DExpectSymbolSsm * sym_ssm
                = DExpectSymbolSsm::make(parser_alloc);

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
        DExpectSymbolSsm::on_parsed_symbol(std::string_view sym,
                                           ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("DExpectSymbolSsm::on_parsed_symbol",
                                           sym,
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
