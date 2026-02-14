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

        std::size_t
        DVsmIfElseContFrame::shallow_size() const noexcept
        {
            return sizeof(*this);
        }

        DVsmIfElseContFrame *
        DVsmIfElseContFrame::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            return mm.std_copy_for<DVsmIfElseContFrame>(this);
        }

        std::size_t
        DVsmIfElseContFrame::forward_children(obj<ACollector> gc) noexcept
        {
            gc.forward_inplace(&parent_);
            gc.forward_inplace(&ifelse_expr_);

            return this->shallow_size();
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
