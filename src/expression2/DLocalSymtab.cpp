/** @file DLocalSymtab.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DLocalSymtab.hpp"
#include "DUniqueString.hpp"
#include <xo/expression2/detail/IPrintable_DVariable.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::typeseq;
    using xo::print::ppstate;

    namespace scm {

        DLocalSymtab::DLocalSymtab(DLocalSymtab * p,
                                   size_type n) : parent_{p},
                                                  capacity_{n},
                                                  size_{0}
        {
            for (size_type i = 0; i < n; ++i) {
                void * mem = &slots_[i];
                new (mem) Slot();
            }
        }

        DLocalSymtab *
        DLocalSymtab::_make_empty(obj<AAllocator> mm,
                                  DLocalSymtab * p,
                                  size_type n)
        {
            void * mem = mm.alloc(typeseq::id<DLocalSymtab>(),
                                  sizeof(DLocalSymtab) + (n * sizeof(Slot)));

            return new (mem) DLocalSymtab(p, n);
        }

        Binding
        DLocalSymtab::append_var(obj<AAllocator> mm,
                                 const DUniqueString * name,
                                 TypeRef typeref)
        {
            assert(name);

            if (size_ >= capacity_ || !name) {
                assert(false);

                return Binding::null();
            } else {
                size_type i_slot = (this->size_)++;
                Binding binding = Binding::local(i_slot);
                DVariable * var = DVariable::make(mm, name, typeref, binding);

                this->slots_[i_slot] = Slot(var);

                return binding;
            }
        }

        Binding
        DLocalSymtab::lookup_binding(const DUniqueString * sym) const noexcept
        {
            assert(sym);

            if (sym) {
                for (size_type i = 0; i < size_; ++i) {
                    const Slot & slot = slots_[i];

                    if (*sym == *(slot.var_->name()))
                        return slot.var_->path();
                }
            }

            return Binding();
        }

        bool
        DLocalSymtab::pretty(const ppindentinfo & ppii) const
        {
            ppstate * pps = ppii.pps();

            if (ppii.upto()) {
                /* perhaps print on one line */
                if (!pps->print_upto("<LocalSymtab"))
                    return false;
                if (!pps->print_upto(xrefrtag("size", size_)))
                    return false;

                for (size_type i = 0; i < size_; ++i) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "[%u]", i);

                    assert(slots_[i].var_);

                    obj<APrintable,DVariable> arg_pr(const_cast<DVariable*>(slots_[i].var_));

                    if (!pps->print_upto(xrefrtag(buf, arg_pr)))
                        return false;
                }

                pps->write(">");
                return true;
            } else {
                /* with line breaks */

                pps->write("<LocalSymtab");
                pps->newline_pretty_tag(ppii.ci1(), "size", size_);

                for (size_type i = 0; i < size_; ++i) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "[%u]", i);

                    assert(slots_[i].var_);

                    obj<APrintable,DVariable> arg_pr(const_cast<DVariable *>(slots_[i].var_));

                    pps->newline_pretty_tag(ppii.ci1(), buf, arg_pr);
                }

                pps->write(">");
                return false;
            }
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalSymtab.cpp */
