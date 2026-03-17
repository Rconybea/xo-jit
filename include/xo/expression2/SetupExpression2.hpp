/** @file SetupExpression2.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/alloc2/Collector.hpp>

namespace xo {
    namespace scm {
        class SetupExpression2 {
        public:
            using ACollector = xo::mm::ACollector;

        public:
            /** Register expression2 (facet,impl) combinations with FacetRegistry **/
            static bool register_facets();
            /** Register expression2 (facet,impl) combinations with FacetRegistry **/
            static bool register_types(obj<ACollector> gc);
        };
    }
}

/* end SetupExpression2.hpp */
