/** @file ISyntaxStateMachine_DExpectQListSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectQListSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectQListSsm.json5]
**/

#include "ssm/ISyntaxStateMachine_DExpectQListSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DExpectQListSsm::ssm_type(const DExpectQListSsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DExpectQListSsm::get_expect_str(const DExpectQListSsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DExpectQListSsm::on_token(DExpectQListSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::on_parsed_symbol(DExpectQListSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::on_parsed_typedescr(DExpectQListSsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::on_parsed_type(DExpectQListSsm & self, obj<AType> type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_type(type, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::on_parsed_formal(DExpectQListSsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal(param_name, param_type, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::on_parsed_formal_with_token(DExpectQListSsm & self, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_with_token(param_name, param_type, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::on_parsed_formal_arglist(DExpectQListSsm & self, DArray * arglist, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_arglist(arglist, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::on_parsed_expression(DExpectQListSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::on_parsed_expression_with_token(DExpectQListSsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_token(expr, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::on_quoted_literal(DExpectQListSsm & self, obj<AGCObject> lit, ParserStateMachine * p_psm) -> void
        {
            self.on_quoted_literal(lit, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQListSsm::visit_gco_children(DExpectQListSsm & self, obj<AGCObjectVisitor> gc) -> void
        {
            self.visit_gco_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DExpectQListSsm.cpp */
