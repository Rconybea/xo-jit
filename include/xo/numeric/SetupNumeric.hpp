/** @file SetupNumeric.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "PrimitiveRegistry.hpp"
#include <xo/alloc2/Collector.hpp>

namespace xo {
    namespace scm {
        /** Setup numeric facet dispatch **/
        struct SetupNumeric {
        public:
            static bool register_facets();
            /** Register primitive factories with primitive registry **/
            static bool register_primitives(obj<ARuntimeContext> rcx,
                                            InstallSink sink,
                                            InstallFlags flags);
        };
    }
}

/* end SetupNumeric.hpp */
