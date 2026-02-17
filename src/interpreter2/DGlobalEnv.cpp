/** @file DGlobalEnv.cpp
 *
 *  @author Roland Conybeare, Feb 2026
**/

#include "GlobalEnv.hpp"
#include <xo/expression2/GlobalSymtab.hpp>
#include <xo/object2/Array.hpp>

namespace xo {
    using xo::mm::AAllocator;
    using xo::mm::AGCObject;

    namespace scm {

        DGlobalEnv::DGlobalEnv(DGlobalSymtab * symtab, DArray * values)
        : symtab_{symtab}, values_{values}
        {}

        DGlobalEnv *
        DGlobalEnv::_make(obj<AAllocator> mm,
                          DGlobalSymtab * symtab)
        {
            DArray * values = DArray::empty(mm, symtab->capacity());

            void * mem = mm.alloc_for<DGlobalSymtab>();

            return new (mem) DGlobalEnv(symtab, values);
        }

        obj<AGCObject>
        DGlobalEnv::lookup_value(Binding ix) const noexcept
        {
            if (!ix.is_global()) {
                assert(false);
                return obj<AGCObject>();
            }

            if (ix.j_slot() >= static_cast<int32_t>(values_->size())) {
                assert(false);
                return obj<AGCObject>();
            }

            return (*values_)[ix.j_slot()];
        }

        void
        DGlobalEnv::assign_value(obj<AAllocator> mm, Binding ix, obj<AGCObject> x)
        {
            scope log(XO_DEBUG(true),
                      xtag("ix.j_slot", ix.j_slot()),
                      xtag("values.cap", values_->capacity()));

            assert(ix.is_global());

            if (ix.j_slot() >= static_cast<int32_t>(values_->size())) {
                // Control will come here in interpreter as new definitions are introduced.
                // After seeing
                //   def foo = 1.2345;
                // introducing new symbol foo:
                // GlobalSymtab extends to include foo without this GlobalEnv
                // knowing about it.

                if (ix.j_slot() + 1 > static_cast<int32_t>(values_->capacity())) {
                    // realloc global array for more size

                    size_t cap_2x = 2 * values_->capacity();

                    while (cap_2x < static_cast<size_t>(ix.j_slot() + 1))
                        cap_2x = 2 * cap_2x;

                    DArray * values_2x = DArray::copy(mm, values_, cap_2x);
                    assert(values_2x);

                    if (values_2x) {
                        log && log("STUB: need write barrier for GC (also in GlobalSymtab!)");
                        this->values_ = values_2x;
                    } else {
                        return;
                    }
                }

                /** expand size sot that j_slot is valid **/
                values_->resize(ix.j_slot() + 1);
            }

            log && log("STUB: need write barrier for GC here");
            (*values_)[ix.j_slot()] = x;
        }

        // ----- AGCObject facet -----

        std::size_t
        DGlobalEnv::shallow_size() const noexcept
        {
            return sizeof(*this);
        }

        DGlobalEnv *
        DGlobalEnv::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            return mm.std_copy_for<DGlobalEnv>(this);
        }

        std::size_t
        DGlobalEnv::forward_children(obj<ACollector> gc) noexcept
        {
            gc.forward_inplace(&symtab_);
            gc.forward_inplace(&values_);

            return this->shallow_size();
        }

        // ----- APrintable facet -----

        bool
        DGlobalEnv::pretty(const ppindentinfo & ppii) const
        {
            return ppii.pps()->pretty_struct
                       (ppii,
                        "DGlobalEnv",
                        refrtag("size", symtab_->size()));
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DGlobalEnv.cpp */
