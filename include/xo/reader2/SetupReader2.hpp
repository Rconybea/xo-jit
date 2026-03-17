/** @file SetupReader2.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/alloc2/Collector.hpp>

namespace xo {
    namespace scm {
        struct SetupReader2 {
        public:
            using ACollector = xo::mm::ACollector;

        public:
            /** Register reader2 (facet,impl) combinations with FacetRegistry **/
            static bool register_facets();
            /** Register object types with @p gc **/
            static bool register_types(obj<ACollector> gc);
        };
    }
}

/* end SetupReader2.hpp */
