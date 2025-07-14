/* @file log_config.hpp */

#pragma once

#include "log_level.hpp"
#include "print/function.hpp"
#include "print/color.hpp"
#include <cstdint>

namespace xo {
    /* Tag here because we want header-only library */
    template <typename Tag>
    struct log_config_impl {
        /* display log messages with severity >= .log_level */
        static log_level min_log_level;
        /* true to log local time */
        static bool time_enabled;
        /* true to log time-of-day in local coords;  false for UTC coords */
        static bool time_local_flag;
        /* true to log time-of-day with microsecond precision;  false for millisecond precision */
        static bool time_usec_flag;
        /* true to enable pretty-printing (see basic_scope::log()) */
        static bool pretty_print_enabled;
        /* when pretty-printing enabled, use newlines in effort to avoid writing beyond this margin */
        static std::uint32_t right_margin;
        /* spaces per nesting level.  0 -> no indenting */
        static std::uint32_t indent_width;
        /* max #of spaces to introduce when indenting */
        static std::uint32_t max_indent_width;
        /* if true enable explicit nesting level display [nnn] */
        static bool nesting_level_enabled;
        /* color to use for explicit nesting level */
        static color_spec_type nesting_level_color;
        /* display style for function names. function_style:: literal|simple|pretty|streamlined */
        static function_style style;
        /* color to use for function name,  on entry/exit (xo::scope creation/destruction)
         * (ansi color codes,  see Select Graphics Rendition subset)
         */
        static color_spec_type function_entry_color;
        static color_spec_type function_exit_color;
        /* if true,  append [file:line] to output */
        static bool location_enabled;
        /* when .location_enabled,  write [file:line] starting this many chars from left margin */
        static std::uint32_t location_tab;
        /* color to use for code location */
        static color_spec_type code_location_color;
    }; /*log_config_impl*/

    template <typename Tag>
    log_level
    log_config_impl<Tag>::min_log_level = log_level::default_level;

    template <typename Tag>
    bool
    log_config_impl<Tag>::time_enabled = 1;

    template <typename Tag>
    bool
    log_config_impl<Tag>::time_local_flag = true;

    template <typename Tag>
    bool
    log_config_impl<Tag>::time_usec_flag = true;

    template <typename Tag>
    bool
    log_config_impl<Tag>::pretty_print_enabled = true;

    template <typename Tag>
    std::uint32_t
    log_config_impl<Tag>::right_margin = 130;

    template <typename Tag>
    std::uint32_t
    log_config_impl<Tag>::indent_width = 2;

    template <typename Tag>
    std::uint32_t
    log_config_impl<Tag>::max_indent_width = 32;

    template <typename Tag>
    bool
    log_config_impl<Tag>::nesting_level_enabled = true;

    template <typename Tag>
    color_spec_type
    log_config_impl<Tag>::nesting_level_color = color_spec_type::xterm(195);

    template <typename Tag>
    function_style
    log_config_impl<Tag>::style = function_style::streamlined;

    template <typename Tag>
    color_spec_type
    log_config_impl<Tag>::function_entry_color = color_spec_type::ansi(34);

    template <typename Tag>
    color_spec_type
    log_config_impl<Tag>::function_exit_color = color_spec_type::ansi(32);

    template <typename Tag>
    bool
    log_config_impl<Tag>::location_enabled = true;

    template <typename Tag>
    std::uint32_t
    log_config_impl<Tag>::location_tab = 80;

    template <typename Tag>
    color_spec_type
    log_config_impl<Tag>::code_location_color = color_spec_type::red();

    using log_config = log_config_impl<class log_config_tag>;
} /*namespace xo*/

/* end log_config.hpp */
