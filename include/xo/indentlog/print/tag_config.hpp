/* @file tag_config.hpp */

#pragma once

#include "color.hpp"
#include <cstdint>

namespace xo {
    /** @class tag_config_impl
     *  @brief configuration for tag-printing
     *
     *  note: Tag here b/c we want header-only library
     **/
    template <typename Tag>
    struct tag_config_impl {
        /** true to enable colored tag printing **/
        static bool tag_color_enabled;
        /** color to use for tags
         *    os << tag("foo", foovalue)
         * to produces output like
         *    :foo foovalue
         * with :foo using .tag_color
         **/
        static color_spec_type tag_color;
    }; /*tag_config_impl*/

    template <typename Tag>
    bool
    tag_config_impl<Tag>::tag_color_enabled = true;

    /** default value is a light gray **/
    template <typename Tag>
    color_spec_type
    tag_config_impl<Tag>::tag_color = color_spec_type::xterm(245);

    using tag_config = tag_config_impl<class tag_config_tag>;
} /*namespace xo*/

/* end tag_config.hpp */
