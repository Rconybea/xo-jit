/** @file DVsmRcx.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/alloc2/Allocator.hpp>

namespace xo {
    namespace scm {
        // see xo-interpreter/ VirtualSchematikaMachine.hpp
        class VirtualSchematikaMachine; 

        /** @brief Runtime context for schematika interpreter
         *
         *  Provides allocator
         **/
        class DVsmRcx {
        public:
            using AAllocator = xo::mm::AAllocator;

        public:
            DVsmRcx(VirtualSchematikaMachine * vsm);

            obj<AAllocator> allocator() const noexcept;
            obj<AAllocator> error_allocator() const noexcept;

        private:
            /** schematika interpreter **/
            VirtualSchematikaMachine * vsm_ = nullptr;;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmRcx.hpp */

