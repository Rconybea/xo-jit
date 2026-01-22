/** @file ISyntaxStateMachine_DProgressSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DProgressSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DProgressSsm.json5]
**/

#include "ssm/ISyntaxStateMachine_DProgressSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DProgressSsm::ssm_type(const DProgressSsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DProgressSsm::get_expect_str(const DProgressSsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DProgressSsm::on_symbol_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_symbol_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DProgressSsm::on_def_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_def_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DProgressSsm::on_if_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_if_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DProgressSsm::on_colon_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_colon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DProgressSsm::on_singleassign_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_singleassign_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DProgressSsm::on_f64_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_f64_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DProgressSsm::on_semicolon_token(DProgressSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_semicolon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DProgressSsm::on_parsed_symbol(DProgressSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DProgressSsm::on_parsed_typedescr(DProgressSsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DProgressSsm.cpp */