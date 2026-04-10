/** @file DLocalEnv.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "LocalEnv.hpp"
#include <xo/object2/Array.hpp>
#include <xo/reflectutil/typeseq.hpp>
#include <xo/indentlog/scope.hpp>

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

                if (j < static_cast<decltype(j)>(env->n_vars())) {
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
            scope log(XO_DEBUG(true));

            assert(!ix.is_global());

            const DLocalEnv * env = this;

            for (auto i = ix.i_link(); i > 0; --i) {
                env = env->parent();
            }

            if (env) {
                auto j = ix.j_slot();

                if (j < static_cast<decltype(j)>(env->n_vars())) {
                    log && log("STUB: need write barrier for GC here");
                    (*(env->args_))[j] = x;
                } else {
                    assert(false);
                }
            } else {
                assert(false);
            }

            /* something terribly wrong if control here */
        }

        DLocalEnv *
        DLocalEnv::gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept {
            return gc.std_move_for(this);
        }

        void
        DLocalEnv::visit_gco_children(VisitReason reason,
                                      obj<AGCObjectVisitor> gc) noexcept
        {
            gc.visit_child(reason, &parent_);
            gc.visit_child(reason, &symtab_);
            gc.visit_child(reason, &args_);
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
