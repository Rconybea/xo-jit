/** @file reflect_struct_info.hpp
 *
 *  Author: Roland Conybeare
 **/

#pragma once

#include <cstdint>
#include <cstddef>

namespace xo {
    namespace reflect {
        /** Instructions for reflecting a struct:
         *
         *    #include "xo/reflectutil/reflect_struct_info.hpp" // this file
         *
         *    namespace xo {
         *        namespace reflect {
         *            // replacing X with the number of member variables in foo:
         *            REFLECT_BASE_STRUCT_INFO(some::ns::foo, X);
         *
         *            // for each member {member_0, member_1, ...} of foo,
         *            // in the same order as they appear in class/struct decl:
         *            REFLECT_STRUCT_MEMBER_INFO(some::ns::foo, 0, member(0));
         *            REFLECT_STRUCT_MEMBER_INFO(some::ns::foo, 1, member(1);
         *            ..
         *            // (last member will have index number X-1)
         *        }
         *    }
         *
         *  These decls can appear anywhere after decl for some::ns::foo;
         *  they must be made in the global namespace
         *  (so that specializations appear in the xo::reflect namespace)
         *
         *  Finally to permanently capture struct ingredients,
         *  this statement must run at least once:
         *
         *    xo::reflect::reflect_struct<some::ns::foo>();
         *
         **/

#define REFLECT_BASE_STRUCT_INFO_TBODY(qualified_struct_name, member_count)       \
                struct reflect_struct_traits<qualified_struct_name> {             \
                    static constexpr std::size_t n_members = member_count;        \
                    static constexpr std::size_t n_parents = 0;                   \
                }

#define REFLECT_BASE_STRUCT_INFO(qualified_struct_name, member_count)               \
                template<>                                                          \
                REFLECT_BASE_STRUCT_INFO_TBODY(qualified_struct_name, member_count)

#define REFLECT_STRUCT_MEMBER_INFO_TBODY(qualified_struct_name, member_ix, member_name)           \
                struct reflect_struct_member<qualified_struct_name, member_ix> {                  \
                    constexpr auto get() const {                                                  \
                        return reflect_struct_member_aux(xo::flatstring(#member_name),            \
                                                         &qualified_struct_name::member_name##_); \
                    }                                                                             \
                };

#define REFLECT_STRUCT_MEMBER_INFO(qualified_struct_name, member_ix, member_name) \
                template <>                                                       \
                REFLECT_STRUCT_MEMBER_INFO_TBODY(qualified_struct_name, member_ix, member_name)

        /** @class reflect_struct_traits
         *
         *  @brief header-only ingredients for reflecting a struct
         *
         *  A class/struct T to be reflected should provide specializations
         *  of
         *  - xo::reflect::reflect_struct_traits<>
         *  - xo::reflect::reflect_struct_member<>
         *  - xo::reflect::reflect_struct_parent<>  [aspirational, not yet]
         *
         *  For example:
         *
         *  @code
         *
         *    namespace foo {
         *        struct mycomplex { double real_, double imag_; };
         *    }
         *
         *    namespace xo {
         *        namespace reflect {
         *            template <>
         *            struct reflect_struct_traits<foo::mycomplex> {
         *
         *                // number of member variables
         *                static constexpr std::size_t n_members = 2;
         *
         *                // number of parent structs
         *                // (i.e. directly inherited structs)
         *                static constexpr std::size_t n_parents = 0;
         *            };
         *        }
         *    }
         *
         *    // or use convenience macro (must be in global namespace)
         *    REFLECT_BASE_STRUCT_INFO(foo::mycomplex, 2);
         *
         *  @endcode
         **/
        template <typename T>
        struct reflect_struct_traits;

        /** @class reflect_struct_member
         *  @brief reflection ingredients for a particular struct member
         *
         *  A class/struct T to be reflected should provide specializations
         *  of
         *  - xo::reflect::reflect_struct_traits<>
         *  - xo::reflect::reflect_struct_member<>
         *  - xo::reflect::reflect_struct_parent<>  [aspirational, not yet]
         *
         *  For example:
         *
         *  @code
         *
         *    namespace foo {
         *        struct mycomplex { double real_, double imag_; };
         *    }
         *
         *    namespace xo {
         *        namespace reflect {
         *            // reflect foo::mycomplex::real_
         *            template <>
         *            struct reflect_struct_member<foo::mycomplex, 0> {
         *                 constexpr auto operator()() const { return reflect_struct_member_aux(xo::flatstring("real"), &foo::mycomplex::real_); }
         *            };
         *
         *            // reflect foo::mycomplex::imag_
         *            template <>
         *            struct reflect_struct_member<foo::mycomplex, 1> {
         *                 constexpr auto operator()() const { return reflect_struct_member_aux(xo::flatstring("imag"), &foo::mycomplex::imag_); }
         *            };
         *        }
         *    }
         *
         *    // or use convenience macro (must be invoked from global namespace)
         *    REFLECT_STRUCT_MEMBER_INFO(foo::mycomplex, 0, real);
         *    REFLECT_STRUCT_MEMBER_INFO(foo::mycomplex, 1, imag);
         *
         *  @endcode
         **/
        template <typename T, std::size_t N>
        struct reflect_struct_member;

        // ----------------------------------------------------------------

        template <typename OwnerT, typename MemberT, typename MemberNameT>
        struct reflect_struct_member_info {
            //using struct_type = StructT;
            using owner_type = OwnerT;
            using member_type = MemberT;
            using member_name_type = MemberNameT;

            constexpr reflect_struct_member_info(const MemberNameT & member_name,
                                                 MemberT OwnerT::* member_addr)
                : member_name_{member_name}, member_addr_{member_addr} {}

            MemberNameT member_name_;
            MemberT OwnerT::* member_addr_;
        };

        // ----------------------------------------------------------------

        template <typename OwnerT, typename MemberT, typename MemberNameT>
        constexpr auto reflect_struct_member_aux(const MemberNameT & member_name,
                                                 MemberT OwnerT:: * member_addr) {
            return reflect_struct_member_info<OwnerT, MemberT, MemberNameT>
                (member_name, member_addr);
        };

    } /*namespace reflect*/
} /*namespace xo*/

/** end reflect_struct_info.hpp **/
