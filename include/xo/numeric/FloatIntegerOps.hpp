/** @file FloatIntegerOps.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "Numeric.hpp"
#include <xo/procedure2/RuntimeContext.hpp>
#include <xo/object2/Float.hpp>
#include <xo/object2/Integer.hpp>


namespace xo {
    namespace scm {

        class FloatIntegerOps {
        public:
            using AGCObject = xo::mm::AGCObject;

        public:
            static obj<AGCObject> multiply(obj<ARuntimeContext> rcx,
                                           DFloat * x, DInteger * y);
            static obj<AGCObject> divide(obj<ARuntimeContext> rcx,
                                         DFloat * x, DInteger * y);
            static obj<AGCObject> add(obj<ARuntimeContext> rcx,
                                      DFloat * x, DInteger * y);
            static obj<AGCObject> subtract(obj<ARuntimeContext> rcx,
                                           DFloat * x, DInteger * y);

            static obj<AGCObject> cmp_equal(obj<ARuntimeContext> rcx,
                                            DFloat * x, DInteger * y);
            static obj<AGCObject> cmp_notequal(obj<ARuntimeContext> rcx,
                                               DFloat * x, DInteger * y);
            static obj<AGCObject> cmp_less(obj<ARuntimeContext> rcx,
                                           DFloat * x, DInteger * y);
            static obj<AGCObject> cmp_lessequal(obj<ARuntimeContext> rcx,
                                                DFloat * x, DInteger * y);
            static obj<AGCObject> cmp_greater(obj<ARuntimeContext> rcx,
                                                 DFloat * x, DInteger * y);
            static obj<AGCObject> cmp_greatequal(obj<ARuntimeContext> rcx,
                                                 DFloat * x, DInteger * y);
        };

        class IntegerFloatOps {
        public:
            using AGCObject = xo::mm::AGCObject;

        public:
            static obj<AGCObject> multiply(obj<ARuntimeContext> rcx,
                                           DInteger * x, DFloat * y);
            static obj<AGCObject> divide(obj<ARuntimeContext> rcx,
                                         DInteger * x, DFloat * y);
            static obj<AGCObject> add(obj<ARuntimeContext> rcx,
                                      DInteger * x, DFloat * y);
            static obj<AGCObject> subtract(obj<ARuntimeContext> rcx,
                                           DInteger * x, DFloat * y);

            static obj<AGCObject> cmp_equal(obj<ARuntimeContext> rcx,
                                            DInteger * x, DFloat * y);
            static obj<AGCObject> cmp_notequal(obj<ARuntimeContext> rcx,
                                               DInteger * x, DFloat * y);
            static obj<AGCObject> cmp_less(obj<ARuntimeContext> rcx,
                                           DInteger * x, DFloat * y);
            static obj<AGCObject> cmp_lessequal(obj<ARuntimeContext> rcx,
                                                DInteger * x, DFloat * y);
            static obj<AGCObject> cmp_greater(obj<ARuntimeContext> rcx,
                                              DInteger * x, DFloat * y);
            static obj<AGCObject> cmp_greatequal(obj<ARuntimeContext> rcx,
                                                 DInteger * x, DFloat * y);
        };

    }
}

/* end FloatIntegerOps.hpp */
