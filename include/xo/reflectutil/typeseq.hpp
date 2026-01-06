/** @file typeseq.hpp
 *
 *  @author Roland Conybeare, Dec 2025
 **/

#pragma once

#include <iostream>
#include <cstdint>

namespace xo {
    namespace reflect {
        /**
         * Tag here so we can preserve header-only implementation
         * and still have static variable
         */
        template<typename Tag = class typeseq_tag>
        struct typeseq_impl {
            explicit typeseq_impl(int32_t s) : seqno_{s} {}

            /** Can't have this be constexpr.
             *  We need ids in shared libraries to be generated
             *  at load time to avoid false positives
             *
             *  Return unique id number for each type.
             *  Numbers are sequentially allocated, so can use
             *  as vector indices
             *
             *  Conversely note that built-in typeinfo may
             *  return false negatives across library boundaries
             *  when using clang.
             **/
            template <typename T>
            static typeseq_impl<Tag> id() {
                static bool armed = true;
                static int32_t id = 0;

                if (armed) {
                    armed = false;
                    id = ++s_next_id;
                }

                return typeseq_impl(id);

            }

            /** 'anonymous' sentinel type.
             *  Niche uses for this, e.g. untyped allocator
             **/
            static typeseq_impl<Tag> anon() {
                return typeseq_impl(-1);
            }

            int32_t seqno() const { return seqno_; }

        private:
            static int32_t s_next_id;

            int32_t seqno_;
        };

        template <typename Tag>
        int32_t typeseq_impl<Tag>::s_next_id = 0;

        template <typename Tag>
        inline bool
        operator==(const typeseq_impl<Tag> & lhs, const typeseq_impl<Tag> & rhs) {
            return lhs.seqno() == rhs.seqno();
        }

        template <typename Tag>
        inline bool
        operator!=(const typeseq_impl<Tag> & lhs, const typeseq_impl<Tag> & rhs) {
            return lhs.seqno() != rhs.seqno();
        }

        template <typename Tag>
        inline std::ostream &
        operator<<(std::ostream & s, const typeseq_impl<Tag> & x) {
            s << x.seqno();
            return s;
        }

        using typeseq = typeseq_impl<>;
    } /*namespace reflect*/
} /*namespace xo*/

/* end typeseq.hpp */
