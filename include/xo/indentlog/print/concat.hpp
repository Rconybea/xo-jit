/* @file concat.hpp */

#pragma once

#include "ppdetail_atomic.hpp"
#include <ostream>
#include <utility>  // for std::move()

namespace xo {
    template <typename T1, typename T2>
    struct concat_impl {
    public:
        concat_impl(T1 && x1, T2 && x2)
            : x1_{std::forward<T1>(x1)}, x2_{std::forward<T2>(x2)} {}

        T1 const & x1() const { return x1_; }
        T2 const & x2() const { return x2_; }

    private:
        T1 x1_;
        T2 x2_;
    }; /*concat_impl*/

    template <typename T1>
    T1 concat(T1 && x1) {
        return x1;
    } /*concat*/

    template <typename T1, typename T2>
    concat_impl<T1,T2> concat(T1 && x1, T2 && x2) {
        return concat_impl<T1,T2>(std::move(x1), std::move(x2));
    } /*concat*/

    template <typename T1, typename T2>
    inline std::ostream &
    operator<<(std::ostream & os, concat_impl<T1, T2> const & x) {
        os << x.x1() << x.x2();
        return os;
    } /*operator<<*/

#ifndef ppdetail_atomic
    namespace print {
        /* concat expected to be used on short string-like things.
         * i.e. don't want structure visible to pretty-printer.
         * could be using it like concat("boeing", 777)
         */
        template <typename T1, typename T2>
        struct ppdetail<concat_impl<T1,T2>> {
            using target_type = concat_impl<T1,T2>;

            static bool print_pretty(const ppindentinfo & ppii, const target_type & x) {
                return ppdetail_atomic<target_type>::print_pretty(ppii, x);
            }
        };
    }
#endif
} /*namespace xo*/

/* end concat.hpp */
