/* ppdetail_atomic.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include <string>
#include <cstdint>

namespace xo {
    namespace print {
        struct ppstate; // see pretty.hpp

// Defining this means ppdetail_atomic is not used.
// In that case where not explicitly specialized ppdetail will revert to ordinary printing for a type,
// instead of giving compile-time error about missing template specialization of ppdetail

#define ppdetail_atomic ppdetail

        /** @class ppdetail
         *  @brief template for opt-in to pretty-printer
         *
         *  Provide a specialization that covers each type you want participating in pretty-printing.
         *  Default treats T as atomic.
         *
         *  Expectations for specializers:
         *  - don't print final newline:
         *    - parent container will do that
         *    - want option to put closing ),],>,} on the same line
         **/
        template <typename T>
        struct ppdetail;

        template <typename T>
        struct ppdetail_atomic;

        template <typename T>
        struct ppdetail_atomic {
            /** 1. print @p x to private stream @ref scratch_ss_
             *  2. return true iff N = number of characters printed satisfies N <= @p budget.
             *     content actually printed to *sbuf may be used as-is in this case
             *  3. return false otherwise.  Will trigger non-degenerate pretty-printing.
             *  4. in any case consume some of @ref scratch_sbuf_
             **/
            static bool print_upto(ppstate * pps, const T & x);
            /** print @p x to private stream @ref scratch_ss_ **/
            static void print_pretty(ppstate * pps, const T & x);
        };

#ifndef ppdetail_atomic
        template <int N>
        struct ppdetail<const char[N]> {
            using target_type = const char[N];

            static bool print_upto(ppstate * pps, const target_type & x) {
                return ppdetail_atomic<target_type>::print_upto(pps, x);
            }
            static void print_pretty(ppstate * pps, const target_type & x) {
                ppdetail_atomic<target_type>::print_pretty(pps, x);
            }
        };

        template <>
        struct ppdetail<const char *> {
            static bool print_upto(ppstate * pps, const char * x) {
                return ppdetail_atomic<const char *>::print_upto(pps, x);
            }
            static void print_pretty(ppstate * pps, const char * x) {
                ppdetail_atomic<const char *>::print_pretty(pps, x);
            }
        };


#define PPDETAIL_ATOMIC_BODY(target_type)                                         \
        struct ppdetail<target_type> {                                            \
            static bool print_upto(ppstate * pps, const target_type & x) {        \
                return ppdetail_atomic<target_type>::print_upto(pps, x);          \
            }                                                                     \
                                                                                  \
            static void print_pretty(ppstate * pps, const target_type & x) {      \
                ppdetail_atomic<target_type>::print_pretty(pps, x);               \
            }                                                                     \
        }

#define PPDETAIL_ATOMIC_BODY_CONST(target_type)                                   \
        struct ppdetail<const target_type> {                                      \
            static bool print_upto(ppstate * pps, const target_type & x) {        \
                return ppdetail_atomic<target_type>::print_upto(pps, x);          \
            }                                                                     \
                                                                                  \
            static void print_pretty(ppstate * pps, const target_type & x) {      \
                ppdetail_atomic<target_type>::print_pretty(pps, x);               \
            }                                                                     \
        }

#define PPDETAIL_ATOMIC(target_type)                                              \
        template<>                                                                \
        PPDETAIL_ATOMIC_BODY(target_type)

#define PPDETAIL_ATOMIC_CONST(target_type)                                        \
        template<>                                                                \
        PPDETAIL_ATOMIC_BODY_CONST(target_type)

        PPDETAIL_ATOMIC(bool);
        PPDETAIL_ATOMIC(char);
//        PPDETAIL_ATOMIC_CONST(char);
        PPDETAIL_ATOMIC(std::int64_t);
        PPDETAIL_ATOMIC(std::uint64_t);
        PPDETAIL_ATOMIC(std::int32_t);
        PPDETAIL_ATOMIC(std::uint32_t);
        PPDETAIL_ATOMIC(std::string);

        using voidptr = void*;
        PPDETAIL_ATOMIC(voidptr);
#endif

    }
}
