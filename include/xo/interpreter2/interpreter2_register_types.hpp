/** @file interpreter2_register_types.hpp
 *
 *  @author Roland Conybeare, Dec 2025
 **/

#pragma once

#include <xo/gc/Collector.hpp>

namespace xo {
    namespace scm {
        /** Register interpreter2 (facet,impl) combinations with FacetRegistry **/
        bool interpreter2_register_types(obj<xo::mm::ACollector> gc);
    }
}

/* end interpreter2_register_types.hpp */
