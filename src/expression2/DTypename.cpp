/** @file DTypename.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "Typename.hpp"
#include <xo/stringtable2/UniqueString.hpp>
#include <xo/alloc2/GCObject.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/print/quoted.hpp>
#include <cstddef>

namespace xo {
    using xo::mm::ACollector;
    using xo::mm::AGCObject;
    using xo::print::APrintable;

    namespace scm {

        DTypename *
        DTypename::_make(obj<AAllocator> mm,
                         const DUniqueString * name,
                         obj<AType> type)
        {
            void * mem = mm.alloc_for<DTypename>();

            return new (mem) DTypename(name, type);
        }

        obj<AGCObject,DTypename>
        DTypename::make(obj<AAllocator> mm,
                        const DUniqueString * name,
                        obj<AType> type)
        {
            return obj<AGCObject,DTypename>(_make(mm, name, type));
        }

        DTypename::DTypename(const DUniqueString * name,
                             obj<AType> type)
            : name_{name}, type_{type}
        {}

        DTypename *
        DTypename::gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept
        {
            return gc.std_move_for(this);
        }

        void
        DTypename::visit_gco_children(VisitReason reason,
                                      obj<AGCObjectVisitor> gc) noexcept
        {
            gc.visit_child(reason, &name_);
            gc.visit_poly_child(reason, &type_);
        }

        bool
        DTypename::pretty(const ppindentinfo & ppii) const
        {
            using xo::print::quot;

            auto name = (name_
                         ? std::string_view(*name_)
                         : std::string_view(""));

            auto type_pr = type_.to_facet<APrintable>();

            return ppii.pps()->pretty_struct
                       (ppii,
                        "DTypename"
                        , refrtag("name", quot(name))
                        , refrtag("type", type_pr)
                           );
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DTypename.cpp */
