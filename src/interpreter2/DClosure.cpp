/** @file DClosure.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "Closure.hpp"
#include "LambdaExpr.hpp"
#include "LocalEnv.hpp"
#include <cstddef>

namespace xo {
    using xo::mm::AGCObject;

    namespace scm {

        DClosure::DClosure(const DLambdaExpr * lm,
                           const DLocalEnv * env)
        : lambda_{lm}, env_{env}
        {}

        DClosure *
        DClosure::make(obj<AAllocator> mm,
                       const DLambdaExpr * lm,
                       const DLocalEnv * env)
        {
            void * mem = mm.alloc_for<DClosure>();

            return new (mem) DClosure(lm, env);
        }

        obj<AGCObject>
        DClosure::apply_nocheck(obj<ARuntimeContext> rcx,
                                const DArray * args)
        {
            (void)rcx;
            (void)args;

            assert(false);
        }

        size_t
        DClosure::shallow_size() const noexcept {
            return sizeof(DClosure);
        }

        DClosure *
        DClosure::shallow_copy(obj<AAllocator> mm) const noexcept {
            DClosure * copy = (DClosure *)mm.alloc_copy((std::byte *)this);

            if (copy)
                *copy = *this;

            return copy;
        }

        std::size_t
        DClosure::forward_children(obj<ACollector> gc) noexcept
        {
            {
                auto iface = xo::facet::impl_for<AGCObject,DLambdaExpr>();
                gc.forward_inplace(&iface, (void **)(&lambda_));

            }
            {
                auto iface = xo::facet::impl_for<AGCObject,DLocalEnv>();
                gc.forward_inplace(&iface, (void **)(&env_));
            }

            return shallow_size();
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DClosure.cpp */
