/* @file type_name.hpp */

#pragma once

//#include <string>
#include <string_view>
#include <array>   // std::array
#include <utility> // std::index_sequence

namespace xo {
    namespace reflect {

        template <std::size_t...Idxs>
        constexpr auto
        substring_as_array(std::string_view str,
                           std::index_sequence<Idxs...> indexes)
        {
            //return std::array<char, indexes.size()+1>{str[Idxs]..., '\n'};
            return std::array<char, indexes.size()>{str[Idxs]...};
        } /*substring_as_array*/

        template <typename T> constexpr auto type_name_array() {
#if defined(__clang__)
            constexpr auto prefix = std::string_view{"[T = "};
            constexpr auto suffix = std::string_view{"]"};
            constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(__GNUC__)
            constexpr auto prefix = std::string_view{"with T = "};
            constexpr auto suffix = std::string_view{"]"};
            constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(_MSC_VER)
            constexpr auto prefix = std::string_view{"type_name_array<"};
            constexpr auto suffix = std::string_view{">(void)"};
            constexpr auto function = std::string_view{__FUNCSIG__};
#else
# error type_name_array: Unsupported compiler
#endif

            constexpr auto start = function.find(prefix) + prefix.size();
            constexpr auto end = function.rfind(suffix);

            //static_assert(start < end);

            constexpr auto name = function.substr(start, (end - start));

            constexpr auto ixseq = std::make_index_sequence<name.size()>{};

            return substring_as_array(name, ixseq);
        } /*type_name_array*/

        template <typename T>
        struct type_name_holder {
            static inline constexpr auto value = type_name_array<T>();
        };

        /** report name of type T as a string_view;
         *  using constexpr deps so runs at          **/
        template <typename T>
        constexpr auto type_name() -> std::string_view
        {
            constexpr auto& value = type_name_holder<T>::value;
            return std::string_view{value.data(), value.size()};
        }

#ifdef NOT_IN_USE
        template <std::string_view const&... Strs>
        struct join
        {
            // Join all strings into a single std::array of chars
            static constexpr auto impl() noexcept
                {
                    constexpr std::size_t len = (Strs.size() + ... + 0);
                    std::array<char, len + 1> arr{};
                    auto append = [i = 0, &arr](auto const& s) mutable {
                        for (auto c : s) arr[i++] = c;
                    };
                    (append(Strs), ...);
                    arr[len] = 0;
                    return arr;
                }
            // Give the joined string static storage
            static constexpr auto arr = impl();
            // View as a std::string_view
            static constexpr std::string_view value {arr.data(), arr.size() - 1};
        };

        // Helper to get the value out
        template <std::string_view const&... Strs>
        static constexpr auto join_v = join<Strs...>::value;
#endif
    } /*namespace reflect*/
} /*namespace xo*/

/* end type_name.hpp */
