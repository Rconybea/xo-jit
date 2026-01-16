/** @file expression2_register_types.hpp
 *
 *  @author Roland Conybeare, Dec 2025
 **/

#pragma once

#include <xo/gc/Collector.hpp>

namespace xo {
    namespace scm {
        /** Register expression2 (facet,impl) combinations with FacetRegistry **/
        bool expression2_register_types(obj<xo::mm::ACollector> gc);
    }
}

/* end expression2_register_types.hpp */
