/* file pretty_tag.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "pretty.hpp"
#include "tag.hpp"

namespace xo {
    namespace print {
        template <bool PrefixSpace, tagstyle TagStyle, typename Name, typename Value>
        struct ppdetail<tag_impl<PrefixSpace, TagStyle, Name, Value>> {
            static bool print_upto(ppstate * pps, const tag_impl<PrefixSpace, TagStyle, Name, Value> & tag) {
                std::uint32_t saved = pps->pos();
                if (!pps->has_margin())
                    return false;

                if (PrefixSpace)
                    pps->write(" ");

                /* skil colorizing here, since doesn't consume visible space */
                if (!pps->print_upto(concat((char const *)":", concat(tag.name(), (char const *)" "))))
                    return false;

                if (!pps->print_upto(quot_impl(TagStyle == tagstyle::autoescape, tag.value())))
                    return false;

                return pps->scan_no_newline(saved);
            }

            static void print_pretty(ppstate * pps, const tag_impl<PrefixSpace, TagStyle, Name, Value> & tag) {
                /* pretty-print like
                 *   :somename
                 *      pretty(value)
                 */
                std::uint32_t ci0 = pps->lpos();
                std::uint32_t ci1 = ci0 + pps->indent_width();

                if (PrefixSpace)
                    pps->write(" ");
                pps->write(with_color(tag_config::tag_color,
                                      concat((char const *)":", tag.name())));

                pps->newline_indent(ci1);
                pps->pretty(tag.value());

                pps->newline_indent(ci0);
            }
        };

    } /*namespace print*/
} /*namespace xo*/

/* end pretty_tag.hpp */
