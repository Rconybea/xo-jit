/* @file tag.hpp */

#pragma once

#include "tag_config.hpp"
#include "concat.hpp"
#include "quoted.hpp"
#include "color.hpp"
#include <iostream>

// STRINGIFY(xyz) -> "xyz"
#ifndef STRINGIFY
# define STRINGIFY(x) #x
#endif

// TAG(xyz) -> tag("xyz", xyz)
#define TAG(x) xo::make_tag(STRINGIFY(x), x)
#define TAG2(x, y) xo::make_tag(x, y)

#define XTAG(x) xo::xtag(STRINGIFY(x), x)

namespace xo {
    enum class tagstyle {
        /** print with automatic escapes for embedded special characters
         *  (any of ' ','"','\','\n','\r','\t').
         *  print inside double-quotes if an escape is required.
         **/
        autoescape,
        /** print literally. caller responsible for machine-readability **/
        raw,
    };

    /** K,V pair for printing.
     *
     *  @tparam PrefixSpace if true print one space before :K
     *  @tparam TagStyle    controls printing format
     *  @tparam Name        typename for key K.
     *  @tparam Value       typename for value V.
     *
     *  1. Uses escapes to preserve machine-readability of V.
     *  2. Optionally colors K for readability.
     *     Can disable with
     *     @code
     *     xo::tag_config::tag_color = xo::color_spec_type::none()
     *     @endcode
     *
     *  Printing styles
     *    @pre
     *    :name value                     // unadorned
     *    :name "value needing\nescapes"  // with escapes
     *    :name <foo :nested 1 :param 2>  // autoescape disabled
     *    @endpre
     *
     * NOTE: will search for operator<< overloads in the xo
     *       namespace
     *
     *
     **/
    template <bool PrefixSpace, tagstyle TagStyle, typename Name, typename Value>
    struct tag_impl {
        tag_impl(Name const & n, Value const & v)
            : name_{n}, value_{v} {}
        tag_impl(Name && n, Value && v)
            : name_{std::forward<Name>(n)}, value_{std::forward<Value>(v)} {}

        Name const & name() const { return name_; }
        Value const & value() const { return value_; }

    private:
        Name name_;
        Value value_;
    };

    // ----- xtag -----

    template<typename Name, typename Value>
    auto //tag_impl<true, tagstyle::autoescape, Name, std::decay_t<Value>>
    xtag(Name && n, Value && v)
    {
        return tag_impl<true, tagstyle::autoescape, Name, std::decay_t<Value>>(n, v);
    } /*xtag*/

    template<typename Value>
    auto //tag_impl<true, tagstyle::autoescape, char const *, std::decay_t<Value>>
    xtag(char const * n, Value && v) {
        return tag_impl<true, tagstyle::autoescape, char const *, std::decay_t<Value>>(n, v);
    } /*xtag*/

    // ----- tag -----

    template<typename Name, typename Value>
    tag_impl<false, tagstyle::autoescape, Name, Value>
    tag(Name && n, Value && v)
    {
        return tag_impl<false, tagstyle::autoescape, Name, Value>(n, v);
    }

    template<typename Value>
    tag_impl<false, tagstyle::autoescape, char const *, Value>
    tag(char const * n, Value && v)
    {
        return tag_impl<false, tagstyle::autoescape, char const *, Value>(n, v);
    }

    // ----- operator<< on tag_impl -----

    template <bool PrefixSpace, tagstyle TagStyle, typename Name, typename Value>
    inline std::ostream &
    operator<<(std::ostream &s,
               tag_impl<PrefixSpace, TagStyle, Name, Value> const & tag)
    {
        using xo::print::unq;

        if (PrefixSpace)
            s << " ";

        s << with_color(tag_config::tag_color, concat((char const *)":", tag.name()))
          << " ";

        if (TagStyle == tagstyle::autoescape)
            s << unq(tag.value());
        else
            s << tag.value();

        return s;
    } /*operator<<*/
} /*namespace xo*/

/* end tag.hpp */
