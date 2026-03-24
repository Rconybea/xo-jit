/** @file DQuoterSsm.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "QuoteSsm.hpp"
#include "ExpectQLiteralSsm.hpp"
#include "syntaxstatetype.hpp"
#include <xo/expression2/Constant.hpp>
//#include <string_view>

namespace xo {
    using xo::facet::with_facet;
    using xo::facet::typeseq;

    namespace scm {

        extern const char *
        QuoteXst::_descr(enum QuoteXst::code x)
        {
            switch (x) {
            case code::invalid: return "invalid";
            case code::quote_0: return "quote_0";
            case code::quote_1: return "quote_1";
            case code::quote_2: return "quote_2";
            case code::quote_3: return "quote_3";
            case code::N: break;
            }

            return "?QuoteXst";
        }

        DQuoteSsm::DQuoteSsm()
            : quote_xst_(QuoteXst::code::quote_0),
              expr_{}
        {}

        DQuoteSsm *
        DQuoteSsm::_make(DArena & mm)
        {
            void * mem = mm.alloc_for<DQuoteSsm>();

            return new (mem) DQuoteSsm();
        }

        obj<ASyntaxStateMachine,DQuoteSsm>
        DQuoteSsm::make(DArena & mm)
        {
            return obj<ASyntaxStateMachine,DQuoteSsm>(_make(mm));
        }

        void
        DQuoteSsm::start(ParserStateMachine * p_psm)
        {
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            auto paren_ssm = DQuoteSsm::make(p_psm->parser_alloc());

            p_psm->push_ssm(ckp, paren_ssm);
        }

        syntaxstatetype
        DQuoteSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::paren;
        }

        std::string_view
        DQuoteSsm::get_expect_str() const noexcept
        {
            switch (this->quote_xst_.code()) {
            case QuoteXst::code::invalid:
            case QuoteXst::code::N:
                break;
            case QuoteXst::code::quote_0: return "#q";
            case QuoteXst::code::quote_1: return "leftbrace";
            case QuoteXst::code::quote_2: return "qliteral";
            case QuoteXst::code::quote_3: return "rightbrace";
            }

            assert(false);

            return "?quotexst";
        }

        void
        DQuoteSsm::on_token(const Token & tk,
                            ParserStateMachine * p_psm)
        {
            switch (tk.tk_type()) {

            case tokentype::tk_quote:
                this->on_quote_token(tk, p_psm);
                return;

            case tokentype::tk_leftbrace:
                this->on_leftbrace_token(tk, p_psm);
                return;

            case tokentype::tk_rightbrace:
                this->on_rightbrace_token(tk, p_psm);
                return;

            // all the not-yet handled cases
            case tokentype::tk_symbol:
            case tokentype::tk_def:
            case tokentype::tk_deftype:
            case tokentype::tk_colon:
            case tokentype::tk_singleassign:
            case tokentype::tk_semicolon:
            case tokentype::tk_invalid:
            case tokentype::tk_string:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_if:
            case tokentype::tk_leftparen:
            case tokentype::tk_rightparen:
            case tokentype::tk_leftbracket:
            case tokentype::tk_rightbracket:
            case tokentype::tk_leftangle:
            case tokentype::tk_rightangle:
            case tokentype::tk_cmple:
            case tokentype::tk_cmpge:
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
            case tokentype::tk_nil:
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
        DQuoteSsm::on_quote_token(const Token & tk,
                                  ParserStateMachine * p_psm)
        {
            if (quote_xst_.code() == QuoteXst::code::quote_0) {
                this->quote_xst_ = QuoteXst(QuoteXst::code::quote_1);
                return;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DQuoteSsm::on_leftbrace_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            if (quote_xst_.code() == QuoteXst::code::quote_1) {
                this->quote_xst_ = QuoteXst(QuoteXst::code::quote_2);
                DExpectQLiteralSsm::start(p_psm);
                return;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DQuoteSsm::on_rightbrace_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            if (quote_xst_.code() == QuoteXst::code::quote_3) {
                // quoted literal (reported as constant expr) successfully parsed

                p_psm->pop_ssm();
                p_psm->on_parsed_expression(this->expr_);
                return;
            }

            Super::on_token(tk, p_psm);
        }

        void
        DQuoteSsm::on_quoted_literal(obj<AGCObject> literal,
                                     ParserStateMachine * p_psm)
        {
            if (quote_xst_.code() == QuoteXst::code::quote_2) {
                this->quote_xst_ = QuoteXst(QuoteXst::code::quote_3);
                this->expr_ = DConstant::make(p_psm->expr_alloc(), literal);

                return;
            }

            Super::illegal_quoted_literal(literal, p_psm);
        }

        bool
        DQuoteSsm::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DQuoteSsm",
                                             refrtag("quote_xst", quote_xst_),
                                             refrtag("expect", this->get_expect_str()));
        }

        void
        DQuoteSsm::forward_children(obj<ACollector> gc) noexcept
        {
            gc.forward_pivot_inplace(&expr_);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DQuoteSsm.cpp */
