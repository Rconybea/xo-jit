/* file tostr.hpp
 *
 * author: Roland Conybeare, Sep 2022
 */

#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace xo {
    /*
     * write x to stream s
     * note: here x is a universal reference, since
     *   (a) it's a template type
     *   (b) requires deduction to establish x's type
     * this means:
     *   x will be an r-value reference or an l-value reference
     * depending on calling context
     *
     * see:
     *   https://eli.thegreenplace.net/2014/variadic-templates-in-c/
     *   http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html
     *   https://en.cppreference.com/w/cpp/language/value_category
     *
     *   has identity == has address
     *
     *     /- has identity -----------------\
     *     |                                |
     *     |  lvalue                        |
     *     |  glvalue         /------------------------------\
     *     |                  |             |                |
     *     |                  |  xvalue     |                |
     *     |                  |  rvalue     |                |
     *     |                  | glvalue     |                |
     *     |                  |             |                |
     *     \--------------------------------/                |
     *                        |                    rvalue    |
     *                        |                    prvalue   |
     *                        |                              |
     *                        \- can be moved ---------------/
     *
     * 1. has identity,  but cannot be moved -> it's an lvalue; otherwise it's an rvalue
     *    e.g: local variable name
     *
     * 2. can be moved,  but no identity -> it's a prvalue (pure right-value);
     *                                      otherwise it's a glvalue (generalized left-value)
     *    e.g: non-reference function return value,  or literal constant
     *
     * 3. has identity and can be moved -> it's an xvalue (strange value)
     *    e.g: std::move(a)
     *
     * reminder:
     * - std::move() does not move:  it converts lvalue to rvalue,  so compiler can select
     *   desired overload
     * - std::forward() does not forward: it recovers original value category
     *   (when starting with a universal reference),  so compiler can select
     *   desired ctor
     */

    /* no-op terminal case */
    template <class Stream>
    Stream & tos(Stream & s) {
        return s;
    }

    // Use:
    //   tos(s,a,b,c)
    // is the same as
    //   s << a << b << c;
    //
    template <class Stream, typename T>
    Stream & tos(Stream & s, T && x) {
        s << x;
        return s;
    } /*tos*/

    template <class Stream, typename T, typename... Tn>
    Stream &tos(Stream & s, T && x, Tn && ...rest) {
        s << x;
        return tos(s, rest...);
    } /*tos*/

    // like tos(..),  but append newline
    //
    template <class Stream, typename... Tn>
    Stream &tosn(Stream &s, Tn &&...args) {
        tos(s, args...);
        s << std::endl;
        return s;
    } /*tosn*/

    // tostr(args..) writes arguments to temporary stingstream,
    // returns its contents
    //
    template <typename... Tn> std::string tostr(Tn &&...args) {
        std::stringstream ss;
        tos(ss, args...);
        //ss << std::ends;
        return ss.str();
    } /*tostr*/
} /*namespace xo*/

/* end tostr.hpp */
