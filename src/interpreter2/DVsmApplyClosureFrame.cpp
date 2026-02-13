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

        std::size_t
        DVsmApplyClosureFrame::shallow_size() const noexcept
        {
            return sizeof(DVsmApplyClosureFrame);
        }

        DVsmApplyClosureFrame *
        DVsmApplyClosureFrame::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            DVsmApplyClosureFrame * copy
                = (DVsmApplyClosureFrame *)mm.alloc_copy((std::byte *)this);

            if (copy)
                *copy = *this;

            return copy;
        }

        std::size_t
        DVsmApplyClosureFrame::forward_children(obj<ACollector> gc) noexcept
        {
            gc.forward_inplace(&stack_);
            gc.forward_inplace(&local_env_);

            return this->shallow_size();
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
