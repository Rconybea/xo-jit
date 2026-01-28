/** @file DExpectFormalArgSsm.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DExpectFormalArgSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectFormalArgSsm.hpp"
#include "DExpectSymbolSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectSymbolSsm.hpp"

#ifdef NOT_YET
#include "expect_type_xs.hpp"
//#include "parserstatemachine.hpp"
//#include "exprstatestack.hpp"
#include "xo/expression/Variable.hpp"
#endif

namespace xo {
    using xo::scm::DVariable;
    using xo::reflect::TypeDescr;
    using xo::facet::typeseq;

    namespace scm {
        const char *
        formalstatetype_descr(formalstatetype x) {
            switch (x) {
            case formalstatetype::invalid:
            case formalstatetype::n_formalstatetype:
                return "?formalstatetype";
            case formalstatetype::formal_0:
                return "formal_0";
            case formalstatetype::formal_1:
                return "formal_1";
            case formalstatetype::formal_2:
                return "formal_2";
            }

            return "???formalstatetype";
        }

        DExpectFormalArgSsm::DExpectFormalArgSsm() = default;

        obj<ASyntaxStateMachine,DExpectFormalArgSsm>
        DExpectFormalArgSsm::make(DArena & mm)
        {
            return obj<ASyntaxStateMachine,DExpectFormalArgSsm>(_make(mm));
        }

        DExpectFormalArgSsm *
        DExpectFormalArgSsm::_make(DArena & mm)
        {
            void * mem = mm.alloc(typeseq::id<DExpectFormalArgSsm>(), sizeof(DExpectFormalArgSsm));

            return new (mem) DExpectFormalArgSsm();
        }

        void
        DExpectFormalArgSsm::start(ParserStateMachine * p_psm)
        {
            p_psm->push_ssm(DExpectFormalArgSsm::make(p_psm->parser_alloc()));

            DExpectSymbolSsm::start(p_psm);
        }

        syntaxstatetype
        DExpectFormalArgSsm::ssm_type() const noexcept {
            return syntaxstatetype::expect_formal_arg;
        }

        std::string_view
        DExpectFormalArgSsm::get_expect_str() const noexcept
        {
            switch(fstate_) {
            case formalstatetype::invalid:
            case formalstatetype::n_formalstatetype:
                break;
            case formalstatetype::formal_0:
                return "formal-name";
            case formalstatetype::formal_1:
                return "colon|typename";
            case formalstatetype::formal_2:
                return "typename";
            }

            return "?expect";
        }

        /** update state on incoming token @p tk,
         *  with overall parser state in @p p_psm
         **/
        void
        DExpectFormalArgSsm::on_token(const Token & tk,
                                      ParserStateMachine * p_psm)
        {
            switch (tk.tk_type()) {
                // all the not-yet-handled cases
            case tokentype::tk_leftparen:
            case tokentype::tk_lambda:
            case tokentype::tk_def:
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
            case tokentype::tk_then:
            case tokentype::tk_else:
            case tokentype::tk_let:
            case tokentype::tk_in:
            case tokentype::tk_end:
            case tokentype::N:
                break;
            }

            p_psm->illegal_input_on_token("DExpectFormalArgSsm::on_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectFormalArgSsm::on_parsed_symbol(std::string_view sym,
                                              ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("DExpectFormalArgSsm::on_parsed_symbol",
                                           sym,
                                           this->get_expect_str());
        }

        void
        DExpectFormalArgSsm::on_parsed_typedescr(TypeDescr td,
                                                 ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_typedescr("DExpectFormalArgSsm::on_parsed_typedescr",
                                              td,
                                              this->get_expect_str());
        }

        void
        DExpectFormalArgSsm::on_parsed_expression(obj<AExpression> expr,
                                                  ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_expression("DExpectFormalArgSsm::on_parsed_expression",
                                               expr,
                                               this->get_expect_str());
        }

        void
        DExpectFormalArgSsm::on_parsed_expression_with_semicolon(obj<AExpression> expr,
                                                                 ParserStateMachine * p_psm)
        {
            p_psm->illegal_parsed_expression("DExpectFormalArgSsm::on_parsed_expression_with_semicolon",
                                             expr,
                                             this->get_expect_str());
        }

#ifdef NOT_YET
        expect_formal_xs::expect_formal_xs()
            : exprstate(exprstatetype::expect_formal)
        {}

        void
        expect_formal_xs::on_symbol(const std::string & symbol_name,
                                    parserstatemachine * p_psm)
        {
            if (this->formalxs_type_ == formalstatetype::formal_0) {
                this->formalxs_type_ = formalstatetype::formal_1;
                this->result_.assign_name(symbol_name);
            } else {
                exprstate::on_symbol(symbol_name, p_psm);
            }
        }

        void
        expect_formal_xs::on_colon_token(const token_type & tk,
                                         parserstatemachine * p_psm)
        {
            if (this->formalxs_type_ == formalstatetype::formal_1) {
                this->formalxs_type_ = formalstatetype::formal_2;
                expect_type_xs::start(p_psm);
                /* control reenters via expect_formal_xs::on_typedescr() */
            } else {
                exprstate::on_colon_token(tk,
                                          p_psm);
            }
        }

        void
        expect_formal_xs::on_typedescr(TypeDescr td,
                                       parserstatemachine * p_psm)
        {
            if (this->formalxs_type_ == formalstatetype::formal_2) {
                this->result_.assign_td(td);

                std::unique_ptr<exprstate> self = p_psm->pop_exprstate();

                rp<Variable> var = Variable::make(result_.name(),
                                                  result_.td());

                p_psm->top_exprstate().on_formal(var, p_psm);
            } else {
                exprstate::on_typedescr(td, p_psm);
            }
        }
#endif

        bool
        DExpectFormalArgSsm::pretty(const ppindentinfo & ppii) const {
            return ppii.pps()->pretty_struct
                       (ppii,
                        "DExpectFormalArgSsm");
        }

    } /*namespace scm*/
} /*namespace xo*/


/* end DExpectFormalArgSsm.cpp */
