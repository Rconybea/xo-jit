/* @file DExpectQLiteralSsm.cpp
 *
 * @author Roland Conybeare, Mar 2026
 */

#include "ExpectQLiteralSsm.hpp"
#include "ExpectQListSsm.hpp"
#include "ExpectQArraySsm.hpp"
#include "ExpectQDictSsm.hpp"
#include <xo/object2/Float.hpp>
#include <xo/object2/Integer.hpp>
#include <xo/stringtable2/String.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
//    using xo::print::APrintable;
//    using xo::print::ppstate;
//    using xo::print::ppindentinfo;
    using xo::mm::AGCObject;
//    using xo::mm::AAllocator;
//    using xo::facet::FacetRegistry;
//    using xo::reflect::typeseq;

    namespace scm {
        DExpectQLiteralSsm::DExpectQLiteralSsm(bool cxl_on_rightparen,
                                               bool cxl_on_rightbracket)
            : cxl_on_rightparen_{cxl_on_rightparen},
              cxl_on_rightbracket_{cxl_on_rightbracket}
        {}

        DExpectQLiteralSsm *
        DExpectQLiteralSsm::_make(DArena & arena,
                                  bool cxl_on_rightparen,
                                  bool cxl_on_rightbracket)
        {
            /* out-of-order so argl follows ssm in arena,
             * consistent with any subsequent arglist realloc.
             * Not a load-bearing choice however
             */

            void * mem = arena.alloc_for<DExpectQLiteralSsm>();

            return new (mem) DExpectQLiteralSsm(cxl_on_rightparen, cxl_on_rightbracket);
        }

        obj<ASyntaxStateMachine,DExpectQLiteralSsm>
        DExpectQLiteralSsm::make(DArena & arena,
                                 bool cxl_on_rightparen,
                                 bool cxl_on_rightbracket)
        {
            obj<ASyntaxStateMachine,DExpectQLiteralSsm> retval(_make(arena,
                                                                     cxl_on_rightparen,
                                                                     cxl_on_rightbracket));
            return retval;
        }

        void
        DExpectQLiteralSsm::start(ParserStateMachine * p_psm,
                                  bool cxl_on_rightparen,
                                  bool cxl_on_rightbracket)
        {
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            p_psm->push_ssm(ckp, DExpectQLiteralSsm::make(p_psm->parser_alloc(),
                                                          cxl_on_rightparen,
                                                          cxl_on_rightbracket));
        }

        syntaxstatetype
        DExpectQLiteralSsm::ssm_type() const noexcept {
            return syntaxstatetype::expect_qliteral;
        }

        std::string_view
        DExpectQLiteralSsm::get_expect_str() const
        {
            return "leftparen|leftbracket|leftbrace|string|f64|i64|bool";
        }

        void
        DExpectQLiteralSsm::on_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            switch (tk.tk_type()) {

            case tokentype::tk_f64:
                this->on_f64_token(tk, p_psm);
                return;

            case tokentype::tk_i64:
                this->on_i64_token(tk, p_psm);
                return;

            case tokentype::tk_string:
                this->on_string_token(tk, p_psm);
                return;

            case tokentype::tk_leftparen:
                this->on_leftparen_token(tk, p_psm);
                return;

            case tokentype::tk_rightparen:
                this->on_rightparen_token(tk, p_psm);
                return;

            case tokentype::tk_leftbracket:
                this->on_leftbracket_token(tk, p_psm);
                return;

            case tokentype::tk_rightbracket:
                this->on_rightbracket_token(tk, p_psm);
                return;

            case tokentype::tk_leftbrace:
                this->on_leftbrace_token(tk, p_psm);
                return;

            case tokentype::tk_comma:
            case tokentype::tk_lambda:
            case tokentype::tk_def:
            case tokentype::tk_deftype:
            case tokentype::tk_if:
            case tokentype::tk_symbol:
            case tokentype::tk_colon:
            case tokentype::tk_singleassign:
            case tokentype::tk_bool:
            case tokentype::tk_semicolon:
            case tokentype::tk_invalid:
            case tokentype::tk_quote:
            case tokentype::tk_rightbrace:
            case tokentype::tk_leftangle:
            case tokentype::tk_rightangle:
            case tokentype::tk_cmple:
            case tokentype::tk_cmpge:
            case tokentype::tk_dot:
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
        DExpectQLiteralSsm::on_f64_token(const Token & tk,
                                         ParserStateMachine * p_psm)
        {
            auto literal = DFloat::box<AGCObject>(p_psm->expr_alloc(),
                                                  tk.f64_value());

            p_psm->pop_ssm();
            p_psm->on_quoted_literal(literal);
        }

        void
        DExpectQLiteralSsm::on_i64_token(const Token & tk,
                                         ParserStateMachine * p_psm)
        {
            auto literal = DInteger::box<AGCObject>(p_psm->expr_alloc(),
                                                    tk.i64_value());

            p_psm->pop_ssm();
            p_psm->on_quoted_literal(literal);
        }

        void
        DExpectQLiteralSsm::on_string_token(const Token & tk,
                                            ParserStateMachine * p_psm)
        {
            auto literal = obj<AGCObject,DString>(DString::from_view(p_psm->expr_alloc(),
                                                                     std::string_view(tk.text())));

            p_psm->pop_ssm();
            p_psm->on_quoted_literal(literal);
        }

        void
        DExpectQLiteralSsm::on_leftparen_token(const Token & tk,
                                               ParserStateMachine * p_psm)
        {
            // replace self with specialized version for parsing a literal list

            p_psm->pop_ssm();
            DExpectQListSsm::start(p_psm);
            p_psm->on_token(tk);
        }

        void
        DExpectQLiteralSsm::on_rightparen_token(const Token & tk,
                                                ParserStateMachine * p_psm)
        {
            if (cxl_on_rightparen_) {
                p_psm->pop_ssm();
                p_psm->on_token(tk);
                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DExpectQLiteralSsm::on_leftbracket_token(const Token & tk,
                                                 ParserStateMachine * p_psm)
        {
            // replace self with specialized version for parsing a literal array

            p_psm->pop_ssm();
            DExpectQArraySsm::start(p_psm);
            p_psm->on_token(tk);
        }

        void
        DExpectQLiteralSsm::on_rightbracket_token(const Token & tk,
                                                  ParserStateMachine * p_psm)
        {
            if (cxl_on_rightbracket_) {
                p_psm->pop_ssm();
                p_psm->on_token(tk);
                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DExpectQLiteralSsm::on_leftbrace_token(const Token & tk,
                                               ParserStateMachine * p_psm)
        {
            // replace self with specialized version for parsing a literal dict

            p_psm->pop_ssm();
            DExpectQDictSsm::start(p_psm);
            p_psm->on_token(tk);
        }

        bool
        DExpectQLiteralSsm::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DExpectQLiteralSsm",
                                             refrtag("expect", this->get_expect_str()));
        }
        void
        DExpectQLiteralSsm::visit_gco_children(obj<AGCObjectVisitor>) noexcept
        {
            // cxl_on_rightparen_, cxl_on_rightbracket_: POD, skip
        }

    } /*namespace scm*/
} /*namespace xo*/


/* end DExpectQLiteralSsm.cpp */
