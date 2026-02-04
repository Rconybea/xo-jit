/** @file DVsmEvalArgsFrame.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmEvalArgsFrame.hpp"
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    using xo::facet::typeseq;
    using xo::print::ppindentinfo;

    namespace scm {

        // ----- VsmEvalArgsFrame -----

        DVsmEvalArgsFrame::DVsmEvalArgsFrame(obj<AGCObject> old_parent,
                                             VsmInstr old_cont)
        : VsmFrame(old_parent, old_cont)
        {}

        DVsmEvalArgsFrame *
        DVsmEvalArgsFrame::make(obj<AAllocator> mm,
                                obj<AGCObject> apply_frame,
                                VsmInstr cont)
        {
            DVsmEvalArgsFrame * result = nullptr;

            void * mem = mm.alloc(typeseq::id<DVsmEvalArgsFrame>(),
                                  sizeof(DVsmEvalArgsFrame));

            result = new (mem) DVsmEvalArgsFrame(apply_frame, cont);

            assert(result);

            return result;
        }

        std::size_t
        DVsmEvalArgsFrame::shallow_size() const noexcept
        {
            return sizeof(DVsmEvalArgsFrame);
        }

        DVsmEvalArgsFrame *
        DVsmEvalArgsFrame::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            DVsmEvalArgsFrame * copy = (DVsmEvalArgsFrame *)mm.alloc_copy((std::byte *)this);

            if (copy)
                *copy = *this;

            return copy;
        }

        std::size_t
        DVsmEvalArgsFrame::forward_children(obj<ACollector> gc) noexcept
        {
            (void)gc;

            return this->shallow_size();
        }

        bool
        DVsmEvalArgsFrame::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                (ppii,
                 "DVsmEvalArgsFrame",
                 refrtag("cont", cont_),
                 refrtag("i_arg", i_arg_)
                    );
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmEvalArgsFrame.cpp */
