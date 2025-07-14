/* file pretty_vector.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "pretty.hpp"
#include "pad.hpp"
#include <vector>

namespace xo {
    namespace print {
        template <typename T>
        struct ppdetail<std::vector<T>> {
            static bool print_upto(ppstate * pps, const std::vector<T> & x) {
                std::uint32_t saved = pps->pos();
                if (!pps->has_margin())
                    return false;

                pps->write("[");
                for (size_t i = 0, z = x.size(); i < z; ++i) {
                    if (i > 0)
                        pps->write(", ");

                    if (!pps->print_upto(x[i]))
                        return false;
                    if (!pps->has_margin())
                        return false;
                }
                pps->write("]");
                if (!pps->has_margin())
                    return false;

                return pps->scan_no_newline(saved);
            }

            static void print_pretty(ppstate * pps, const std::vector<T> & x) {
                std::uint32_t ci0 = pps->lpos();

                pps->write('[');

                std::uint32_t ci1 = ci0 + pps->indent_width();
                for (size_t i = 0, z = x.size(); i < z; ++i) {
                    pps->newline_indent(ci1);
                    pps->pretty(x[i]);

                    if (i+1 < z)
                        pps->write(',');
                }

                pps->newline_indent(ci0);
                pps->write(']');
            }
        };

        template <typename T>
        struct ppdetail<const std::vector<T>> {
            static bool print_upto(ppstate * pps, const std::vector<T> & x) {
                return ppdetail<std::vector<T>>::print_upto(pps, x);
            }
            static void print_pretty(ppstate * pps, const std::vector<T> & x) {
                ppdetail<std::vector<T>>::print_pretty(pps, x);
            }
        };
    }
}
