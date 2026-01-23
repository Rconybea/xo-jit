/** @file ReaderConfig.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/arena/CircularBufferConfig.hpp>
#include <xo/arena/ArenaConfig.hpp>

namespace xo {
    namespace scm {

        /** @brief Configuration for SchemtikaReader
         **/
        struct ReaderConfig {
            using CircularBufferConfig = xo::mm::CircularBufferConfig;
            using ArenaConfig = xo::mm::ArenaConfig;
            using size_t = std::size_t;

            /** tokenizer circular buffer config **/
            CircularBufferConfig tk_buffer_config_ {.name_ = "tk-buffer",
                                                    .max_capacity_ = 2*1024*1024,
                                                    .hugepage_z_ = 2*1024*1024,
                                                    .threshold_move_efficiency_ = 50.0,
                                                    .max_captured_span_ = 128 };
            /** debug flag for schematika tokenizer **/
            bool tk_debug_flag_ = false;
            /** arena configuration for parser stack **/
            ArenaConfig parser_arena_config_ { .name_ = "parer-arena",
                                               .size_ = 2*1024*1024,
                                               .hugepage_z_ = 2*1024*1024,
                                               .store_header_flag_ = false,
                                               .header_{},
                                               .debug_flag_ = false };
            /** max size (in bytes) of stringtable **/
            size_t max_stringtable_cap_ = 64*1024;
            /** debug flag for schematika parser **/
            bool parser_debug_flag_ = false;
#ifdef NOT_YET
            /** arena configuration for output expressions **/
            ArenaConfig expr_arena_config_ { .name_ = "expr-arena",
                                             .size_ = 2*1024*1024,
                                             .hugepage_z_ = 2*1024*1024,
                                             .store_header_flag_ = false,
                                             .header_{},
                                             .debug_flag_ = false };
#endif
        };

    } /*namespace scm*/
} /*namepspace xo*/

/* end ReaderConfig.hpp */
