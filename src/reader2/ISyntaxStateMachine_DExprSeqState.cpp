/** @file ISyntaxStateMachine_DExprSeqState.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/home/roland/proj/xo-umbrella2-claude1/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExprSeqState.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExprSeqState.json5]
**/

#include "ssm/ISyntaxStateMachine_DExprSeqState.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DExprSeqState::ssm_type(const DExprSeqState & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DExprSeqState::get_expect_str(const DExprSeqState & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DExprSeqState::on_symbol_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_symbol_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_def_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_def_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_if_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_if_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_colon_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_colon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_singleassign_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_singleassign_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_f64_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_f64_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_semicolon_token(DExprSeqState & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_semicolon_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_parsed_symbol(DExprSeqState & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_parsed_typedescr(DExprSeqState & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_parsed_expression(DExprSeqState & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DExprSeqState::on_parsed_expression_with_semicolon(DExprSeqState & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_semicolon(expr, p_psm);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DExprSeqState.cpp */