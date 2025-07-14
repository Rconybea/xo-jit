/** @file hex.hpp **/

#pragma once

#include "ppdetail_atomic.hpp"
#include <iostream>
#include <cstdint>

namespace xo {
    /**
       @class hex indentlog/print/hex.hpp

       @brief Container for a (1-byte) value to be printed in hexadecimal

       Example:
       @code
       #include "indentlog/print/hex.hpp"

       std::ostream os = ...;
       os << hex(16 + 63);        // output: 1f
       os << hex(16 + 63, true);  // output: 1f(O)
       @endcode
    **/
    struct hex {
        /** @brief constructor;  create stream-inserter instance */
        explicit hex(std::uint8_t x, bool w_char = false) : x_{x}, with_char_{w_char} {}

        /**
           @brief print hexadecimal byte-value on to stream.
           @param os   print on this stream.

           @tparam Stream  typename for character stream.
        **/
        template <typename Stream>
        void print(Stream & os) const {
            std::uint8_t lo = x_ & 0xf;
            std::uint8_t hi = x_ >> 4;

            char lo_ch = (lo < 10) ? '0' + lo : 'a' + lo - 10;
            char hi_ch = (hi < 10) ? '0' + hi : 'a' + hi - 10;

            os << hi_ch << lo_ch;

            if (with_char_) {
                os << "(";
                if (std::isprint(x_))
                    os << static_cast<char>(x_);
                else
                    os << "?";
                os << ")";
            }
        }

    private:
        /** @brief value to print (in hexadecimal) **/
        std::uint8_t x_;
        /** @brief if true, follow with ascii character encoding **/
        bool with_char_;
    };

    /**
       @brief stream inserter for an 8-bit quantity to be printed in hexadecimal.

       @param os    print on this stream
       @param ins   package for value to insert
    **/
    inline std::iostream &
    operator<< (std::iostream & os, hex const & ins) {
        ins.print(os);
        return os;
    }

    /**
       @class hex_view indentlog/print/hex.hpp

       @brief Container for a range (unowned) of 1-byte values to be printed in hexadecimal

       Print a range of bytes on an arbitrary character stream.
       Does not use @c iomanips, so will not alter stream formatting flags if used with @c iostream.

       Example:
       @code
       #include "indentlog/print/hex.hpp"

       std::ostream os = ...;
       os << hex_view("hello", false); // output: [68 65 6c 6c 6f]
       os << hex_view("hello", true);  // output: [68(h) 65(e) 6c(l) 6c(l) 6f(o)]
       @endcode
    **/
    struct hex_view {
        /** @brief constructor;  create stream-inserter instance for a range of bytes **/
        hex_view(std::uint8_t const * lo, std::uint8_t const * hi, bool as_text)
            : lo_{lo}, hi_{hi}, as_text_{as_text} {}
        /** @brief constructor;  create stream-inserter instance for a range of chars **/
        hex_view(char const * lo, char const * hi, bool as_text)
            : lo_{reinterpret_cast<std::uint8_t const *>(lo)},
              hi_{reinterpret_cast<std::uint8_t const *>(hi)},
              as_text_{as_text} {}

        /**
           @brief print hexadecimal byte range on stream.
           @param os print on this stream

           @tparam Stream typename for character stream.
        **/
        template <typename Stream>
        void print(Stream & os) const {
            os << "[";
            std::size_t i = 0;
            for (std::uint8_t const * p = lo_; p < hi_; ++p) {
                if (i > 0)
                    os << " ";
                xo::hex(*p, as_text_).print(os);
                //os << xo::hex(*p, as_text_);
                ++i;
            }
            os << "]";
        }

    private:
        /** @brief print byte range starting at this address **/
        std::uint8_t const * lo_;
        /** @brief print byte range up to (but not including) this address **/
        std::uint8_t const * hi_;
        /** @brief if true also print ascii encoding (for printable codes),
         *  \c ? otherwise.  @see hex::with_char
         **/
        bool as_text_;
    };

    /**
       @brief stream inserter for a range of 1-byte values to be printed in hexadecimal

       @param os    print on this stream.
       @param ins   (container for) values to insert.
    **/
    template <typename Stream>
    Stream &
    operator<< (Stream & os, hex_view const & ins) {
        ins.print(os);
        return os;
    }

#ifndef ppdetail_atomic
    namespace print {
        PPDETAIL_ATOMIC(hex_view);
    }
#endif

} /*namespace xo*/

/* end hex.hpp */
