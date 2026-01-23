/** @file ISyntaxStateMachine_DDefineSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DDefineSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DDefineSsm.json5]
**/

#include "ssm/ISyntaxStateMachine_DDefineSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DDefineSsm::ssm_type(const DDefineSsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DDefineSsm::get_expect_str(const DDefineSsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DDefineSsm::on_symbol_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_symbol_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_def_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_def_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_if_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_if_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_colon_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_colon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_singleassign_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_singleassign_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_f64_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_f64_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_i64_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_i64_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_bool_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_bool_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_string_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_string_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_semicolon_token(DDefineSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_semicolon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_parsed_symbol(DDefineSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_parsed_typedescr(DDefineSsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_parsed_expression(DDefineSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_parsed_expression_with_semicolon(DDefineSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_semicolon(expr, p_psm);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DDefineSsm.cpp */