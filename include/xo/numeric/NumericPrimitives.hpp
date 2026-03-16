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
            using AAllocator = xo::mm::AAllocator;

        public:
            /** polymorphic (in both arguments1) multiply **/
            static DPrimitive_gco_2_gco_gco * make_multiply_pm(obj<AAllocator> mm,
                                                               StringTable * stbl);
            /** polymorphic (in both arguments) divide **/
            static DPrimitive_gco_2_gco_gco * make_divide_pm(obj<AAllocator> mm,
                                                             StringTable * stbl);
            /** polymorphic (in both arguments) add **/
            static DPrimitive_gco_2_gco_gco * make_add_pm(obj<AAllocator> mm,
                                                          StringTable * stbl);
            /** polymorphic (in both arguments) subtract **/
            static DPrimitive_gco_2_gco_gco * make_subtract_pm(obj<AAllocator> mm,
                                                               StringTable * stbl);

            /** polymorphic (in both arguments) compare (==) **/
            static DPrimitive_gco_2_gco_gco * make_cmpeq_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);
            /** polymorphic (in both arguments) compare (!=) **/
            static DPrimitive_gco_2_gco_gco * make_cmpne_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);
            /** polymorphic (in both arguments) compare (<) **/
            static DPrimitive_gco_2_gco_gco * make_cmplt_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);
            /** polymorphic (in both arguments) compare (<=) **/
            static DPrimitive_gco_2_gco_gco * make_cmple_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);
            /** polymorphic (in both arguments) compare (>) **/
            static DPrimitive_gco_2_gco_gco * make_cmpgt_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);
            /** polymorphic (in both arguments) compare (>=) **/
            static DPrimitive_gco_2_gco_gco * make_cmpge_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);
        };
    }
}

/* end NumericDispatch.hpp */
