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
        struct ppindentinfo;

        // Defining this means ppdetail_atomic -> ppdetail.
        // For debugging suppress the #define; causes specialization of ppdetail_atomic to be required for every T
        // that participates in pretty printing
        //
        // In that case where not explicitly specialized ppdetail will revert to ordinary printing for a type,
        // instead of giving compile-time error about missing template specialization of ppdetail.
        //
#define ppdetail_atomic ppdetail

        struct ppindentinfo {
            ppindentinfo(ppstate * pps, std::uint32_t ci0, std::uint32_t indent_width, bool upto)
                : pps_{pps}, ci0_{ci0}, ci1_{ci0 + indent_width}, upto_{upto} {}

            ppstate * pps() const { return pps_; }
            std::uint32_t ci0_unused() const { return ci0_; }
            std::uint32_t ci1() const { return ci1_; }
            bool upto() const { return upto_; }

        private:
            ppstate * pps_ = nullptr;
            /** current indent **/
            std::uint32_t ci0_ = 0;
            /** ci0 +1 indent level **/
            std::uint32_t ci1_ = 0;
            /**
             *  true  -> print on remainder of current line, unless past right margin
             *  false -> pretty across across multiple lines
             **/
            bool upto_;
        };

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
        struct ppdetail_atomic {
            /** upto=true:
             *  1. print @p x to private stream @ref scratch_ss_
             *  2. return true iff N = number of characters printed satisfies N <= @p budget.
             *     content actually printed to *sbuf may be used as-is in this case
             *  3. return false otherwise.  Will trigger non-degenerate pretty-printing.
             *  4. in any case consume some of @ref scratch_sbuf_
             *
             *  upto=false:
             *  print @p x to private stream @ref scratch_ss_
             **/
            static bool print_pretty(const ppindentinfo & ppii, const T & x);
        };

#ifndef ppdetail_atomic
        template <int N>
        struct ppdetail<const char[N]> {
            using target_type = const char[N];
            static bool print_pretty(const ppindentinfo & ppii, const target_type & x) {
                return ppdetail_atomic<target_type>::print_pretty(ppii, x);
            }
        };

        template <>
        struct ppdetail<const char *> {
            static bool print_pretty(const ppindentinfo & ppii, const char * x) {
                return ppdetail_atomic<const char *>::print_pretty(ppii, x);
            }
        };


#define PPDETAIL_ATOMIC_BODY(target_type)                                   \
        struct ppdetail<target_type> {                                      \
            static bool print_pretty(const ppindentinfo & ppii,             \
                                     const target_type & x) {               \
                return ppdetail_atomic<target_type>::print_pretty(ppii, x); \
            }                                                               \
        }

#define PPDETAIL_ATOMIC_BODY_CONST(target_type)                             \
        struct ppdetail<const target_type> {                                \
            static bool print_pretty(const ppindentinfo & ppii,             \
                                     const target_type & x) {               \
                return ppdetail_atomic<target_type>::print_pretty(ppii, x); \
            }                                                               \
        }

#define PPDETAIL_ATOMIC(target_type)                                        \
        template<>                                                          \
        PPDETAIL_ATOMIC_BODY(target_type)

#define PPDETAIL_ATOMIC_CONST(target_type)                                  \
        template<>                                                          \
        PPDETAIL_ATOMIC_BODY_CONST(target_type)

        PPDETAIL_ATOMIC(bool);
        PPDETAIL_ATOMIC(char);
#ifdef __APPLE__
        // unsigned long != std::uint64_t.
        PPDETAIL_ATOMIC(unsigned long);
#endif
        PPDETAIL_ATOMIC(std::int64_t);
        PPDETAIL_ATOMIC(std::uint64_t);
        PPDETAIL_ATOMIC(std::int32_t);
        PPDETAIL_ATOMIC(std::uint32_t);
        PPDETAIL_ATOMIC(float);
        PPDETAIL_ATOMIC(double);
        PPDETAIL_ATOMIC(std::string);
        PPDETAIL_ATOMIC(std::string_view);

        using voidptr = void*;
        PPDETAIL_ATOMIC(voidptr);
#endif // #ifndef ppdetail_atomic

    } /*namespace print*/
} /*namespace xo*/
