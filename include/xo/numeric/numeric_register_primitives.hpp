/** @file numeric_register_primitives.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include "PrimitiveRegistry.hpp"
#include <xo/alloc2/Collector.hpp>

namespace xo {
    namespace scm {
        /** Register gc-aware (AGCObject,DRepr) combinations with garbage collector @p gc **/
        bool numeric_register_primitives(obj<xo::mm::AAllocator> gc,
                                         InstallSink sink,
                                         InstallFlags flags);
    }
}

/* end numeric_register_primitives.hpp */
