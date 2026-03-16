/** @file SetupInterpreter2.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "PrimitiveRegistry.hpp"
#include <xo/alloc2/Collector.hpp>

namespace xo {
    namespace scm {
        struct SetupInterpreter2 {
        public:
            using ACollector = xo::mm::ACollector;

        public:
            static bool register_facets();
            static bool register_types(obj<ACollector> gc);
            static bool register_primitives(obj<ARuntimeContext> rcx,
                                            InstallSink sink,
                                            InstallFlags flags);
        };
    }
}

/* end SetupInterpreter2.hpp */
