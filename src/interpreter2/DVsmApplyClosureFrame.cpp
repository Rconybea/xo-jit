/** @file DVsmApplyClosureFrame.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmApplyClosureFrame.hpp"
#include "LocalEnv.hpp"

namespace xo {
    using xo::mm::AGCObject;
    using xo::reflect::typeseq;

    namespace scm {

        DVsmApplyClosureFrame::DVsmApplyClosureFrame(obj<AGCObject> stack,
                                                     VsmInstr cont,
                                                     DLocalEnv * local_env)
            : stack_{stack},
              cont_{cont},
              local_env_{local_env}
        {}

        DVsmApplyClosureFrame *
        DVsmApplyClosureFrame::make(obj<AAllocator> mm,
                                    obj<AGCObject> stack,
                                    VsmInstr cont,
                                    DLocalEnv * local_env)
        {
            void * mem = mm.alloc(typeseq::id<DVsmApplyClosureFrame>(),
                                  sizeof(DVsmApplyClosureFrame));

            return new (mem) DVsmApplyClosureFrame(stack, cont, local_env);
        }

        DVsmApplyClosureFrame *
        DVsmApplyClosureFrame::shallow_move(obj<ACollector> gc) noexcept
        {
            return gc.std_move_for(this);
        }

        void
        DVsmApplyClosureFrame::visit_gco_children(obj<AGCObjectVisitor> gc) noexcept
        {
            gc.visit_child(&stack_);
            gc.visit_child(&local_env_);
        }

        bool
        DVsmApplyClosureFrame::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                (ppii,
                 "DVsmApplyClosureFrame",
                 refrtag("cont", cont_),
                 refrtag("env", local_env_));
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmApplyClosureFrame.cpp */
