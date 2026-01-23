/** @file ISyntaxStateMachine_DExpectSymbolSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectSymbolSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectSymbolSsm.json5]
**/

#include "ssm/ISyntaxStateMachine_DExpectSymbolSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::ssm_type(const DExpectSymbolSsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DExpectSymbolSsm::get_expect_str(const DExpectSymbolSsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_symbol_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_symbol_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_def_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_def_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_if_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_if_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_colon_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_colon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_singleassign_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_singleassign_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_f64_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_f64_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_bool_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_bool_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_semicolon_token(DExpectSymbolSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_semicolon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_parsed_symbol(DExpectSymbolSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_parsed_typedescr(DExpectSymbolSsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_parsed_expression(DExpectSymbolSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectSymbolSsm::on_parsed_expression_with_semicolon(DExpectSymbolSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_semicolon(expr, p_psm);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DExpectSymbolSsm.cpp */