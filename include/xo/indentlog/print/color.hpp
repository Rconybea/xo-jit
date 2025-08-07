/* color.hpp */

#pragma once

#include "ppdetail_atomic.hpp"
#include <ostream>
#include <cstdint>

namespace xo {
    enum class color_encoding : std::uint8_t {
        none,
        ansi,
        xterm,
        rgb
    };

    inline std::ostream &
    operator<< (std::ostream & os, color_encoding x) {
        switch(x) {
        case color_encoding::none:  os << "none";  break;
        case color_encoding::ansi:  os << "ansi";  break;
        case color_encoding::xterm: os << "xterm"; break;
        case color_encoding::rgb:   os << "rgb";   break;
        default:                    os << "???";   break;
        }
        return os;
    } /*operator<<*/

    /* specify a color (consistent with ANSI escape sequences - the  Select Graphics Rendition subset
     * see [[https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences]]
     *
     * this provides three ways to specify foreground color:
     *
     *   | enum  | escape    | example             | description   | foreground codes |
     *   +-------+-----------+---------------------+---------------+------------------+
     *   | ansi  | \033[31   | \033[31;34m         | 4-bit colors  | 30..37, 90..97   |
     *   | xterm | \033[38;5 | \033[38;5;143m      | 8-bit colors  | 0..255           |
     *   | rgb   | \033[38;2 | \033[38;2;10;20;30m | 24-bit colors | 3x 0..255        |
     *
     */
    class color_spec_type {
    public:
        color_spec_type() = default;
        color_spec_type(color_encoding encoding, std::uint32_t code)
            : encoding_{encoding}, code_{code} {}

        static color_spec_type none() { return color_spec_type(); }
        static color_spec_type ansi(std::uint32_t code) { return color_spec_type(color_encoding::ansi, code); }
        static color_spec_type xterm(std::uint32_t code) { return color_spec_type(color_encoding::xterm, code); }
        static color_spec_type rgb(std::uint8_t red, std::uint8_t green, std::uint8_t blue) {
            return color_spec_type(color_encoding::rgb, (red << 16 | green << 8 | blue));
        }

        /* 4-bit foreground colors */
        static color_spec_type black          () { return ansi(30); }
        static color_spec_type red            () { return ansi(31); }
        static color_spec_type green          () { return ansi(32); }
        static color_spec_type yellow         () { return ansi(33); }
        static color_spec_type blue           () { return ansi(34); }
        static color_spec_type magenta        () { return ansi(35); }
        static color_spec_type cyan           () { return ansi(36); }
        static color_spec_type white          () { return ansi(37); }
        static color_spec_type bright_black   () { return ansi(90); }
        static color_spec_type bright_red     () { return ansi(91); }
        static color_spec_type bright_green   () { return ansi(92); }
        static color_spec_type bright_yellow  () { return ansi(99); }
        static color_spec_type bright_blue    () { return ansi(94); }
        static color_spec_type bright_magenta () { return ansi(95); }
        static color_spec_type bright_cyan    () { return ansi(96); }
        static color_spec_type bright_white   () { return ansi(97); }

        color_encoding encoding() const { return encoding_; }
        std::uint32_t code() const { return code_; }

        void print_fg_color_on (std::ostream & os) const {
            switch (encoding_) {
            case color_encoding::none:
                break;
            case color_encoding::ansi:
                os << "\033[31;" << code_ << "m";
                break;
            case color_encoding::xterm:
                os << "\033[38;5;" << code_ << "m";
                break;
            case color_encoding::rgb:
                os << "\033[38;2;"
                   << (0xff & (code_ >> 16)) << ";"
                   << (0xff & (code_ >> 8)) << ";"
                   << (0xff & (code_ >> 0)) << "m";
            }
        } /*print_fg_color_on*/

        /* escape to reverse effect of .print_on() */
        void print_fg_color_off (std::ostream & os) const {
            switch (encoding_) {
            case color_encoding::none:
                break;
            case color_encoding::ansi:
            case color_encoding::xterm:
            case color_encoding::rgb:
                os << "\033[0m";
                break;
            }
        } /*print_fg_color_off*/

    private:
        /* none | ansi | xterm | rgb */
        color_encoding encoding_ = color_encoding::none;
        /* ansi  : 30..37, 90..97
         * xterm : 0..255
         *   see [[https://i.stack.imgur.com/KTSQa.png]]
         *   0..7               standard colors (muted: grey, red, green, yellow, blue, pink, cyan, white)
         *   8..15              high-intensity colors (grey, red, green, yellow, blue, pink, cyan, white)
         *   16..51             chooses hue
         *   16..51 + (0..5)x36 increases whiteness
         * rgb   : r={hi 8 bits}, g={mid 8 bits}, b={lo 8 bits}
         */
        std::uint32_t code_ = 0;
    }; /*color_spec_type*/

    inline std::ostream &
    operator<< (std::ostream & os, color_spec_type const & x) {
        os << "<color_spec_type :encoding " << x.encoding() << " :code " << x.code() << ">";
        return os;
    } /*operator<<*/

    enum class coloring_control_flags : std::uint8_t {
        none      = 0x0,
        color_on  = 0x01,
        contents  = 0x02,
        color_off = 0x04,
        all       = 0x07
    };

    inline std::uint8_t operator& (coloring_control_flags x, coloring_control_flags y) {
        return static_cast<std::uint8_t>(x) & static_cast<std::uint8_t>(y);
    }
    inline std::uint8_t operator| (coloring_control_flags x, coloring_control_flags y) {
        return static_cast<std::uint8_t>(x) | static_cast<std::uint8_t>(y);
    }

    /* stream-insertable color control */
    template <typename Contents>
    class color_impl {
    public:
        color_impl(coloring_control_flags flags, color_spec_type spec, Contents && contents)
            : flags_{flags},
              spec_{spec}, contents_{std::forward<Contents>(contents)} {}

        color_spec_type const & spec() const { return spec_; }
        std::uint32_t color() const { return spec_.code(); }
        Contents const & contents() const { return contents_; }

        void print(std::ostream & os) const {
            if (flags_ & coloring_control_flags::color_on)
                spec_.print_fg_color_on(os);

            if (flags_ & coloring_control_flags::contents)
                os << contents_;

            if (flags_ & coloring_control_flags::color_off)
                spec_.print_fg_color_off(os);
        } /*print*/

    private:
        /** controls independently what to print
         *     \033[38;5;117m hello, world! \033[0m
         *     <------------> <-----------> <----->
         *        color_on       contents    color_off
         **/
        coloring_control_flags flags_ = coloring_control_flags::none;

        /** color to use, when @ref color_enabled_ is on **/
        color_spec_type spec_;

        /** contents to print surrounded by color escapes **/
        Contents contents_;
    }; /*color_impl*/

    template <typename Contents>
    color_impl<Contents> with_color_if(bool color_enabled, color_spec_type const & spec, Contents && contents) {
        return color_impl<Contents>((color_enabled
                                     ? coloring_control_flags::all
                                     : coloring_control_flags::contents),
                                    spec,
                                    std::forward<Contents>(contents));
    }

    template <typename Contents>
    color_impl<Contents> with_color(color_spec_type const & spec, Contents && contents) {
        return color_impl<Contents>(coloring_control_flags::all,
                                    spec,
                                    std::forward<Contents>(contents));
    }

    inline color_impl<int>
    color_on(color_spec_type const & spec) {
        return color_impl<int>(coloring_control_flags::color_on,
                               spec,
                               0);
    }

    inline color_impl<int>
    color_off(color_spec_type const & spec) {
        /* any spec other than color_spec_type::none() works here */
        return color_impl<int>(coloring_control_flags::color_off,
                               spec,
                               0);
    }

    template <typename Contents>
    inline std::ostream &
    operator<<(std::ostream & os, color_impl<Contents> const & x) {
        x.print(os);
        return os;
    }

#ifndef ppdetail_atomic
    namespace print {
        /* concat expected to be used on short string-like things.
         * i.e. don't want structure visible to pretty-printer.
         * could be using it like concat("boeing", 777)
         */
        template <typename Contents>
        struct ppdetail<color_impl<Contents>> {
            using target_type = color_impl<Contents>;

            static bool print_pretty(const ppindentinfo & ppii, const target_type & x) {
                return ppdetail_atomic<target_type>::print_pretty(ppii, x);
            }
        };
    }
#endif

} /*namespace xo*/

/* end color.hpp */
