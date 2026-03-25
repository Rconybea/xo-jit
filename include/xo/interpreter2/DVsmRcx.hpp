/** @file DVsmRcx.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/stringtable2/StringTable.hpp>
#include <xo/alloc2/Allocator.hpp>
#include <xo/arena/MemorySizeInfo.hpp>

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
            using StringTable = xo::scm::StringTable;
            using AAllocator = xo::mm::AAllocator;
            using ACollector = xo::mm::ACollector;
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;

        public:
            DVsmRcx(VirtualSchematikaMachine * vsm);

            obj<AAllocator> allocator() const noexcept;
            obj<ACollector> collector() const noexcept;
            StringTable * stringtable() const noexcept;
            obj<AAllocator> error_allocator() const noexcept;
            void visit_pools(const MemorySizeVisitor & visitor) const;

        private:
            /** schematika interpreter **/
            VirtualSchematikaMachine * vsm_ = nullptr;;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DVsmRcx.hpp */
