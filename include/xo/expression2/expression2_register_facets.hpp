/** @file expression2_register_facets.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/gc/Collector.hpp>

namespace xo {
    namespace scm {
        /** Register expression2 (facet,impl) combinations with FacetRegistry **/
        bool expression2_register_facets();
    }
}

/* end expression2_register_facets.hpp */
