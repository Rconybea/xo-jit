/** @file IntegerOps.cpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "IntegerOps.hpp"
#include "integer/INumeric_DInteger.hpp"
#include <xo/object2/Boolean.hpp>

namespace xo {
    using xo::scm::DBoolean;
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

        obj<AGCObject>
        IntegerOps::cmp_equal(obj<ARuntimeContext> rcx,
                              DInteger * x, DInteger * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(), x->value() == y->value());
        }

        obj<AGCObject>
        IntegerOps::cmp_notequal(obj<ARuntimeContext> rcx,
                                 DInteger * x, DInteger * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(), x->value() != y->value());
        }

        obj<AGCObject>
        IntegerOps::cmp_less(obj<ARuntimeContext> rcx,
                             DInteger * x, DInteger * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(), x->value() < y->value());
        }

    }
}

/* end IntegerOps.cpp */
