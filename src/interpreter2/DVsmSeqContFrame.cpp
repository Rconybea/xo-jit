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

        std::size_t
        DVsmSeqContFrame::shallow_size() const noexcept
        {
            return sizeof(*this);
        }

        DVsmSeqContFrame *
        DVsmSeqContFrame::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            return mm.std_copy_for<DVsmSeqContFrame>(this);
        }

        std::size_t
        DVsmSeqContFrame::forward_children(obj<ACollector> gc) noexcept
        {
            gc.forward_inplace(&parent_);
            gc.forward_inplace(&seq_expr_);

            return this->shallow_size();
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
