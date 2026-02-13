/** @file DLocalEnv.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "LocalEnv.hpp"
#include <xo/object2/Array.hpp>
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
        DLocalEnv::_make(obj<AAllocator> mm,
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

        std::size_t
        DLocalEnv::shallow_size() const noexcept {
            return sizeof(DLocalEnv);
        }

        DLocalEnv *
        DLocalEnv::shallow_copy(obj<AAllocator> mm) const noexcept {
            DLocalEnv * copy = (DLocalEnv *)mm.alloc_copy((std::byte *)this);

            if (copy)
                *copy = *this;

            return copy;
        }

        std::size_t
        DLocalEnv::forward_children(obj<ACollector> gc) noexcept
        {
            {
                auto iface = xo::facet::impl_for<AGCObject,DLocalEnv>();
                gc.forward_inplace(&iface, (void **)(&parent_));
            }
            {
                auto iface = xo::facet::impl_for<AGCObject,DLocalSymtab>();
                gc.forward_inplace(&iface, (void **)(&symtab_));
            }
            {
                auto iface = xo::facet::impl_for<AGCObject,DArray>();
                gc.forward_inplace(&iface, (void **)(&args_));
            }

            return shallow_size();
        }

        // ----- printable facet -----

        bool
        DLocalEnv::pretty(const ppindentinfo & ppii) const noexcept
        {
            // print local bindings, perhaps
            //   symtab_
            //   args_

            return ppii.pps()->pretty_struct
                (ppii,
                 "DLocalEnv",
                 refrtag("n_args", args_->size())
                    );
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalEnv.cpp */
