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
            /** polymorphic (in both arguments) multiply **/
            static DPrimitive_gco_2_gco_gco s_mul_gco_gco_pm;
            /** polymorphic (in both arguments) divide **/
            static DPrimitive_gco_2_gco_gco s_div_gco_gco_pm;
            /** polymorphic (in both arguments) add **/
            static DPrimitive_gco_2_gco_gco s_add_gco_gco_pm;
            /** polymorphic (in both arguments) subtract **/
            static DPrimitive_gco_2_gco_gco s_sub_gco_gco_pm;

            /** polymorphic (in both arguments) compare (==) **/
            static DPrimitive_gco_2_gco_gco s_cmpeq_gco_gco_pm;
        };
    }
}

/* end NumericDispatch.hpp */
