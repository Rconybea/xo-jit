/** @file DConstant.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DConstant.hpp"
#include "detail/IExpression_DConstant.hpp"
#include "TypeDescr.hpp"
#include <xo/object2/DFloat.hpp>
#include <xo/object2/DInteger.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/facet/obj.hpp>
#include <xo/reflect/Reflect.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/reflectutil/typeseq.hpp>

namespace xo {
    using xo::scm::DFloat;
    using xo::scm::DInteger;
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::reflect::Reflect;
    using xo::reflect::TypeDescr;
    using xo::reflect::typeseq;

    namespace scm {
        DConstant::DConstant(obj<AGCObject> value) noexcept
            :
            //typeref_{TypeRef::resolved(td)},
              value_{value}
        {
            // todo: use ObjectConverter here

            auto tseq = value_._typeseq();

            TypeDescr td = this->_lookup_td(tseq);

            if (td) {
                typeref_ = TypeRef::resolved(td);
            }
        }

        obj<AExpression,DConstant>
        DConstant::make(obj<AAllocator> mm,
                        obj<AGCObject> value)
        {
            return obj<AExpression,DConstant>(_make(mm, value));
        }

        DConstant *
        DConstant::_make(obj<AAllocator> mm,
                         obj<AGCObject> value)
        {
            void * mem = mm.alloc(typeseq::id<DConstant>(),
                                  sizeof(DConstant));

            return new (mem) DConstant(value);
        }

        TypeDescr
        DConstant::_lookup_td(typeseq tseq)
        {
            if (tseq == typeseq::id<DFloat>()) {
                /* double */
                return Reflect::require<DFloat::value_type>();
            } else if (tseq == typeseq::id<DInteger>()) {
                /* long */
                return Reflect::require<DInteger::value_type>();
            }

            return nullptr;
        }

        bool
        DConstant::pretty(const ppindentinfo & ppii) const
        {
            obj<APrintable> value_pr
                = FacetRegistry::instance().variant<APrintable,AGCObject>(value_);

            return ppii.pps()->pretty_struct
                       (ppii,
                        "DConstant",
                        refrtag("value_.tseq", value_._typeseq()),
                        refrtag("value.tseq", value_pr._typeseq()),
                        refrtag("value", value_pr));
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DConstant.cpp */
