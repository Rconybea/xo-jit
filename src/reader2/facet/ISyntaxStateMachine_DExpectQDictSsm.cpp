/** @file ISyntaxStateMachine_DExpectQDictSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DExpectQDictSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DExpectQDictSsm.json5]
**/

#include "expect_qdict/ISyntaxStateMachine_DExpectQDictSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DExpectQDictSsm::ssm_type(const DExpectQDictSsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DExpectQDictSsm::get_expect_str(const DExpectQDictSsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_token(DExpectQDictSsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_parsed_symbol(DExpectQDictSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_parsed_typedescr(DExpectQDictSsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_parsed_type(DExpectQDictSsm & self, obj<AType> type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_type(type, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_parsed_formal(DExpectQDictSsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal(param_name, param_type, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_parsed_formal_with_token(DExpectQDictSsm & self, const DUniqueString * param_name, TypeDescr param_type, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_with_token(param_name, param_type, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_parsed_formal_arglist(DExpectQDictSsm & self, DArray * arglist, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_arglist(arglist, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_parsed_expression(DExpectQDictSsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_parsed_expression_with_token(DExpectQDictSsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_token(expr, tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::on_quoted_literal(DExpectQDictSsm & self, obj<AGCObject> lit, ParserStateMachine * p_psm) -> void
        {
            self.on_quoted_literal(lit, p_psm);
        }
        auto
        ISyntaxStateMachine_DExpectQDictSsm::forward_children(DExpectQDictSsm & self, obj<ACollector> gc) -> void
        {
            self.forward_children(gc);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DExpectQDictSsm.cpp */
