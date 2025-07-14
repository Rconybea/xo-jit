/* file ppstr.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "xo/indentlog/print/pretty.hpp"
#include "xo/indentlog/print/pretty_vector.hpp"
#include <iostream>
#include <sstream>
#include <string>

namespace xo {

    /** @class ppconcat
     *  @brief Container for a tuple of values to be pretty-printed in order
     *  @tparam Args... parameter pack of argument types
     *
     *  We don't use the tuple directly, so that we don't inadvertently
     *  usurp pretty-printing behavior for all tuples.
     *
     **/
    template <typename... Args>
    struct ppconcat {
        /** tuple to be pretty-printed.  See toppstr(), toppstr2() **/
        std::tuple<Args...> contents_;
    };

    namespace print {
        namespace detail {
            inline bool
            ppconcat_printupto_aux(print::ppstate *) {
                return true;
            }

            template <typename T>
            bool
            ppconcat_printupto_aux(print::ppstate * pps, T && x) {
                if (!pps->print_upto(x))
                    return false;
                if (!pps->has_margin())
                    return false;
                return true;
            }

            template <typename T, typename... Tn>
            bool
            ppconcat_printupto_aux(print::ppstate * pps, T && x, Tn && ...rest) {
                if (!pps->print_upto(x))
                    return false;
                if (!pps->has_margin())
                    return false;
                return ppconcat_printupto_aux(pps, rest...);
            }

            inline void
            ppconcat_print_pretty_rest(print::ppstate *, std::uint32_t) {}

            template <typename T>
            void
            ppconcat_print_pretty_rest(print::ppstate * pps, std::uint32_t ci1, T && x) {
                pps->newline_indent(ci1);
                pps->pretty(x);
            }

            template <typename T, typename... Tn>
            void
            ppconcat_print_pretty_rest(print::ppstate * pps, std::uint32_t ci1, T && x, Tn && ...rest) {
                pps->newline_indent(ci1);
                pps->pretty(x);

                ppconcat_print_pretty_rest(pps, ci1, rest...);
            }

            inline void
            ppconcat_print_pretty_aux(print::ppstate *, std::uint32_t /*ci1*/) {}

            template <typename T, typename... Tn>
            void
            ppconcat_print_pretty_aux(print::ppstate * pps, std::uint32_t ci1, T && x, Tn && ...rest) {
                // first item doesn't get extra indent.  Want pretty output like:
                //
                //   pretty(first)
                //     pretty(second)
                //     pretty(third)
                //     ..
                //
                pps->pretty(x);
                ppconcat_print_pretty_rest(pps, ci1, rest...);
            }
        }

        /** implement pretty-printing for template @c ppconcat r**/
        template <typename... Args>
        struct ppdetail<ppconcat<Args...>> {
            /** try to print @p target on one line.
             *  return false if budget (space until right margin) exhausted
             *  or if an embedded newline appears
             *
             *  @return true on success, otherwise false.
             **/
            static bool print_upto(ppstate * pps, ppconcat<Args...> target) {
                std::uint32_t saved = pps->pos();

                if (!pps->has_margin())
                    return false;

                if (std::apply(
                        [pps](auto &&... args) {
                            return detail::ppconcat_printupto_aux(pps, std::forward<decltype(args)>(args)...);
                        },
                        std::forward<std::tuple<Args...>>(target.contents_)
                        ) == false)
                {
                    return false;
                }

                return pps->scan_no_newline(saved);
            }

            /** pretty-print @p target using multiple lines
             **/
            static void print_pretty(ppstate * pps, ppconcat<Args...> target) {
                std::uint32_t ci0 = pps->lpos();
                std::uint32_t ci1 = ci0 + pps->indent_width();

                std::apply(
                    [pps, ci1](auto &&... args) {
                        detail::ppconcat_print_pretty_aux(pps, ci1,
                                                          std::forward<decltype(args)>(args)...);
                    },
                    std::forward<std::tuple<Args...>>(target.contents_)
                    );
            }
        };
    } /*namespace print*/

    /** @return string comprised of pretty-printed sequence of values in @p args
     *
     *  Example:
     *  @code
     *    toppstr("hello") -> "hello"
     *    toppstr("string1", .., "string99")
     *      -> "string1
     *            string2
     *            ...
     *            string99"
     *  @endcode
     **/
    template <typename... Tn>
    std::string toppstr(Tn&&... args) {
        print::ppconfig ppc;

        std::stringstream ss;
        print::ppstate_standalone pps(&ss, 0, &ppc);

        // - std::decay_t<T> remove reference and cv-qualifiers
        // - brace initialization of ppconcat and its contained tuple
        pps.pretty(ppconcat<std::decay_t<Tn>...>{{std::forward<Tn>(args)...}});

        return ss.str();
    }

    /** like @ref toppstr, but use pretty-printing configuration @p ppc
     *
     *  Example:
     *  @code
     *    ppconfig ppc{.right_margin_=20, .indent_width_=4};
     *    toppstr2("string1", "string2", "string3") ->
     *    "string1
     *         string2
     *         string3"
     *  @endcode
     **/
    template <typename... Tn>
    std::string toppstr2(const print::ppconfig& ppc, Tn&&... args) {
        std::stringstream ss;
        print::ppstate_standalone pps(&ss, 0, &ppc);

        // - std::decay_t<T> remove reference and cv-qualifiers
        // - brace initialization of ppconcat and its contained tuple
        pps.pretty(ppconcat<std::decay_t<Tn>...>{{std::forward<Tn>(args)...}});

        return ss.str();
    }

} /*namespace xo*/

/* end ppstr.hpp */
