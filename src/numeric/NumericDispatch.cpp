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

            return (*target_fn)(rcx, x.data(), y.data());
        }

        obj<AGCObject>
        NumericDispatch::divide(obj<ARuntimeContext> rcx,
                                obj<AGCObject> x,
                                obj<AGCObject> y)
        {
            scope log(XO_DEBUG(true));

            KeyType key(x._typeseq(), y._typeseq());

            log && log(xtag("x.tseq", x._typeseq().seqno()),
                       xtag("y.tseq", y._typeseq().seqno()));

            auto target_fn
                = NumericDispatch::instance().dispatch_[key].divide_;

            log && log(xtag("target_fn", target_fn));

            assert(target_fn);

            return (*target_fn)(rcx, x.data(), y.data());
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end NumericDispatch.cpp **/
