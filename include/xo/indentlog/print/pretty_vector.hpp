/* file pretty_vector.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "pretty.hpp"
#include "array.hpp" /*printing*/
#include "pad.hpp"
#include <vector>
#include <array>
#include <cstdint>

namespace xo {
    namespace print {
        template <typename Vector>
        struct ppdetail_vector {
            static bool print_pretty(const ppindentinfo & ppii, const Vector & x) {
                if (ppii.upto()) {
                    ppii.pps()->write("[");
                    for (size_t i = 0, z = x.size(); i < z; ++i) {
                        if (i > 0)
                            ppii.pps()->write(", ");

                        if (!ppii.pps()->print_upto(x[i]))
                            return false;
                        if (!ppii.pps()->has_margin())
                            return false;
                    }
                    ppii.pps()->write("]");

                    return true;
                } else {
                    ppii.pps()->write('[');

                    for (size_t i = 0, z = x.size(); i < z; ++i) {
                        if (i == 0)
                            ppii.pps()->indent(std::max(ppii.pps()->indent_width(), 1u) - 1);
                        else
                            ppii.pps()->newline_indent(ppii.ci1());
                        ppii.pps()->pretty(x[i]);

                        if (i+1 < z)
                            ppii.pps()->write(',');
                    }

                    ppii.pps()->write(" ]");
                    return false;
                }
            }
        };

        template <typename T>
        struct ppdetail<std::vector<T>> {
            static bool print_pretty(const ppindentinfo & ppii, const std::vector<T> & x) {
                return ppdetail_vector<std::vector<T>>::print_pretty(ppii, x);
            }
        };

        template <typename T, std::size_t N>
        struct ppdetail<std::array<T, N>> {
            static bool print_pretty(const ppindentinfo & ppii, const std::array<T,N> & x) {
                return ppdetail_vector<std::array<T, N>>::print_pretty(ppii, x);
            }
        };
    }
}
