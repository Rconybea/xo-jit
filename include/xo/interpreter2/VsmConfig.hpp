/** @file VsmConfig.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/reader2/ReaderConfig.hpp>
#include <xo/gc/X1CollectorConfig.hpp>
#include <xo/arena/ArenaConfig.hpp>

namespace xo {
    namespace scm {
        /** Configuration for virtual schematika machine
         **/
        struct VsmConfig {
            using X1CollectorConfig = xo::mm::X1CollectorConfig;
            using ArenaConfig = xo::mm::ArenaConfig;

            VsmConfig() = default;

            VsmConfig with_debug_flag(bool x) const {
                VsmConfig retval = *this;
                retval.debug_flag_ = x;
                return retval;
            }

            /** true for interactive parser session; false for batch session **/
            bool interactive_flag_ = true;

            /** true to enable logging **/
            bool debug_flag_ = false;

            /** reader configuration **/
            ReaderConfig rdr_config_;
            /** Configuration for allocator/collector.
             *  TODO: may want to make CollectorConfig polymorphic
             **/
            X1CollectorConfig x1_config_ = X1CollectorConfig().with_name("gc").with_size(4*1024*1024);
            /** Configuration for handful of non-moveable high-level objects
             *  e.g. DArenaHashMap in global symtab
             **/
            ArenaConfig fixed_config_ = ArenaConfig().with_name("fixed").with_size(4*1024);
            /** Configuration for error allocator
             *  TODO: may want to make ArenaConfig polymorphic
             **/
            ArenaConfig error_config_ = ArenaConfig().with_name("error-reserve").with_size(64*1024);
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmConfig.hpp */
