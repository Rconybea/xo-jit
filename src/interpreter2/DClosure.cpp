/** @file DClosure.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "Closure.hpp"
#include "LambdaExpr.hpp"
#include "LocalEnv.hpp"
#include "VsmRcx.hpp"
#include <xo/object2/RuntimeError.hpp>
#include <xo/indentlog/scope.hpp>
#include <cstddef>

namespace xo {
    using xo::mm::AGCObject;
    using xo::print::APrintable;

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
            // control here only if you try to invoke a closure
            // as a procedure.
            //
            // May support this later, but requires
            // nesting VSM (because call consumes c++ stack)
            //
            // typically prefer trampoline built into VSM

            (void)args;

            scope log(XO_DEBUG(true));

            auto vsm_rcx
                = obj<ARuntimeContext,DVsmRcx>::from(rcx);

            log && log(xtag("vsm_rcx.data", (void*)vsm_rcx.data()));

            auto err_mm
                = vsm_rcx->error_allocator();

            auto err
                = DRuntimeError::make(err_mm,
                                      "DClosure::apply_nocheck",
                                      "not implemented");
            return err;
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

        // ----- printable facet -----

        bool
        DClosure::pretty(const ppindentinfo & ppii) const
        {
            obj<APrintable,DLambdaExpr> lambda_pr(const_cast<DLambdaExpr *>(lambda_));
            obj<APrintable,DLocalEnv> env_pr(const_cast<DLocalEnv *>(env_));

            bool lambda_present = lambda_pr;
            bool env_present = env_pr;

            return ppii.pps()->pretty_struct
                (ppii,
                 "DClosure",
                 refrtag("lambda", lambda_pr, lambda_present),
                 refrtag("env", env_pr, env_present));
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DClosure.cpp */
