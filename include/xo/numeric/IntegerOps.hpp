/** @file IntegerOps.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "Numeric.hpp"
#include <xo/procedure2/RuntimeContext.hpp>
#include <xo/object2/Integer.hpp>

namespace xo {
    namespace scm {

        class IntegerOps {
        public:
            using AGCObject = xo::mm::AGCObject;

        public:
            static obj<AGCObject> multiply(obj<ARuntimeContext> rcx,
                                           DInteger * x, DInteger * y);

            static obj<AGCObject> divide(obj<ARuntimeContext> rcx,
                                         DInteger * x, DInteger * y);

            static obj<AGCObject> add(obj<ARuntimeContext> rcx,
                                      DInteger * x, DInteger * y);

            static obj<AGCObject> subtract(obj<ARuntimeContext> rcx,
                                           DInteger * x, DInteger * y);

            static obj<AGCObject> cmp_equal(obj<ARuntimeContext> rcx,
                                            DInteger * x, DInteger * y);

        };

    }
}

/* end IntegerOps.hpp */
