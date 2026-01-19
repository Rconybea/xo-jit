/** @file ISyntaxStateMachine_DDefineSsm.cpp
 *
 *  Generated automagically from ingredients:
 *  1. code generator:
 *       [/Users/roland/proj/xo-umbrella2/xo-facet/codegen/genfacet]
 *     arguments:
 *       --input [idl/ISyntaxStateMachine_DDefineSsm.json5]
 *  2. jinja2 template for abstract facet .hpp file:
 *       [iface_facet_any.hpp.j2]
 *  3. idl for facet methods
 *       [idl/ISyntaxStateMachine_DDefineSsm.json5]
**/

#include "ssm/ISyntaxStateMachine_DDefineSsm.hpp"

namespace xo {
    namespace scm {
        auto
        ISyntaxStateMachine_DDefineSsm::ssm_type(const DDefineSsm & self) noexcept
            -> syntaxstatetype
        {
            return self.ssm_type();
        }

        auto
        ISyntaxStateMachine_DDefineSsm::get_expect_str(const DDefineSsm & self) noexcept
            -> std::string_view
        {
            return self.get_expect_str();
        }

        auto
        ISyntaxStateMachine_DDefineSsm::on_def_token(DDefineSsm & self,
                                                     const Token & tk,
                                                     ParserStateMachine * ps_psm) -> void
        {
            self.on_def_token(tk, ps_psm);
        }
        auto
        ISyntaxStateMachine_DDefineSsm::on_if_token(DDefineSsm & self,
                                                    const Token & tk,
                                                    ParserStateMachine * p_psm) -> void
        {
            self.on_if_token(tk, p_psm);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end ISyntaxStateMachine_DDefineSsm.cpp */
