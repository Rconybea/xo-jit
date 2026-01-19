/** @file init_reader2.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/subsys/Subsystem.hpp>

namespace xo {
    /* tag to represent the xo-reader2/ subsystem within ordered initialization */
    enum S_reader2_tag {};

    template <>
    struct InitSubsys<S_reader2_tag> {
        static void init();
        static InitEvidence require();
    };
} /*namespace xo*/

/* end init_reader2.hpp */
