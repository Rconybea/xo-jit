/** @file init_expression2.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/subsys/Subsystem.hpp>

namespace xo {
    /* tag to represent the xo-expression2/ subsystem within ordered initialization */
    enum S_expression2_tag {};

    template <>
    struct InitSubsys<S_expression2_tag> {
        static void init();
        static InitEvidence require();
    };
} /*namespace xo*/

/* end init_expression2.hpp */
