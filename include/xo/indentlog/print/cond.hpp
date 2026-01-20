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
        cond_impl(bool condition, T const & then_value, U const & else_value)
            : condition_{condition},
              then_value_{then_value},
              else_value_{else_value} {}

        bool condition() const { return condition_; }
        T const & then_value() const { return then_value_; }
        U const & else_value() const { return else_value_; }

    private:
        bool condition_;
        T then_value_;
        U else_value_;
    };

    /** Create conditional stream inserter.
     *
     *  @param condition   when true, print @p then_value; otherwise print @p else_value
     *  @param then_value  value to print when condition is true
     *  @param else_value  value to print when condition is false
     *
     *  Example:
     *  @code
     *    std::cout << cond(ptr != nullptr, xtag("ptr", *ptr), "nullptr");
     *  @endcode
     **/
    template <typename T, typename U>
    auto
    cond(bool condition, T && then_value, U && else_value)
    {
        return cond_impl<std::decay_t<T>, std::decay_t<U>>
            (condition,
             std::forward<T>(then_value),
             std::forward<U>(else_value));
    }

    /** Stream insertion operator for cond_impl **/
    template <typename T, typename U>
    inline std::ostream &
    operator<<(std::ostream & os, cond_impl<T, U> const & c)
    {
        if (c.condition())
            os << c.then_value();
        else
            os << c.else_value();
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
                    return ppdetail<T>::print_pretty(ppii, c.then_value());
                else
                    return ppdetail<U>::print_pretty(ppii, c.else_value());
            }
        };
    }
#endif

}

/* end cond.hpp */
