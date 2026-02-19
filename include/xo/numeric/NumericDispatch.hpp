/** @file NumericDispatch.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "NumericOps.hpp"
#include <xo/procedure2/RuntimeContext.hpp>
#include <xo/gc/GCObject.hpp>
#include <xo/arena/DArenaHashMap.hpp>
#include <xo/reflectutil/typeseq.hpp>

namespace xo {
    namespace scm {

        /** Runtime polymoprhic multimethod dispatch.
         *  Hash on argument types to get function table
         **/
        class NumericDispatch {
        public:
            //using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;
            using typeseq = xo::reflect::typeseq;
            using KeyType = std::pair<typeseq, typeseq>;
            using MappedType = AnonymizedNumericOps;

            /** hash function for key_type **/
            struct KeyHash {

                std::size_t operator()(const KeyType & k) const noexcept {
                    // combine the two seqno values
                    std::size_t h1 = std::hash<int32_t>{}(k.first.seqno());
                    std::size_t h2 = std::hash<int32_t>{}(k.second.seqno());

                    return h1 ^ (h2 << 7);
                }

            };

            using MapType = xo::map::DArenaHashMap<KeyType,
                                                   MappedType,
                                                   KeyHash,
                                                   std::equal_to<void>>;

        public:
            NumericDispatch(uint32_t hint_max_capacity)
                : dispatch_{"numeric-dispatch",
                hint_max_capacity,
                false /*!debug_flag*/} {}

            /** @p hint_max_capacity only honored the first time instance()
             *  is called.
             **/
            static NumericDispatch & instance(uint32_t hint_max_capacity = 64) {
                static NumericDispatch s_instance(hint_max_capacity);
                return s_instance;
            }

            /** multiply w/ runtime polymorphism (double-dispatch)
             **/
            static obj<AGCObject> multiply(obj<ARuntimeContext> rcx,
                                           obj<AGCObject> x,
                                           obj<AGCObject> y);

            /** divide w/ runtime polymorphism (double-dispatch)
             **/
            static obj<AGCObject> divide(obj<ARuntimeContext> rcx,
                                         obj<AGCObject> x,
                                         obj<AGCObject> y);

            /** add w/ runtime polymorphism (double-dispatch)
             **/
            static obj<AGCObject> add(obj<ARuntimeContext> rcx,
                                      obj<AGCObject> x,
                                      obj<AGCObject> y);

            /** subtract w/ runtime polymorphism (double-dispatch)
             **/
            static obj<AGCObject> subtract(obj<ARuntimeContext> rcx,
                                           obj<AGCObject> x,
                                           obj<AGCObject> y);

            /** compare two numeric values for equality **/
            static obj<AGCObject> cmp_equal(obj<ARuntimeContext> rcx,
                                            obj<AGCObject> x,
                                            obj<AGCObject> y);

            /** report memory use for owned arenas to @p visitor **/
            void visit_pools(const MemorySizeVisitor & visitor);

            /** Use:
             *  Need to have overload
             *    multiply(obj<AAllocator>, DRepr1, DRepr2) -> obj<ANumeric>
             *  available
             **/
            template <typename DRepr1, typename DRepr2>
            void register_impl(typename NumericOps<DRepr1, DRepr2>::BinaryOp_Impl mul_fn,
                               typename NumericOps<DRepr1, DRepr2>::BinaryOp_Impl div_fn,
                               typename NumericOps<DRepr1, DRepr2>::BinaryOp_Impl add_fn,
                               typename NumericOps<DRepr1, DRepr2>::BinaryOp_Impl sub_fn,
                               typename NumericOps<DRepr1, DRepr2>::BinaryOp_Impl cmpeq_fn) {

                KeyType key(typeseq::id<DRepr1>().seqno(),
                            typeseq::id<DRepr2>().seqno());

                // note: copying op table so they're in proximity
                this->dispatch_[key]
                    = NumericOps<DRepr1, DRepr2>::make(mul_fn,
                                                       div_fn,
                                                       add_fn,
                                                       sub_fn,
                                                       cmpeq_fn);
            }

        private:
            /** 2d dispatch for arithmetic **/
            MapType dispatch_;
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end NumericDispatch.hpp */
