/** @file init_numeric.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/subsys/Subsystem.hpp>

namespace xo {
    /* tag to represent the xo-numeric/ subsystem within ordered initialization */
    enum S_numeric_tag {};

    template <>
    struct InitSubsys<S_numeric_tag> {
        static void init();
        static InitEvidence require();
    };
}

/* end init_numeric.hpp */
