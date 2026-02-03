/** @file StringTable.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "DUniqueString.hpp"
#include <xo/arena/DArenaHashMap.hpp>
#include <xo/arena/DArena.hpp>
#include <xo/arena/hashmap/verify_policy.hpp>

namespace xo {
    namespace scm {

        /** @class StringTable
         *  @brief table containing a set of interned strings
         *
         *  A table of strings referenced in schematika expressions
         **/
        class StringTable {
        public:
            using DArena = xo::mm::DArena;
            using MemorySizeInfo = xo::mm::MemorySizeInfo;
            using StringMap = xo::map::DArenaHashMap<std::string_view,
                                                     DUniqueString*>;
            using size_type = StringMap::size_type;

        public:
            StringTable(size_type hint_max_capacity,
                        bool debug_flag = false);

            /** lookup interned string; nullptr if not present **/
            const DUniqueString * lookup(std::string_view key) const;

            /** return unique string with contents @p key. Idempotent! **/
            const DUniqueString * intern(std::string_view key);

            /** generate unique symbol -- guaranteed not to collide
             *  with existing symbol in this table.
             **/
            const DUniqueString * gensym(std::string_view prefix);

            /** verify StringTable invariants.
             *  Act on failure according to policy @p p
             **/
            bool verify_ok(verify_policy p = verify_policy::throw_only()) const;

            std::size_t _n_store() const noexcept;
            MemorySizeInfo _store_info(std::size_t i) const noexcept;

        private:
            /** allocate string storage in this arena; use DString to represent each string.
             *  Can't use DArenaVector b/c DString has variable size
             **/
            DArena strings_;
            /** map_[s] points to arena strings, i.e. members of @ref strings_ **/
            StringMap map_;
        };


    } /*namespace scm*/
} /*namespace xo*/

/* end StringTable.hpp */
