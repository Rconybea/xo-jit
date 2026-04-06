/** @file DVsmEvalArgsFrame.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmEvalArgsFrame.hpp"
#include <xo/expression2/ApplyExpr.hpp>
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    using xo::facet::typeseq;
    using xo::print::ppindentinfo;

    namespace scm {

        // ----- VsmEvalArgsFrame -----

        DVsmEvalArgsFrame::DVsmEvalArgsFrame(DVsmApplyFrame * parent,
                                             VsmInstr cont,
                                             DApplyExpr * apply_expr)
        : parent_{parent},
          cont_{cont},
          apply_expr_{apply_expr}
        {}

        DVsmEvalArgsFrame *
        DVsmEvalArgsFrame::make(obj<AAllocator> mm,
                                DVsmApplyFrame * apply_frame,
                                VsmInstr cont,
                                DApplyExpr * apply_expr)
        {
            DVsmEvalArgsFrame * result = nullptr;

            void * mem = mm.alloc(typeseq::id<DVsmEvalArgsFrame>(),
                                  sizeof(DVsmEvalArgsFrame));

            result = new (mem) DVsmEvalArgsFrame(apply_frame, cont, apply_expr);

            assert(result);

            return result;
        }

        DVsmEvalArgsFrame *
        DVsmEvalArgsFrame::gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept
        {
            return gc.std_move_for(this);
        }

        void
        DVsmEvalArgsFrame::visit_gco_children(obj<AGCObjectVisitor> gc) noexcept
        {
            gc.visit_child(&parent_);
            gc.visit_child(&apply_expr_);
        }

        bool
        DVsmEvalArgsFrame::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DVsmEvalArgsFrame",
                                             refrtag("cont", cont_),
                                             refrtag("i_arg", i_arg_));
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmEvalArgsFrame.cpp */
