/** @file DVsmSeqContFrame.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmSeqContFrame.hpp"

namespace xo {
    namespace scm {

        DVsmSeqContFrame::DVsmSeqContFrame(obj<AGCObject> parent,
                                           VsmInstr cont,
                                           DSequenceExpr * seq_expr,
                                           uint32_t i_seq)
                : parent_{parent},
                  cont_{cont},
                  seq_expr_{seq_expr},
                  i_seq_{i_seq}
        {}

        DVsmSeqContFrame *
        DVsmSeqContFrame::make(obj<AAllocator> mm,
                                    obj<AGCObject> parent,
                                    VsmInstr cont,
                                    DSequenceExpr * seq_expr,
                                    uint32_t i_seq)
        {
            void * mem = mm.alloc_for<DVsmSeqContFrame>();

            return new (mem) DVsmSeqContFrame(parent, cont, seq_expr, i_seq);
        }

        // gcobject facet

        DVsmSeqContFrame *
        DVsmSeqContFrame::gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept
        {
            return gc.std_move_for(this);
        }

        void
        DVsmSeqContFrame::visit_gco_children(obj<AGCObjectVisitor> gc) noexcept
        {
            gc.visit_child(&parent_);
            gc.visit_child(&seq_expr_);
        }

        // printable facet

        bool
        DVsmSeqContFrame::pretty(const ppindentinfo & ppii) const noexcept
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DVsmSeqContFrame",
                                             refrtag("cont", cont_),
                                             refrtag("i_seq", i_seq_));
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmSeqContFrame.cpp */
