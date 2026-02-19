/** @file IntegerOps.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "IntegerOps.hpp"
#include "integer/INumeric_DInteger.hpp"

namespace xo {
    using xo::mm::AGCObject;

    namespace scm {

        obj<AGCObject>
        IntegerOps::multiply(obj<ARuntimeContext> rcx,
                             DInteger * x, DInteger * y)
        {
            return DInteger::box<AGCObject>(rcx.allocator(), x->value() * y->value());
        }

        obj<AGCObject>
        IntegerOps::divide(obj<ARuntimeContext> rcx,
                           DInteger * x, DInteger * y)
        {
            return DInteger::box<AGCObject>(rcx.allocator(), x->value() / y->value());
        }

        obj<AGCObject>
        IntegerOps::add(obj<ARuntimeContext> rcx,
                        DInteger * x, DInteger * y)
        {
            return DInteger::box<AGCObject>(rcx.allocator(), x->value() + y->value());
        }

        obj<AGCObject>
        IntegerOps::subtract(obj<ARuntimeContext> rcx,
                             DInteger * x, DInteger * y)
        {
            return DInteger::box<AGCObject>(rcx.allocator(), x->value() - y->value());
        }

    }
}

/* end IntegerOps.cpp */
