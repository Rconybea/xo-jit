/** @file Binding.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <iostream>
#include <cstdint>

namespace xo {
    namespace scm {
        class Binding {
        public:
            using slot_type = int32_t;

            static constexpr int32_t c_link_sentinel = -2;
            static constexpr int32_t c_link_global = -1;

        public:
            Binding() : i_link_{-2}, j_slot_{-1} {}
            Binding(int32_t i_link, int32_t j_slot)
                : i_link_{i_link}, j_slot_{j_slot} {}

            static Binding null() { return Binding(); }
            /** global bindings are located by symbol name **/
            static Binding global(int32_t j_slot) { return Binding(c_link_global, j_slot); }
            static Binding local(int32_t j_slot) { return Binding(0, j_slot); }
            static Binding relative(int32_t i_link, Binding def);

            bool is_null() const {
                return (i_link_ == c_link_sentinel) && (j_slot_ == -1);
            }
            bool is_global() const { return i_link_ == c_link_global; }
            bool is_local() const { return (i_link_ == 0) && (j_slot_ >= 0); }

            int32_t i_link() const noexcept { return i_link_; }
            int32_t j_slot() const noexcept { return j_slot_; }

            /** print human-readable repr to stream @p os **/
            void print(std::ostream & os) const;

        private:
            /**
             *  >= 0: number of parent links to traverse
             *        to a fixed-size frame
             *    -1: resolve globally
             *    -2: sentinel (binding info not computed)
             **/
            int32_t i_link_ = c_link_sentinel;
            /** if @ref i_link_ >= 0, frame offset
             *  (in 'variables' not bytes).
             *  ignored if @ref i_link_ is global
             **/
            int32_t j_slot_ = -1;
        };

        inline std::ostream & operator<< (std::ostream & os, Binding x) {
            x.print(os);
            return os;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* Binding.hpp */
