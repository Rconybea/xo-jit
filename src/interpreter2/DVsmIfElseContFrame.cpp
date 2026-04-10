/** @file DVsmIfElseContFrame.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmIfElseContFrame.hpp"

namespace xo {
    namespace scm {

        DVsmIfElseContFrame::DVsmIfElseContFrame(obj<AGCObject> parent,
                                                 VsmInstr cont,
                                                 DIfElseExpr * ifelse_expr)
                : parent_{parent},
                  cont_{cont},
                  ifelse_expr_{ifelse_expr}
        {}

        DVsmIfElseContFrame *
        DVsmIfElseContFrame::make(obj<AAllocator> mm,
                                    obj<AGCObject> parent,
                                    VsmInstr cont,
                                  DIfElseExpr * seq_expr)
        {
            void * mem = mm.alloc_for<DVsmIfElseContFrame>();

            return new (mem) DVsmIfElseContFrame(parent, cont, seq_expr);
        }

        // gcobject facet

        DVsmIfElseContFrame *
        DVsmIfElseContFrame::gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept
        {
            return gc.std_move_for<DVsmIfElseContFrame>(this);
        }

        void
        DVsmIfElseContFrame::visit_gco_children(VisitReason reason,
                                                obj<AGCObjectVisitor> gc) noexcept
        {
            gc.visit_child(reason, &parent_);
            gc.visit_child(reason, &ifelse_expr_);
        }

        // printable facet

        bool
        DVsmIfElseContFrame::pretty(const ppindentinfo & ppii) const noexcept
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DVsmIfElseContFrame",
                                             refrtag("cont", cont_));
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmIfElseContFrame.cpp */
