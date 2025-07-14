/* @file pad.hpp */

#pragma once

#include <iostream>
#include <cstdint>

namespace xo {
    /* use:
     *   ostream os = ...;
     *
     * 1.
     *   os << ":" << pad(8) << ":"
     *
     * writes
     *   :        :
     *
     * 2.
     *   os << pad(16, '-')
     *
     * writes
     *   ----------------
     *
     * on os
     */
    class pad_impl {
    public:
        pad_impl(std::uint32_t n, char pad_char) : n_pad_{n}, pad_char_{pad_char} {}

        std::uint32_t n_pad() const { return n_pad_; }
        char pad_char() const { return pad_char_; }

    private:
        std::uint32_t n_pad_ = 0;
        char pad_char_ = '\0';
    }; /*pad_impl*/

    inline pad_impl
    pad(std::uint32_t n, char pad_char = ' ') { return pad_impl(n, pad_char); }

    inline pad_impl
    spaces(std::uint32_t n) { return pad_impl(n, ' '); }

    inline std::ostream &
    operator<<(std::ostream & s,
               pad_impl const & pad)
    {
        for(std::uint32_t i=0; i<pad.n_pad(); ++i)
            s << pad.pad_char();
        return s;
    } /*operator<<*/

} /*namespace xo*/

/* end pad.hpp */
