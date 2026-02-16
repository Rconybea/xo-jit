/** @file DExpectTypeSsm.cpp
 *
 *  @author Roland Conybeare, Aug 2024
 **/

#include "DExpectTypeSsm.hpp"
#include "ssm/ISyntaxStateMachine_DExpectTypeSsm.hpp"
#include "SyntaxStateMachine.hpp"
#include <string_view>
#include <xo/reflect/Reflect.hpp>
#include <xo/facet/facet_implementation.hpp>
#include <xo/reflectutil/typeseq.hpp>
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    using xo::facet::with_facet;
    using xo::reflect::Reflect;
    using xo::reflect::TypeDescr;
    using xo::reflect::typeseq;

    namespace scm {
        DExpectTypeSsm::DExpectTypeSsm()
        {}

        DExpectTypeSsm *
        DExpectTypeSsm::make(DArena & mm)
        {
            void * mem = mm.alloc(typeseq::id<DArena>(),
                                  sizeof(DArena));

            return new (mem) DExpectTypeSsm();
        }

        void
        DExpectTypeSsm::start(ParserStateMachine * p_psm)
        {
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            DExpectTypeSsm * expect_type_ssm
                = DExpectTypeSsm::make(p_psm->parser_alloc());

            auto ssm
                = with_facet<ASyntaxStateMachine>::mkobj(expect_type_ssm);

            p_psm->push_ssm(ckp, ssm);
        }

        syntaxstatetype
        DExpectTypeSsm::ssm_type() const noexcept
        {
            return syntaxstatetype::expect_type;
        }

        std::string_view
        DExpectTypeSsm::get_expect_str() const noexcept
        {
            return "typename";
        }

        void
        DExpectTypeSsm::on_token(const Token & tk,
                                 ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()), xtag("tk", tk));

            switch (tk.tk_type()) {
            case tokentype::tk_symbol:
                this->on_symbol_token(tk, p_psm);
                break;

            // all the not-yet handled cases
            case tokentype::tk_invalid:
            case tokentype::tk_def:
            case tokentype::tk_if:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_string:
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
            case tokentype::tk_colon:
            case tokentype::tk_semicolon:
            case tokentype::tk_doublecolon:
            case tokentype::tk_assign:
            case tokentype::tk_singleassign:
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
                p_psm->illegal_input_on_token("DExpectTypeSsm::on_token",
                                              tk,
                                              this->get_expect_str());
                break;
            }
        }

        void
        DExpectTypeSsm::on_symbol_token(const Token & tk,
                                        ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(p_psm->debug_flag()));

            TypeDescr td = nullptr;

            /* TODO: replace with typetable lookup */

            if (tk.text() == "bool")
                td = Reflect::require<bool>();
            else if (tk.text() == "str")
                td = Reflect::require<std::string>();
            else if (tk.text() == "f64")
                td = Reflect::require<double>();
            else if(tk.text() == "f32")
                td = Reflect::require<float>();
            else if(tk.text() == "i16")
                td = Reflect::require<std::int16_t>();
            else if(tk.text() == "i32")
                td = Reflect::require<std::int32_t>();
            else if(tk.text() == "i64")
                td = Reflect::require<std::int64_t>();

            if (!td) {
                Super::on_token(tk, p_psm);
                return;
            }

            p_psm->pop_ssm();
            p_psm->on_parsed_typedescr(td);
        }

        bool
        DExpectTypeSsm::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                (ppii,
                 "DExpectTypeSsm");
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DExpectTypeSsm.cpp */
