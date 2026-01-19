/** @file reader2_register_types.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/gc/Collector.hpp>

namespace xo {
    namespace scm {
        /** Register reader2 gc-aware types with collector **/
        bool reader2_register_types(obj<xo::mm::ACollector> gc);
    }
}

/* end reader2_register_types.hpp */
