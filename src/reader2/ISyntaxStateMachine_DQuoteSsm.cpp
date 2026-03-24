/** @file ISyntaxStateMachine_DQuoteSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DQuoteSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DQuoteSsm.json5]
**/

#include "ssm/ISyntaxStateMachine_DQuoteSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DQuoteSsm::ssm_type(const DQuoteSsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DQuoteSsm::get_expect_str(const DQuoteSsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DQuoteSsm::on_token(DQuoteSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::on_parsed_symbol(DQuoteSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::on_parsed_typedescr(DQuoteSsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::on_parsed_type(DQuoteSsm & self, obj<AType> type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_type(type, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::on_parsed_formal(DQuoteSsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal(param_name, param_type, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::on_parsed_formal_with_token(DQuoteSsm & self, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_with_token(param_name, param_type, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::on_parsed_formal_arglist(DQuoteSsm & self, DArray * arglist, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_arglist(arglist, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::on_parsed_expression(DQuoteSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::on_parsed_expression_with_token(DQuoteSsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_token(expr, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::on_quoted_literal(DQuoteSsm & self, obj<AGCObject> lit, ParserStateMachine * p_psm) -> void
        {
            self.on_quoted_literal(lit, p_psm);
        }
        auto
        ISyntaxStateMachine_DQuoteSsm::forward_children(DQuoteSsm & self, obj<ACollector> gc) -> void
        {
            self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DQuoteSsm.cpp */
