/** @file DLocalEnv.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DLocalEnv.hpp"
#include <xo/reflectutil/typeseq.hpp>

namespace xo {
    using xo::mm::AGCObject;
    using xo::reflect::typeseq;

    namespace scm {

        DLocalEnv::DLocalEnv(DLocalEnv * parent,
                             DLocalSymtab * symtab,
                             DArray * args)
        : parent_{parent},
          symtab_{symtab},
          args_{args}
        {}

        DLocalEnv *
        DLocalEnv::_make_empty(obj<AAllocator> mm,
                               DLocalEnv * parent,
                               DLocalSymtab * symtab,
                               DArray * args)
        {
            assert(symtab);

            void * mem = mm.alloc_for<DLocalEnv>();

            return new (mem) DLocalEnv(parent, symtab, args);
        }

        obj<AGCObject>
        DLocalEnv::lookup_value(Binding ix) const noexcept
        {
            assert(!ix.is_global());

            const DLocalEnv * env = this;

            for (auto i = ix.i_link(); i > 0; --i) {
                env = env->parent();
            }

            if (env) {
                auto j = ix.j_slot();

                if (j < static_cast<decltype(j)>(env->size())) {
                    return (*(env->args_))[j];
                } else {
                    assert(false);
                }
            } else {
                assert(false);
            }

            /* something terribly wrong if control here */
            return obj<AGCObject>();
        }

        void
        DLocalEnv::assign_value(Binding ix, obj<AGCObject> x)
        {
            assert(!ix.is_global());

            const DLocalEnv * env = this;

            for (auto i = ix.i_link(); i > 0; --i) {
                env = env->parent();
            }

            if (env) {
                auto j = ix.j_slot();

                if (j < static_cast<decltype(j)>(env->size())) {
                    (*(env->args_))[j] = x;
                } else {
                    assert(false);
                }
            } else {
                assert(false);
            }

            /* something terribly wrong if control here */
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalEnv.cpp */
