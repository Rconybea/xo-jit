/** @file ISyntaxStateMachine_DDeftypeSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DDeftypeSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DDeftypeSsm.json5]
**/

#include "deftype/ISyntaxStateMachine_DDeftypeSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DDeftypeSsm::ssm_type(const DDeftypeSsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DDeftypeSsm::get_expect_str(const DDeftypeSsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DDeftypeSsm::on_token(DDeftypeSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::on_parsed_symbol(DDeftypeSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::on_parsed_typedescr(DDeftypeSsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::on_parsed_type(DDeftypeSsm & self, obj<AType> type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_type(type, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::on_parsed_formal(DDeftypeSsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal(param_name, param_type, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::on_parsed_formal_with_token(DDeftypeSsm & self, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_with_token(param_name, param_type, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::on_parsed_formal_arglist(DDeftypeSsm & self, DArray * arglist, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_arglist(arglist, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::on_parsed_expression(DDeftypeSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::on_parsed_expression_with_token(DDeftypeSsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_token(expr, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::on_quoted_literal(DDeftypeSsm & self, obj<AGCObject> lit, ParserStateMachine * p_psm) -> void
        {
            self.on_quoted_literal(lit, p_psm);
        }
        auto
        ISyntaxStateMachine_DDeftypeSsm::visit_gco_children(DDeftypeSsm & self, obj<AGCObjectVisitor> gc) -> void
        {
            self.visit_gco_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DDeftypeSsm.cpp */
