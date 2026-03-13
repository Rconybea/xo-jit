/** @file SchematikaParserConfig.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include <xo/procedure2/PrimitiveRegistry.hpp>
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
                                               .store_header_flag_ = true,
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
                .debug_flag_ = false
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
                .debug_flag_ = false
            };

            /** max capacity for unique string table **/
            size_t max_stringtable_capacity_ = 4096;

            /** flags controlling which primitives to install **/
            InstallFlags pm_install_flags_ = InstallFlags::f_all;

            /** control SchematikaParser debug logging **/
            bool debug_flag_ = false;
        };

    }
}

/* end SchematikaParserConfig.hpp */
