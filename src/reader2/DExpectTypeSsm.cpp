/** @file DExpectTypeSsm.cpp
 *
 *  @author Roland Conybeare, Aug 2024
 **/

#include "ExpectTypeSsm.hpp"
#include "ExpectListTypeSsm.hpp"
#include "SyntaxStateMachine.hpp"
#include <xo/type/AtomicType.hpp>
#include <xo/reflect/Reflect.hpp>
#include <xo/facet/facet_implementation.hpp>
#include <xo/reflectutil/typeseq.hpp>
#include <xo/indentlog/print/pretty.hpp>
#include <string_view>

namespace xo {
    using xo::facet::with_facet;
    using xo::reflect::Reflect;
    using xo::reflect::TypeDescr;
    using xo::reflect::typeseq;

    namespace scm {
        DExpectTypeSsm::DExpectTypeSsm(bool corrected)
            : corrected_{corrected}
        {}

        DExpectTypeSsm *
        DExpectTypeSsm::_make(DArena & mm, bool corrected)
        {
            void * mem = mm.alloc(typeseq::id<DArena>(),
                                  sizeof(DArena));

            return new (mem) DExpectTypeSsm(corrected);
        }

        obj<ASyntaxStateMachine,DExpectTypeSsm>
        DExpectTypeSsm::make(DArena & mm, bool corrected)
        {
            return obj<ASyntaxStateMachine,DExpectTypeSsm>(_make(mm, corrected));
        }

        void
        DExpectTypeSsm::start(bool corrected,
                              ParserStateMachine * p_psm)
        {
            DArena::Checkpoint ckp = p_psm->parser_alloc().checkpoint();

            auto ssm = DExpectTypeSsm::make(p_psm->parser_alloc(), corrected);

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
            case tokentype::tk_deftype:
            case tokentype::tk_if:
            case tokentype::tk_f64:
            case tokentype::tk_i64:
            case tokentype::tk_bool:
            case tokentype::tk_string:
            case tokentype::tk_quote:
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
            case tokentype::tk_nil:
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

            if (corrected_) {
                obj<AType> type;
                obj mm = p_psm->expr_alloc();

                if (tk.text() == "list") {
                    /* replace top ssm with specialized list-type ssm parser */
                    p_psm->pop_ssm();
                    DExpectListTypeSsm::start(p_psm);
                    p_psm->on_token(tk);
                } else {
                    if (tk.text() == "unit")
                        type = DAtomicType::make(mm, Metatype::t_unit());
                    if (tk.text() == "bool")
                        type = DAtomicType::make(mm, Metatype::t_bool());
                    else if (tk.text() == "str")
                        type = DAtomicType::make(mm, Metatype::t_str());
                    else if (tk.text() == "f64")
                        type = DAtomicType::make(mm, Metatype::t_f64());
                    else if(tk.text() == "f32")
                        type = DAtomicType::make(mm, Metatype::t_f32());
                    else if(tk.text() == "i16")
                        type = DAtomicType::make(mm, Metatype::t_i16());
                    else if(tk.text() == "i32")
                        type = DAtomicType::make(mm, Metatype::t_i32());
                    else if(tk.text() == "i64")
                        type = DAtomicType::make(mm, Metatype::t_i64());

                    p_psm->pop_ssm();
                    p_psm->on_parsed_type(type);
                }
            } else {
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
