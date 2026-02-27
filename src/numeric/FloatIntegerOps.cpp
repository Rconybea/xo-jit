/** @file FloatIntegerOps.cp
 *
 *  @author Roland Conybeare, Feb 2206
 **/

#include "FloatIntegerOps.hpp"
#include "float/INumeric_DFloat.hpp"
#include <xo/object2/Boolean.hpp>

namespace xo {
    using xo::mm::AGCObject;

    namespace scm {

        // ----- Float op Integer -----

        obj<AGCObject>
        FloatIntegerOps::multiply(obj<ARuntimeContext> rcx,
                                  DFloat * x, DInteger * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() * y->value());
        }

        obj<AGCObject>
        FloatIntegerOps::divide(obj<ARuntimeContext> rcx,
                                DFloat * x, DInteger * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() / y->value());
        }

        obj<AGCObject>
        FloatIntegerOps::add(obj<ARuntimeContext> rcx,
                             DFloat * x, DInteger * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() + y->value());
        }

        obj<AGCObject>
        FloatIntegerOps::subtract(obj<ARuntimeContext> rcx,
                                  DFloat * x, DInteger * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() - y->value());
        }

        obj<AGCObject>
        FloatIntegerOps::cmp_equal(obj<ARuntimeContext> rcx,
                                   DFloat * x, DInteger * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(),
                                            x->value() == DFloat::value_type(y->value()));
        }

        obj<AGCObject>
        FloatIntegerOps::cmp_notequal(obj<ARuntimeContext> rcx,
                                      DFloat * x, DInteger * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(),
                                            x->value() != DFloat::value_type(y->value()));
        }

        obj<AGCObject>
        FloatIntegerOps::cmp_less(obj<ARuntimeContext> rcx,
                                  DFloat * x, DInteger * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(),
                                            x->value() < DFloat::value_type(y->value()));
        }

        // ----- Integer op Float -----

        obj<AGCObject>
        IntegerFloatOps::multiply(obj<ARuntimeContext> rcx,
                                  DInteger * x, DFloat * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() * y->value());
        }

        obj<AGCObject>
        IntegerFloatOps::divide(obj<ARuntimeContext> rcx,
                                DInteger * x, DFloat * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() / y->value());
        }

        obj<AGCObject>
        IntegerFloatOps::add(obj<ARuntimeContext> rcx,
                             DInteger * x, DFloat * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() + y->value());
        }

        obj<AGCObject>
        IntegerFloatOps::subtract(obj<ARuntimeContext> rcx,
                                  DInteger * x, DFloat * y)
        {
            return DFloat::box<AGCObject>(rcx.allocator(), x->value() - y->value());
        }

        obj<AGCObject>
        IntegerFloatOps::cmp_equal(obj<ARuntimeContext> rcx,
                                   DInteger * x, DFloat * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(),
                                            DFloat::value_type(x->value()) == y->value());
        }

        obj<AGCObject>
        IntegerFloatOps::cmp_notequal(obj<ARuntimeContext> rcx,
                                      DInteger * x, DFloat * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(),
                                            DFloat::value_type(x->value()) != y->value());
        }

        obj<AGCObject>
        IntegerFloatOps::cmp_less(obj<ARuntimeContext> rcx,
                                  DInteger * x, DFloat * y)
        {
            return DBoolean::box<AGCObject>(rcx.allocator(),
                                            DFloat::value_type(x->value()) < y->value());
        }

    }
}

/* end FloatIntegerOps.cpp */
