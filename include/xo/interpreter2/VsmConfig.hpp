/** @file VsmConfig.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/reader2/ReaderConfig.hpp>
#include <xo/gc/X1CollectorConfig.hpp>

namespace xo {
    namespace scm {
        /** Configuration for virtual schematika machine
         **/
        struct VsmConfig {
            using X1CollectorConfig = xo::mm::X1CollectorConfig;

            VsmConfig() = default;

            /** true for interactive parser session; false for batch session **/
            bool interactive_flag_ = true;

            /** reader configuration **/
            ReaderConfig rdr_config_;
            /** Configuration for allocator/collector.
             *  TODO: may want to make CollectorConfig polymorphic
             **/
            X1CollectorConfig x1_config_ = X1CollectorConfig().with_size(4*1024*1024);
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmConfig.hpp */
