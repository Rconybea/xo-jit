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
        DExpectTypeSsm::start(DArena & mm,
        //obj<AAllocator> expr_mm,
                          ParserStateMachine * p_psm)
        {
            DExpectTypeSsm * expect_type_ssm = DExpectTypeSsm::make(mm);

            auto ssm
                = with_facet<ASyntaxStateMachine>::mkobj(expect_type_ssm);

            p_psm->push_ssm(ssm);
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
        DExpectTypeSsm::on_def_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectTypeSsm",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectTypeSsm::on_if_token(const Token & tk,
                                    ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DxpectTypeSsm",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectTypeSsm::on_colon_token(const Token & tk,
                                       ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DxpectTypeSsm",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectTypeSsm::on_singleassign_token(const Token & tk,
                                              ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectTypeSsm::on_singleassign_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectTypeSsm::on_f64_token(const Token & tk,
                                     ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectTypeSsm",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectTypeSsm::on_semicolon_token(const Token & tk,
                                           ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_token("DExpectTypeSsm::on_semicolon_token",
                                          tk,
                                          this->get_expect_str());
        }

        void
        DExpectTypeSsm::on_symbol_token(const Token & tk,
                                        ParserStateMachine * p_psm)
        {
            scope log(XO_DEBUG(true));

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
                p_psm->illegal_input_on_token("DExpectTypeSsm",
                                              tk,
                                              this->get_expect_str());
            }

            p_psm->pop_ssm();
            p_psm->on_parsed_typedescr(td);
        }

        void
        DExpectTypeSsm::on_parsed_symbol(std::string_view sym,
                                         ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_symbol("ExpectTypeSsm",
                                           sym,
                                           this->get_expect_str());
        }

        void
        DExpectTypeSsm::on_parsed_typedescr(TypeDescr td,
                                            ParserStateMachine * p_psm)
        {
            p_psm->illegal_input_on_typedescr("ExpectTypeSsm",
                                              td,
                                              this->get_expect_str());
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
