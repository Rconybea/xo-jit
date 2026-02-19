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
                                          BinaryOp divide)
                : multiply_{multiply}, divide_{divide} {}

            BinaryOp multiply_ = nullptr;
            BinaryOp divide_ = nullptr;
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
                                             BinaryOp_Impl divide) {
                return AnonymizedNumericOps(reinterpret_cast<BinaryOp_Anon>(multiply),
                                            reinterpret_cast<BinaryOp_Anon>(divide));
            }
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end NumericOps.hpp */
