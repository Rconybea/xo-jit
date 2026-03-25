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
            /** name for multiply primitive. Used for op* **/
            static constexpr const char * c_multiply_pm_name = "_mul";
            /** polymorphic (in both arguments1) multiply **/
            static DPrimitive_gco_2_gco_gco * make_multiply_pm(obj<AAllocator> mm,
                                                               StringTable * stbl);

            /** name for divide primitives. Used for op/ **/
            static constexpr const char * c_divide_pm_name = "_div";
            /** polymorphic (in both arguments) divide **/
            static DPrimitive_gco_2_gco_gco * make_divide_pm(obj<AAllocator> mm,
                                                             StringTable * stbl);

            /** name for add primitives. Used for op+ **/
            static constexpr const char * c_add_pm_name = "_add";
            /** polymorphic (in both arguments) add **/
            static DPrimitive_gco_2_gco_gco * make_add_pm(obj<AAllocator> mm,
                                                          StringTable * stbl);

            /** name for sub primitives. Used for op- **/
            static constexpr const char * c_sub_pm_name = "_sub";
            /** polymorphic (in both arguments) subtract **/
            static DPrimitive_gco_2_gco_gco * make_subtract_pm(obj<AAllocator> mm,
                                                               StringTable * stbl);

            /** name for equality-comparison primitive. Used for op== **/
            static constexpr const char * c_cmpeq_pm_name = "_cmpeq";
            /** polymorphic (in both arguments) compare (==) **/
            static DPrimitive_gco_2_gco_gco * make_cmpeq_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);

            /** name for inequality-comparison prmitive. Used for op!= **/
            static constexpr const char * c_cmpne_pm_name = "_cmpne";
            /** polymorphic (in both arguments) compare (!=) **/
            static DPrimitive_gco_2_gco_gco * make_cmpne_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);

            /** name for less-comparison primitive.  Used for op< **/
            static constexpr const char * c_cmplt_pm_name = "_cmplt";
            /** polymorphic (in both arguments) compare (<) **/
            static DPrimitive_gco_2_gco_gco * make_cmplt_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);

            /** name for lesser-or-equal-comparison primitive.  Used for op<= **/
            static constexpr const char * c_cmple_pm_name = "_cmple";
            /** polymorphic (in both arguments) compare (<=) **/
            static DPrimitive_gco_2_gco_gco * make_cmple_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);

            /** name for greater-comparison primitive.  Used for op> **/
            static constexpr const char * c_cmpgt_pm_name = "_cmpgt";
            /** polymorphic (in both arguments) compare (>) **/
            static DPrimitive_gco_2_gco_gco * make_cmpgt_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);

            /** name for greater-or-equal-comparison primitive.  Used for op>= **/
            static constexpr const char * c_cmpge_pm_name = "_cmpge";
            /** polymorphic (in both arguments) compare (>=) **/
            static DPrimitive_gco_2_gco_gco * make_cmpge_pm(obj<AAllocator> mm,
                                                            StringTable * stbl);
        };
    }
}

/* end NumericDispatch.hpp */
