/** @file DExpectQArraySsm.cpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#include "ExpectQArraySsm.hpp"
#include "ExpectQLiteralSsm.hpp"
#include <xo/object2/Array.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::mm::AGCObject;

    namespace scm {
        const char *
        QArrayXst::_descr(enum code x)
        {
            switch (x) {
            case code::invalid: break;
            case code::qarray_0: return "qarray_0";
            case code::qarray_1a: return "qarray_1a";
            case code::qarray_2: return "qarray_2";
            case code::N: break;
            }

            return "?QArrayXst";
        }

        DExpectQArraySsm::DExpectQArraySsm() : state_{QArrayXst::code::qarray_0} {}

        obj<ASyntaxStateMachine,DExpectQArraySsm>
        DExpectQArraySsm::make(DArena & parser_mm)
        {
            return obj<ASyntaxStateMachine,DExpectQArraySsm>(_make(parser_mm));
        }

        DExpectQArraySsm *
        DExpectQArraySsm::_make(DArena & parser_mm)
        {
            void * mem = parser_mm.alloc_for<DExpectQArraySsm>();

            return new (mem) DExpectQArraySsm();
        }

        void
        DExpectQArraySsm::start(ParserStateMachine * p_psm)
        {
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            p_psm->push_ssm(ckp, DExpectQArraySsm::make(p_psm->parser_alloc()));
        }

        syntaxstatetype
        DExpectQArraySsm::ssm_type() const noexcept {
            return syntaxstatetype::expect_qarray;
        }

        std::string_view
        DExpectQArraySsm::get_expect_str() const {
            switch (state_.code()) {
            case QArrayXst::code::qarray_0:
                return "leftparen";
            case QArrayXst::code::qarray_1a:
                return "qliteral|rightparen";
            case QArrayXst::code::qarray_2:
                return "(done)";
            case QArrayXst::code::invalid:
            case QArrayXst::code::N:
                break;
            }

            return "?DExpectQArraySsm";
        }

        void
        DExpectQArraySsm::on_token(const Token & tk,
                                  ParserStateMachine * p_psm)
        {
            switch(tk.tk_type())
            {
            case tokentype::tk_leftbracket:
                this->on_leftbracket_token(tk, p_psm);
                return;

            case tokentype::tk_rightbracket:
                this->on_rightbracket_token(tk, p_psm);
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
            case tokentype::tk_leftparen:
            case tokentype::tk_rightparen:
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
        }

        void
        DExpectQArraySsm::on_leftbracket_token(const Token & tk,
                                               ParserStateMachine * p_psm)
        {
            if (state_.code() == QArrayXst::code::qarray_0) {
                this->state_ = QArrayXst(QArrayXst::code::qarray_1a);

                this->array_ = DArray::empty(p_psm->expr_alloc(),
                                             8 /*heuristic starting capacity*/);

                DExpectQLiteralSsm::start(p_psm,
                                          false /*cxl_on_rightparen*/,
                                          true /*cxl_on_rightbracket*/);
                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DExpectQArraySsm::on_rightbracket_token(const Token & tk,
                                                ParserStateMachine * p_psm)
        {
            if (state_.code() == QArrayXst::code::qarray_1a) {
                this->state_ = QArrayXst(QArrayXst::code::qarray_2);

                array_->shrink_to_fit();

                obj<AGCObject> lit = obj<AGCObject,DArray>(array_);

                p_psm->pop_ssm();
                p_psm->on_quoted_literal(lit);
                return;
            }

            Super::illegal_token(tk, p_psm);
        }

        void
        DExpectQArraySsm::on_quoted_literal(obj<AGCObject> lit,
                                           ParserStateMachine * p_psm)
        {
            if(state_.code() == QArrayXst::code::qarray_1a) {
                // append lit at the end of array_
                {
                    assert(array_);

                    // ensure sufficient capacity
                    if (array_->size() == array_->capacity()) {
                        assert(array_->capacity() > 0);

                        /* reallocate w/ 2x capacity */
                        this->array_ = DArray::copy(p_psm->expr_alloc(),
                                                    array_,
                                                    2 * array_->capacity());
                    }

                    bool ok = array_->push_back(lit);

                    assert(ok);
                }

                // start syntax to receive next literal
                DExpectQLiteralSsm::start(p_psm,
                                          false /*cxl_on_rightparen*/,
                                          true /*cxl_on_rightbracket*/);
                return;
            }

            Super::illegal_quoted_literal(lit, p_psm);
        }

        bool
        DExpectQArraySsm::pretty(const ppindentinfo & ppii) const
        {
            obj<AGCObject,DArray> array(array_);
            auto array_pr = FacetRegistry::instance().variant<APrintable,AGCObject>(array);

            return ppii.pps()->pretty_struct(ppii,
                                             "DExpectQArraySsm",
                                             refrtag("state", state_),
                                             refrtag("expect", this->get_expect_str()),
                                             refrtag("array", array_pr));
        }
    }
}

/* end DExpectQArraySsm.cpp */
