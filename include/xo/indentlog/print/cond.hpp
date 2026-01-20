/** @file cond.hpp
 *
 *  author: Roland Conybeare
 **/

#pragma once

#include "ppdetail_atomic.hpp"
#include <iostream>
#include <type_traits>

namespace xo {

    /** @class cond_impl
     *  @brief conditional stream inserter implementation
     *
     *  @tparam T  type printed when condition is true
     *  @tparam U  type printed when condition is false
     **/
    template <typename T, typename U>
    struct cond_impl {
    public:
        cond_impl(bool condition, const T & if_true, const U & if_false)
            : condition_{condition},
              if_true_{if_true},
              if_false_{if_false} {}

        bool condition() const { return condition_; }
        const T & if_true() const { return if_true_; }
        const U & if_false() const { return if_false_; }

    private:
        bool condition_;
        T if_true_;
        U if_false_;
    };

    /** Create conditional stream inserter.
     *
     *  @param condition   when true, print @p if_true; otherwise print @p if_false
     *  @param if_true  value to print when condition is true
     *  @param if_false  value to print when condition is false
     *
     *  Example:
     *  @code
     *    std::cout << cond(ptr, xtag("ptr", *ptr), "nullptr");
     *  @endcode
     **/
    template <typename T, typename U>
    auto
    cond(bool condition, T && if_true, U && if_false)
    {
        return cond_impl<std::decay_t<T>, std::decay_t<U>>
            (condition,
             std::forward<T>(if_true),
             std::forward<U>(if_false));
    }

    /** Stream insertion operator for cond_impl **/
    template <typename T, typename U>
    inline std::ostream &
    operator<<(std::ostream & os, const cond_impl<T, U> & c)
    {
        if (c.condition())
            os << c.if_true();
        else
            os << c.if_false();
        return os;
    }

#ifndef ppdetail_atomic
    namespace print {
        /** Pretty-printing support for cond_impl **/
        template <typename T, typename U>
        struct ppdetail<cond_impl<T, U>> {
            using target_type = cond_impl<T, U>;

            static bool print_pretty(const ppindentinfo & ppii,
                                     const target_type & c)
            {
                if (c.condition())
                    return ppdetail<T>::print_pretty(ppii, c.if_true());
                else
                    return ppdetail<U>::print_pretty(ppii, c.if_false());
            }
        };
    }
#endif

}

/* end cond.hpp */
