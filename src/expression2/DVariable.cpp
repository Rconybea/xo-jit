/** @file DVariable.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DVariable.hpp"
#include "exprtype.hpp"
#include <xo/indentlog/print/quoted.hpp>
#include <cstddef>

namespace xo {
    using xo::mm::ACollector;
    using xo::facet::typeseq;

    namespace scm {

        DVariable *
        DVariable::make(obj<AAllocator> mm,
                        const DUniqueString * name,
                        const TypeRef & typeref,
                        Binding path)
        {
            void * mem = mm.alloc(typeseq::id<DVariable>(),
                                  sizeof(DVariable));

            return new (mem) DVariable(name, typeref, path);
        }

        DVariable::DVariable(const DUniqueString * name,
                             const TypeRef & typeref,
                             Binding path)
          : name_{name}, typeref_{typeref}, path_{path}
        {}

        void
        DVariable::assign_valuetype(TypeDescr td) noexcept
        {
            typeref_.resolve(td);
        }

        size_t
        DVariable::shallow_size() const noexcept
        {
            return sizeof(DVariable);
        }

        DVariable *
        DVariable::shallow_move(obj<AAllocator> mm) const noexcept
        {
            DVariable * copy = (DVariable *)mm.alloc_copy((std::byte *)this);

            if (copy) {
                *copy = *this;
            }

            return copy;
        }

        size_t
        DVariable::forward_children(obj<ACollector> gc) noexcept
        {
            typeref_.forward_children(gc);

            return shallow_size();
        }

        bool
        DVariable::pretty(const ppindentinfo & ppii) const
        {
            using xo::print::quot;

            auto name = (name_
                         ? std::string_view(*name_)
                         : std::string_view(""));

            return ppii.pps()->pretty_struct
                       (ppii,
                        "DVariable"
                        , refrtag("name", quot(name))
                        , refrtag("typeref", typeref_)
                           );
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DVariable.cpp */
