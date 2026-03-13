/** @file ReaderConfig.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/procedure2/PrimitiveRegistry.hpp>
#include <xo/arena/CircularBufferConfig.hpp>
#include <xo/arena/ArenaHashMapConfig.hpp>
#include <xo/arena/ArenaConfig.hpp>

namespace xo {
    namespace scm {

        /** @brief Configuration for SchematikaReader
         **/
        struct ReaderConfig {
            using ArenaHashMapConfig = xo::map::ArenaHashMapConfig;
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
            ArenaConfig parser_arena_config_ { .name_ = "parser-arena",
                                               .size_ = 2*1024*1024,
                                               .hugepage_z_ = 2*1024*1024,
                                               .store_header_flag_ = false,
                                               .header_{},
                                               .debug_flag_ = false };

            /** configuration for hash map for global symbol table (variables)
             *
             *  reminder: ownership chain
             *    SchematikaReader
             *      ->SchematikaParser
             *      ->ParserStateMachine
             *      ->DGlobalSymtab
             **/
            ArenaHashMapConfig symtab_var_config_ {
                .name_ = "global-vars",
                .hint_max_capacity_ = 64*1024,
                .debug_flag_ = false,
            };

            /** configuration for hash map for global symbol table (types)
             *
             *  reminder: ownership chain
             *    SchematikaReader
             *      ->SchematikaParser
             *      ->ParserStateMachine
             *      ->DGlobalSymtab
             **/
            ArenaHashMapConfig symtab_types_config_ {
                .name_ = "global-types",
                .hint_max_capacity_ = 32*1024,
                .debug_flag_ = false,
            };

            /** debug flag for schematika parser **/
            bool parser_debug_flag_ = false;

            /** max size (in bytes) of stringtable **/
            size_t max_stringtable_cap_ = 64*1024;

            /** flags controlling which primitives to install **/
            InstallFlags pm_install_flags_ = InstallFlags::f_all;

            /** debug flag for schematika_reader **/
            bool reader_debug_flag_ = false;
        };

    } /*namespace scm*/
} /*namepspace xo*/

/* end ReaderConfig.hpp */
