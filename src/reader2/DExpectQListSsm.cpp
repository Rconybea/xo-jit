/** @file DExpectQListSsm.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "ExpectQListSsm.hpp"
#include "ExpectQLiteralSsm.hpp"
#include <xo/object2/List.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::mm::AGCObject;

    namespace scm {
        const char *
        QListXst::_descr(enum code x)
        {
            switch (x) {
            case code::invalid: break;
            case code::qlist_0: return "qlist_0";
            case code::qlist_1a: return "qlist_1a";
            case code::qlist_2: return "qlist_2";
            case code::N: break;
            }

            return "?QListXst";
        }

        DExpectQListSsm::DExpectQListSsm() : state_{QListXst::code::qlist_0} {}

        obj<ASyntaxStateMachine,DExpectQListSsm>
        DExpectQListSsm::make(DArena & parser_mm)
        {
            return obj<ASyntaxStateMachine,DExpectQListSsm>(_make(parser_mm));
        }

        DExpectQListSsm *
        DExpectQListSsm::_make(DArena & parser_mm)
        {
            void * mem = parser_mm.alloc_for<DExpectQListSsm>();

            return new (mem) DExpectQListSsm();
        }

        void
        DExpectQListSsm::start(ParserStateMachine * p_psm)
        {
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            p_psm->push_ssm(ckp, DExpectQListSsm::make(p_psm->parser_alloc()));
        }

        syntaxstatetype
        DExpectQListSsm::ssm_type() const noexcept {
            return syntaxstatetype::expect_qlist;
        }

        std::string_view
        DExpectQListSsm::get_expect_str() const {
            switch (state_.code()) {
            case QListXst::code::qlist_0:
                return "leftparen";
            case QListXst::code::qlist_1a:
                return "qliteral|rightparen";
            case QListXst::code::qlist_2:
                return "(done)";
            case QListXst::code::invalid:
            case QListXst::code::N:
                break;
            }

            return "?DExpectQListSsm";
        }

        void
        DExpectQListSsm::on_token(const Token & tk,
                                  ParserStateMachine * p_psm)
        {
            switch(tk.tk_type())
            {
            case tokentype::tk_leftparen:
                this->on_leftparen_token(tk, p_psm);
                return;

            case tokentype::tk_rightparen:
                this->on_rightparen_token(tk, p_psm);
                return;

            case tokentype::tk_comma:
            case tokentype::tk_lambda:
            case tokentype::tk_def:
            case tokentype::tk_deftype:
            case tokentype::tk_if:
            case tokentype::tk_symbol:
            case tokentype::tk_colon:
            case tokentype::tk_singleassign:
            case tokentype::tk_string:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_semicolon:
            case tokentype::tk_invalid:
            case tokentype::tk_quote:
            case tokentype::tk_leftbracket:
            case tokentype::tk_rightbracket:
            case tokentype::tk_leftbrace:
            case tokentype::tk_rightbrace:
            case tokentype::tk_leftangle:
            case tokentype::tk_rightangle:
            case tokentype::tk_lessequal:
            case tokentype::tk_greatequal:
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
            case tokentype::tk_type:
            case tokentype::tk_then:
            case tokentype::tk_else:
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                break;
            }
        }

        void
        DExpectQListSsm::on_leftparen_token(const Token & tk,
                                            ParserStateMachine * p_psm)
        {
            if (state_.code() == QListXst::code::qlist_0) {
                this->state_ = QListXst(QListXst::code::qlist_1a);
                this->start_ = DList::_nil();
                this->end_ = nullptr;

                DExpectQLiteralSsm::start(p_psm,
                                          true /*cxl_on_rightparen*/);
                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DExpectQListSsm::on_rightparen_token(const Token & tk,
                                             ParserStateMachine * p_psm)
        {
            if (state_.code() == QListXst::code::qlist_1a) {
                this->state_ = QListXst(QListXst::code::qlist_2);

                obj<AGCObject> lit = obj<AGCObject,DList>(start_);

                p_psm->pop_ssm();
                p_psm->on_quoted_literal(lit);
                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DExpectQListSsm::on_quoted_literal(obj<AGCObject> lit,
                                           ParserStateMachine * p_psm)
        {
            if(state_.code() == QListXst::code::qlist_1a) {
                // append lit at the end of list start_ .. end_
                {
                    DList * new_last
                        = DList::_cons(p_psm->expr_alloc(), lit, DList::_nil());

                    if (this->end_) {
                        end_->assign_rest(new_last);

                        this->end_ = new_last;
                    } else {
                        this->start_ = DList::_cons(p_psm->expr_alloc(),
                                                    lit,
                                                    DList::_nil());
                        this->end_ = this->start_;
                    }
                }

                // start syntax to receive next literal
                DExpectQLiteralSsm::start(p_psm,
                                          true /*cxl_on_rightparen*/);
                return;
            }

            Super::illegal_quoted_literal(lit, p_psm);
        }

        bool
        DExpectQListSsm::pretty(const ppindentinfo & ppii) const
        {
            obj<AGCObject,DList> list(start_);
            auto list_pr = FacetRegistry::instance().variant<APrintable,AGCObject>(list);

            return ppii.pps()->pretty_struct(ppii,
                                             "DExpectQListSsm",
                                             refrtag("state", state_),
                                             refrtag("expect", this->get_expect_str()),
                                             refrtag("list", list_pr));
        }
    }
}

/* end DExpectQListSsm.cpp */
