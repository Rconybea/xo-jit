/** @file ISyntaxStateMachine_DApplySsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DApplySsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DApplySsm.json5]
**/

#include "ssm/ISyntaxStateMachine_DApplySsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DApplySsm::ssm_type(const DApplySsm & self) noexcept -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DApplySsm::get_expect_str(const DApplySsm & self) noexcept -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DApplySsm::on_token(DApplySsm & self, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_token(tk, p_psm);
        }
        auto
        ISyntaxStateMachine_DApplySsm::on_parsed_symbol(DApplySsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
        }
        auto
        ISyntaxStateMachine_DApplySsm::on_parsed_typedescr(DApplySsm & self, TypeDescr td, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_typedescr(td, p_psm);
        }
        auto
        ISyntaxStateMachine_DApplySsm::on_parsed_formal(DApplySsm & self, const DUniqueString * param_name, TypeDescr param_type, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal(param_name, param_type, p_psm);
        }
        auto
        ISyntaxStateMachine_DApplySsm::on_parsed_formal_arglist(DApplySsm & self, DArray * arglist, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_formal_arglist(arglist, p_psm);
        }
        auto
        ISyntaxStateMachine_DApplySsm::on_parsed_expression(DApplySsm & self, obj<AExpression> expr, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression(expr, p_psm);
        }
        auto
        ISyntaxStateMachine_DApplySsm::on_parsed_expression_with_token(DApplySsm & self, obj<AExpression> expr, const Token & tk, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_expression_with_token(expr, tk, p_psm);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DApplySsm.cpp */
