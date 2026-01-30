/** @file Binding.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <cstdint>

namespace xo {
    namespace scm {
        class Binding {
        public:
            static constexpr int32_t s_link_sentinel = -2;
            static constexpr int32_t s_link_global = -1;

        public:
            Binding() : i_link_{-2}, j_slot_{-1} {}
            Binding(int32_t i_link, int32_t j_slot)
                : i_link_{i_link}, j_slot_{j_slot} {}

            static Binding null() { return Binding(); }
            /** global bindings are located by symbol name **/
            static Binding global() { return Binding(s_link_global, 0); }
            static Binding local(int32_t j_slot) { return Binding(0, j_slot); }

            bool is_global() const { return i_link_ == s_link_global; }
            bool is_local() const { return (i_link_ == 0) && (j_slot_ >= 0); }

            int32_t i_link() const noexcept { return i_link_; }
            int32_t j_slot() const noexcept { return j_slot_; }

        private:
            /**
             *  >= 0: number of parent links to traverse
             *        to a fixed-size frame
             *    -1: resolve globally
             *    -2: sentinel (binding info not computed)
             **/
            int32_t i_link_ = s_link_sentinel;
            /** if @ref i_link_ >= 0, frame offset
             *  (in 'variables' not bytes).
             *  ignored if @ref i_link_ is global
             **/
            int32_t j_slot_ = -1;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* Binding.hpp */
