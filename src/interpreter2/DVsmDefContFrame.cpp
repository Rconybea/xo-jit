/** @file DVsmDefContFrame.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "DVsmDefContFrame.hpp"
#include <xo/expression2/DefineExpr.hpp>
#include <xo/gc/Collector.hpp>

namespace xo {
    namespace scm {

        DVsmDefContFrame::DVsmDefContFrame(obj<AGCObject> parent,
                                           VsmInstr cont,
                                           DDefineExpr * def_expr)
                : parent_{parent},
                  cont_{cont},
                  def_expr_{def_expr}
        {}

        DVsmDefContFrame *
        DVsmDefContFrame::make(obj<AAllocator> mm,
                               obj<AGCObject> parent,
                               VsmInstr cont,
                               DDefineExpr * def_expr)
        {
            void * mem = mm.alloc_for<DVsmDefContFrame>();

            return new (mem) DVsmDefContFrame(parent, cont, def_expr);
        }

        // gcobject facet

        std::size_t
        DVsmDefContFrame::shallow_size() const noexcept
        {
            return sizeof(*this);
        }

        DVsmDefContFrame *
        DVsmDefContFrame::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            return mm.std_copy_for<DVsmDefContFrame>(this);
        }

        std::size_t
        DVsmDefContFrame::forward_children(obj<ACollector> gc) noexcept
        {
            gc.forward_inplace(&parent_);
            gc.forward_inplace(&def_expr_);

            return this->shallow_size();
        }

        // printable facet

        bool
        DVsmDefContFrame::pretty(const ppindentinfo & ppii) const noexcept
        {
            return ppii.pps()->pretty_struct(ppii,
                                             "DVsmDefContFrame",
                                             refrtag("cont", cont_));
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmDefContFrame.cpp */
