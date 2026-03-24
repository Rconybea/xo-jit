/** @file ISyntaxStateMachine_DExpectListTypeSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectListTypeSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectListTypeSsm.json5]
**/

#include "expect_listtype/ISyntaxStateMachine_DExpectListTypeSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::ssm_type(const DExpectListTypeSsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DExpectListTypeSsm::get_expect_str(const DExpectListTypeSsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_token(DExpectListTypeSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_parsed_symbol(DExpectListTypeSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_parsed_typedescr(DExpectListTypeSsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_parsed_type(DExpectListTypeSsm & self, obj<AType> type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_type(type, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_parsed_formal(DExpectListTypeSsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal(param_name, param_type, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_parsed_formal_with_token(DExpectListTypeSsm & self, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_with_token(param_name, param_type, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_parsed_formal_arglist(DExpectListTypeSsm & self, DArray * arglist, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_arglist(arglist, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_parsed_expression(DExpectListTypeSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_parsed_expression_with_token(DExpectListTypeSsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_token(expr, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::on_quoted_literal(DExpectListTypeSsm & self, obj<AGCObject> lit, ParserStateMachine * p_psm) -> void
        {
            self.on_quoted_literal(lit, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectListTypeSsm::forward_children(DExpectListTypeSsm & self, obj<ACollector> gc) -> void
        {
            self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DExpectListTypeSsm.cpp */
