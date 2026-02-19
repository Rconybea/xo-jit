/** @file NumericOps.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/numeric/Numeric.hpp>
#include <xo/procedure2/RuntimeContext.hpp>
#include <xo/gc/GCObject.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        class AnonymizedNumericOps {
        public:
            using ARuntimeContext = xo::scm::ARuntimeContext;
            using AGCObject = xo::mm::AGCObject;
            using BinaryOp = obj<AGCObject> (*)(obj<ARuntimeContext> mm, void * x, void * y);

        public:
            /** note: null ctor load-bearing for membership in DArenaHashTable **/
            AnonymizedNumericOps() = default;
            /** @p multiply to multiply (x,y); allocate from mm **/
            explicit AnonymizedNumericOps(BinaryOp multiply,
                                          BinaryOp divide,
                                          BinaryOp add,
                                          BinaryOp subtract,
                                          BinaryOp cmpeq,
                                          BinaryOp cmpne)
                : multiply_{multiply}, divide_{divide}, add_{add}, subtract_{subtract},
                  cmpeq_{cmpeq}, cmpne_{cmpne} {}

            BinaryOp multiply_ = nullptr;
            BinaryOp divide_ = nullptr;
            BinaryOp add_ = nullptr;
            BinaryOp subtract_ = nullptr;

            /** compare numerics for equality **/
            BinaryOp cmpeq_ = nullptr;
            /** compare numerics for inequality **/
            BinaryOp cmpne_ = nullptr;
        };

        template <typename DRepr1, typename DRepr2>
        class NumericOps {
        public:
            using ARuntimeContext = xo::scm::ARuntimeContext;
            using AGCObject = xo::mm::AGCObject;
            using BinaryOp_Impl = obj<AGCObject> (*)(obj<ARuntimeContext> rcx, DRepr1 * x, DRepr2 * y);
            using BinaryOp_Anon = AnonymizedNumericOps::BinaryOp;

        public:
            static AnonymizedNumericOps make(BinaryOp_Impl multiply,
                                             BinaryOp_Impl divide,
                                             BinaryOp_Impl add,
                                             BinaryOp_Impl subtract,
                                             BinaryOp_Impl cmpeq,
                                             BinaryOp_Impl cmpne) {
                return AnonymizedNumericOps(reinterpret_cast<BinaryOp_Anon>(multiply),
                                            reinterpret_cast<BinaryOp_Anon>(divide),
                                            reinterpret_cast<BinaryOp_Anon>(add),
                                            reinterpret_cast<BinaryOp_Anon>(subtract),
                                            reinterpret_cast<BinaryOp_Anon>(cmpeq),
                                            reinterpret_cast<BinaryOp_Anon>(cmpne));
            }
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end NumericOps.hpp */
