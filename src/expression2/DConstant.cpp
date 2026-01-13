/** @file DConstant.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DConstant.hpp"
#include "TypeDescr.hpp"
#include <xo/object2/DFloat.hpp>
#include <xo/object2/DInteger.hpp>
#include <xo/reflect/Reflect.hpp>
#include <xo/reflectutil/typeseq.hpp>

namespace xo {
    using xo::scm::DFloat;
    using xo::scm::DInteger;
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
    } /*namespace scm*/
} /*namespace xo*/

/* end DConstant.cpp */
