/** @file SchematikaParserConfig.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/arena/ArenaHashMapConfig.hpp>
#include <xo/arena/ArenaConfig.hpp>

namespace xo {
    namespace scm {

        /** @brief Configuration for SchematikaParser **/
        struct ParserConfig {
            using ArenaHashMapConfig = xo::map::ArenaHashMapConfig;
            using ArenaConfig = xo::mm::ArenaConfig;

            /** arena configuration for parser stack **/
            ArenaConfig parser_arena_config_ { .name_ = "parser-arena",
                                               .size_ = 2*1024*1024,
                                               .hugepage_z_ = 2*1024*1024,
                                               .store_header_flag_ = false,
                                               .header_{},
                                               .debug_flag_ = false };

            /** configuration for hash map for global symbol table
             *
             *  reminder: ownership chain
             *    SchematikaReader
             *      ->SchematikaParser
             *      ->ParserStateMachine
             *      ->DGlobalSymtab
             **/
            ArenaHashMapConfig symtab_config_ { .name_ = "global-symtab",
                                                .hint_max_capacity_ = 64*1024,
                                                .debug_flag_ = false };

            /** max capacity for unique string table **/
            size_t max_stringtable_capacity_ = 4096;

            /** control SchematikaParser debug logging **/
            bool debug_flag_ = false;
        };

    }
}

/* end SchematikaParserConfig.hpp */
