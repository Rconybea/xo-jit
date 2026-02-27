/** @file FloatOps.cp
 *
 *  @author Roland Conybeare, Feb 2206
 **/

#include "FloatOps.hpp"
#include "float/INumeric_DFloat.hpp"
#include <xo/object2/Boolean.hpp>

namespace xo {
    using xo::mm::AGCObject;

    namespace scm {

        obj<AGCObject>
        FloatOps::multiply(obj<ARuntimeContext> rcx,
                           DFloat * x, DFloat * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() * y->value());
        }

        obj<AGCObject>
        FloatOps::divide(obj<ARuntimeContext> rcx,
                           DFloat * x, DFloat * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() / y->value());
        }

        obj<AGCObject>
        FloatOps::add(obj<ARuntimeContext> rcx,
                      DFloat * x, DFloat * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() + y->value());
        }

        obj<AGCObject>
        FloatOps::subtract(obj<ARuntimeContext> rcx,
                           DFloat * x, DFloat * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() - y->value());
        }

        obj<AGCObject>
        FloatOps::cmp_equal(obj<ARuntimeContext> rcx,
                            DFloat * x, DFloat * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(), x->value() == y->value());
        }

        obj<AGCObject>
        FloatOps::cmp_notequal(obj<ARuntimeContext> rcx,
                               DFloat * x, DFloat * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(), x->value() != y->value());
        }

        obj<AGCObject>
        FloatOps::cmp_less(obj<ARuntimeContext> rcx,
                           DFloat * x, DFloat * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(), x->value() < y->value());
        }
    }
}

/* end FloatOps.cpp */
