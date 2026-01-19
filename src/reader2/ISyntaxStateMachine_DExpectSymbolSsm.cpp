/** @file ISyntaxStateMachine_DExpectSymbolSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
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
        ISyntaxStateMachine_DExpectSymbolSsm::on_parsed_symbol(DExpectSymbolSsm & self, std::string_view sym, ParserStateMachine * p_psm) -> void
        {
            self.on_parsed_symbol(sym, p_psm);
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

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DExpectSymbolSsm.cpp */