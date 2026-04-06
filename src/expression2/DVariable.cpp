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

        DVariable *
        DVariable::gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept
        {
            return gc.std_move_for(this);
        }

        void
        DVariable::visit_gco_children(obj<AGCObjectVisitor> gc) noexcept
        {
            typeref_.visit_gco_children(gc);
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
