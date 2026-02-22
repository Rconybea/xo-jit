/** @file NumericDispatch.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "NumericDispatch.hpp"
#include <xo/object2/RuntimeError.hpp>
#include <xo/facet/TypeRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::AGCObject;
    using xo::facet::TypeRegistry;

    namespace scm {

        void
        NumericDispatch::visit_pools(const MemorySizeVisitor & visitor)
        {
            dispatch_.visit_pools(visitor);
        }

        obj<AGCObject>
        NumericDispatch::dispatch(obj<ARuntimeContext> rcx,
                                  const char * caller,
                                  const char * error_headline,
                                  BinaryOp AnonymizedNumericOps::* member_ptr,
                                  obj<AGCObject> x,
                                  obj<AGCObject> y)
        {
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].*member_ptr;

            if (!target_fn) {
                // FIXME: use {fmt} here
                std::stringstream ss;
                tosn(ss,
                     error_headline,
                     xtag("x.type", TypeRegistry::id2name(x._typeseq())),
                     xtag("y.type", TypeRegistry::id2name(y._typeseq())));

                return DRuntimeError::make(rcx.allocator(),
                                           caller,
                                           ss.str().c_str());
            }

            return (*target_fn)(rcx, x.data(), y.data());
        }

        obj<AGCObject>
        NumericDispatch::multiply(obj<ARuntimeContext> rcx,
                                  obj<AGCObject> x,
                                  obj<AGCObject> y)
        {
            return dispatch(rcx,
                            "NumericDispatch::multiply",
                            "incomparable types in x*y",
                            &AnonymizedNumericOps::multiply_,
                            x, y);

#ifdef OBSOLETE
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].multiply_;

            if (!target_fn)
                assert(false);

            return (*target_fn)(rcx, x.data(), y.data());
#endif
        }

        obj<AGCObject>
        NumericDispatch::divide(obj<ARuntimeContext> rcx,
                                obj<AGCObject> x,
                                obj<AGCObject> y)
        {
            return dispatch(rcx,
                            "NumericDispatch::divide",
                            "incomparable types in x/y",
                            &AnonymizedNumericOps::divide_,
                            x, y);

#ifdef OBSOLETE
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].divide_;

            if (!target_fn)
                assert(false);

            return (*target_fn)(rcx, x.data(), y.data());
#endif
        }

        obj<AGCObject>
        NumericDispatch::add(obj<ARuntimeContext> rcx,
                             obj<AGCObject> x,
                             obj<AGCObject> y)
        {
            return dispatch(rcx,
                            "NumericDispatch::add",
                            "incomparable types in x+y",
                            &AnonymizedNumericOps::add_,
                            x, y);

#ifdef OBSOLETE
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].add_;

            if (!target_fn)
                assert(false);

            return (*target_fn)(rcx, x.data(), y.data());
#endif
        }

        obj<AGCObject>
        NumericDispatch::subtract(obj<ARuntimeContext> rcx,
                                  obj<AGCObject> x,
                                  obj<AGCObject> y)
        {
            return dispatch(rcx,
                            "NumericDispatch::subtract",
                            "incomparable types in x-y",
                            &AnonymizedNumericOps::subtract_,
                            x, y);

#ifdef OBSOLETE
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].subtract_;

            if (!target_fn)
                assert(false);

            return (*target_fn)(rcx, x.data(), y.data());
#endif
        }

        obj<AGCObject>
        NumericDispatch::cmp_equal(obj<ARuntimeContext> rcx,
                                   obj<AGCObject> x,
                                   obj<AGCObject> y)
        {
            return dispatch(rcx,
                            "NumericDispatch::cmp_equal",
                            "incomparable types in x==y",
                            &AnonymizedNumericOps::cmpeq_,
                            x, y);

#ifdef OBSOLETE
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].cmpeq_;

            if (!target_fn) {
                // FIXME: use {fmt} here
                std::stringstream ss;
                tosn(ss,
                     "incomparable types in x==y",
                     xtag("x.type", TypeRegistry::id2name(x._typeseq())),
                     xtag("y.type", TypeRegistry::id2name(y._typeseq())));

                return DRuntimeError::make(rcx.allocator(),
                                           "NumericDispatch::cmp_equal",
                                           ss.str().c_str());
            }

            return (*target_fn)(rcx, x.data(), y.data());
#endif
        }

        obj<AGCObject>
        NumericDispatch::cmp_notequal(obj<ARuntimeContext> rcx,
                                      obj<AGCObject> x,
                                      obj<AGCObject> y)
        {
            return dispatch(rcx,
                            "NumericDispatch::cmp_notequal",
                            "incomparable types in x!=y",
                            &AnonymizedNumericOps::cmpne_,
                            x, y);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end NumericDispatch.cpp **/
