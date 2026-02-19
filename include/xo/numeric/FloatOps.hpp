/** @file FloatOps.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "Numeric.hpp"
#include <xo/procedure2/RuntimeContext.hpp>
#include <xo/object2/Float.hpp>


namespace xo {
    namespace scm {

        class FloatOps {
        public:
            using AGCObject = xo::mm::AGCObject;

        public:
            static obj<AGCObject> multiply(obj<ARuntimeContext> rcx,
                                           DFloat * x, DFloat * y);
            static obj<AGCObject> divide(obj<ARuntimeContext> rcx,
                                         DFloat * x, DFloat * y);
            static obj<AGCObject> add(obj<ARuntimeContext> rcx,
                                      DFloat * x, DFloat * y);
            static obj<AGCObject> subtract(obj<ARuntimeContext> rcx,
                                           DFloat * x, DFloat * y);
        };

    }
}

/* end FloatOps.hpp */
