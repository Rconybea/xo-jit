/** @file DLocalSymtab.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DLocalSymtab.hpp"
#include "DUniqueString.hpp"
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::facet::typeseq;

    namespace scm {

        DLocalSymtab::DLocalSymtab(size_type n) : capacity_{n}, size_{0}
        {
            for (size_type i = 0; i < n; ++i) {
                void * mem = &slots_[i];
                new (mem) Slot();
            }
        }

        DLocalSymtab *
        DLocalSymtab::_make_empty(obj<AAllocator> mm, size_type n)
        {
            void * mem = mm.alloc(typeseq::id<DLocalSymtab>(),
                                  sizeof(DLocalSymtab) + (n * sizeof(Slot)));

            return new (mem) DLocalSymtab(n);
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

    } /*namespace scm*/
} /*namespace xo*/

/* end DLocalSymtab.cpp */
