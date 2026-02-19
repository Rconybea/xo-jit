/** @file NumericDispatch.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "NumericDispatch.hpp"
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::AGCObject;

    namespace scm {

        void
        NumericDispatch::visit_pools(const MemorySizeVisitor & visitor)
        {
            dispatch_.visit_pools(visitor);
        }

        obj<AGCObject>
        NumericDispatch::multiply(obj<ARuntimeContext> rcx,
                                  obj<AGCObject> x,
                                  obj<AGCObject> y)
        {
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].multiply_;

            if (!target_fn)
                assert(false);

            return (*target_fn)(rcx, x.data(), y.data());
        }

        obj<AGCObject>
        NumericDispatch::divide(obj<ARuntimeContext> rcx,
                                obj<AGCObject> x,
                                obj<AGCObject> y)
        {
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].divide_;

            if (!target_fn)
                assert(false);

            return (*target_fn)(rcx, x.data(), y.data());
        }

        obj<AGCObject>
        NumericDispatch::add(obj<ARuntimeContext> rcx,
                             obj<AGCObject> x,
                             obj<AGCObject> y)
        {
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].add_;

            if (!target_fn)
                assert(false);

            return (*target_fn)(rcx, x.data(), y.data());
        }

        obj<AGCObject>
        NumericDispatch::subtract(obj<ARuntimeContext> rcx,
                                  obj<AGCObject> x,
                                  obj<AGCObject> y)
        {
            KeyType key(x._typeseq(), y._typeseq());

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].subtract_;

            if (!target_fn)
                assert(false);

            return (*target_fn)(rcx, x.data(), y.data());
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end NumericDispatch.cpp **/
