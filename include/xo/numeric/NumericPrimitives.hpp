/** @file NumericDispatch.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/procedure2/DPrimitive_gco_2_gco_gco.hpp>

namespace xo {
    namespace scm {

        /** @brief primitives using multidispatch
         **/
        class NumericPrimitives {
        public:
            /** poly divide **/
            static DPrimitive_gco_2_gco_gco s_div_gco_gco_pm;
        };
    }
}

/* end NumericDispatch.hpp */
