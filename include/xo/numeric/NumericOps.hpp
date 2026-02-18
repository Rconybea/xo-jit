/** @file NumericOps.hpp
*
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "Numeric.hpp"

namespace xo {
    namespace scm {

        class INumericOps {
        public:
            using BinaryOp1 = obj<ANumeric> (*)(obj<AAllocator> mm, void * x, void * y);

        public:
            explicit INumericOp9s(BinaryOp1 multiply) : multiply_{multiply} {}

            /** multiply (x,y); allocate from mm **/
            BinaryOp1 multiply_;
        };

        /** Convenience template. To use, provide implementation
         *  for
         *    _multiply() ...
         *  
         **/
        template <typename DRepr1, typename DRepr2>
        class NumericOps : public INumericOps {
        public:
            using BinaryOp1_Impl = obj<ANumeric> (*)(obj<AAllocator> mm, DRepr1 * x, DRepr2 * y);

        public:
            explicit NumericOps(BinaryOp1_Impl multiply)
                    : INumericOps(reinterpret_cast<INumericOps::BinaryOp1>(multiply))
            {}
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end NumericOps.hpp */

