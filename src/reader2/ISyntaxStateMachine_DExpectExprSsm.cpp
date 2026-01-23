/** @file ISyntaxStateMachine_DExpectExprSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectExprSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectExprSsm.json5]
**/

#include "ssm/ISyntaxStateMachine_DExpectExprSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DExpectExprSsm::ssm_type(const DExpectExprSsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DExpectExprSsm::get_expect_str(const DExpectExprSsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DExpectExprSsm::on_symbol_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_symbol_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_def_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_def_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_if_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_if_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_colon_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_colon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_singleassign_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_singleassign_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_f64_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_f64_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_i64_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_i64_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_bool_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_bool_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_semicolon_token(DExpectExprSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_semicolon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_parsed_symbol(DExpectExprSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_parsed_typedescr(DExpectExprSsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_parsed_expression(DExpectExprSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectExprSsm::on_parsed_expression_with_semicolon(DExpectExprSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_semicolon(expr, p_psm);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DExpectExprSsm.cpp */