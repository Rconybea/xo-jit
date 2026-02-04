/** @file DVsmApplyFrame.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmApplyFrame.hpp"
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    using xo::facet::typeseq;

    namespace scm {

        DVsmApplyFrame::DVsmApplyFrame(obj<AGCObject> old_parent,
                                       VsmInstr old_cont,
                                       DArray * args)
        : VsmFrame(old_parent, old_cont),
          args_{args}
        {}

        DVsmApplyFrame *
        DVsmApplyFrame::make(obj<AAllocator> mm,
                             obj<AGCObject> old_parent,
                             VsmInstr old_cont,
                             DArray * args)
        {
            DVsmApplyFrame * result = nullptr;

            void * mem = mm.alloc(typeseq::id<DVsmApplyFrame>(),
                                  sizeof(DVsmApplyFrame));

            result = new (mem) DVsmApplyFrame(old_parent,
                                              old_cont,
                                              args);

            assert(result);

            return result;
        }

        std::size_t
        DVsmApplyFrame::shallow_size() const noexcept
        {
            return sizeof(DVsmApplyFrame);
        }

        DVsmApplyFrame *
        DVsmApplyFrame::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            DVsmApplyFrame * copy = (DVsmApplyFrame *)mm.alloc_copy((std::byte *)this);

            if (copy)
                *copy = *this;

            return copy;
        }

        std::size_t
        DVsmApplyFrame::forward_children(obj<ACollector> gc) noexcept
        {
            // GC needs to locate AGCObject iface for each member

            (void)gc;

            return this->shallow_size();
        }

        bool
        DVsmApplyFrame::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                (ppii,
                 "DVsmApplyFrame",
                 refrtag("cont", cont_),
                 refrtag("n_args", args_->size())
                    );
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmApplyFrame.cpp */
