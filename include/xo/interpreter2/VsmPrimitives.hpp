/** @file VsmPrimitives.hpp
 *
 *  @author Roland Conybeare, Mar 2026
 **/

#pragma once

#include <xo/procedure2/Primitive_gco_0.hpp>

namespace xo {
    namespace scm {
        /** @brief primitives centered on interpreter2/ data
         *
         **/
        class VsmPrimitives {
        public:
            using AAllocator = xo::mm::AAllocator;

        public:
            /** create primitive: report memory use to console **/
            static DPrimitive_gco_0 * make_report_memory_use_pm(obj<AAllocator> mm);
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end VsmPrimitives.hpp */
