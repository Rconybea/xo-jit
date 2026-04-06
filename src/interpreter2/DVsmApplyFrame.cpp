/** @file DVsmApplyFrame.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmApplyFrame.hpp"
#include <xo/object2/Array.hpp>
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    using xo::facet::typeseq;

    namespace scm {

        DVsmApplyFrame::DVsmApplyFrame(obj<AGCObject> old_parent,
                                       VsmInstr old_cont,
                                       DArray * args)
            : parent_{old_parent},
              cont_{old_cont},
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

        DVsmApplyFrame *
        DVsmApplyFrame::gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept
        {
            return gc.std_move_for(this);
        }

        void
        DVsmApplyFrame::visit_gco_children(obj<AGCObjectVisitor> gc) noexcept
        {
            gc.visit_child(&parent_);
            gc.visit_child(&fn_);
            gc.visit_child(&args_);
        }

        bool
        DVsmApplyFrame::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DVsmApplyFrame",
                                             refrtag("cont", cont_),
                                             refrtag("n_args", args_->size()));
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmApplyFrame.cpp */
