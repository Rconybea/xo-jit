/** @file StringTable.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "StringTable.hpp"
#include <xo/alloc2/Allocator.hpp>
#include <xo/alloc2/arena/IAllocator_DArena.hpp>

namespace xo {
    using xo::mm::ArenaConfig;
    using xo::mm::AAllocator;
    using xo::facet::with_facet;
    using xo::facet::obj;

    namespace scm {
        StringTable::StringTable(size_type hint_max_capacity,
                                 bool debug_flag)
        : strings_{DArena::map(ArenaConfig{.name_ = "strings",
                                           .size_ = hint_max_capacity})},
          map_{hint_max_capacity}
        {
            (void)debug_flag;
        }

        const DUniqueString *
        StringTable::lookup(std::string_view key) const
        {
            auto ix = map_.find(key);

            if (ix != map_.end())
                return ix->second;

            return nullptr;
        }

        const DUniqueString *
        StringTable::intern(std::string_view key)
        {
            // 1a. lookup key in map_.
            // 1b. if present, return existing DString*

            auto ix = map_.find(key);

            if (ix != map_.end())
                return ix->second;

            // 2. otherwise need to add.
            //
            // 2d. return key2 address

            // 2a. allocate DUniqueString copy 'interned' of key in strings_
            auto mm = with_facet<AAllocator>::mkobj(&strings_);
            DUniqueString * interned = DUniqueString::from_view(mm, key);

            assert(interned);
            if (interned) {
                // 2b. make string_view from *interned
                std::string_view interned_key = std::string_view(*interned);

                // interned_key has same lifetime as StringTable,
                // we can use it in map_

                // 2c. store address of 'interned' in map_
                auto & slot = this->map_[interned_key];

                slot = interned;

                return slot;
            }

            return nullptr;
        }

        bool
        StringTable::verify_ok(verify_policy policy) const
        {
            using xo::scope;
            using xo::xtag;

            constexpr const char * c_self = "StringTable::verify_ok";
            scope log(XO_DEBUG(false));

            /* ST1: underlying hash map passes its invariants */
            if (!map_.verify_ok(policy)) {
                return policy.report_error(log,
                                           c_self, ": map_.verify_ok failed");
            }

            /* ST2: for each entry, key points to value's string data */
            for (const auto & kv : map_) {
                const std::string_view & key = kv.first;
                const DUniqueString * value = kv.second;

                /* ST2.1: value is not null */
                if (value == nullptr) {
                    return policy.report_error(log,
                                               c_self, ": null value in map",
                                               xtag("key", key));
                }

                /* ST2.2: value lies within strings_ arena */
                if (!strings_.contains(value)) {
                    return policy.report_error(log,
                                               c_self, ": value not in strings_ arena",
                                               xtag("key", key),
                                               xtag("value", (void*)value));
                }

                /* ST2.3: key.data() points to value's chars */
                if (key.data() != value->chars()) {
                    return policy.report_error(log,
                                               c_self, ": key.data() != value->chars()",
                                               xtag("key", key),
                                               xtag("key.data()", (void*)key.data()),
                                               xtag("value->chars()", (void*)value->chars()));
                }

                /* ST2.4: key.size() == value->size() */
                if (key.size() != value->size()) {
                    return policy.report_error(log,
                                               c_self, ": key.size() != value->size()",
                                               xtag("key", key),
                                               xtag("key.size()", key.size()),
                                               xtag("value->size()", value->size()));
                }
            }

            return true;
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end StringTable.cpp */
