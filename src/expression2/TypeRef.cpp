/** @file TypeRef.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "TypeRef.hpp"

namespace xo {
    namespace scm {
        TypeRef::TypeRef(const type_var & id, TypeDescr td)
                : id_{id}, td_{td}
        {}

        TypeRef
        TypeRef::dwim(prefix_type prefix, TypeDescr td)
        {
            if (td) {
                /* type already resolved
                 *  -> we don't need a type variable name
                 */
                type_var null;
                return TypeRef(null, td);
            } else {
                /* type is not resolved yet.
                 *  -> give it a unique name,
                 *     to seed unification
                 */
                return TypeRef(generate_unique(prefix), td);
            }
        }

        auto
        TypeRef::generate_unique(prefix_type prefix) -> type_var
        {
            static uint32_t s_counter = 0;

            s_counter = (1 + s_counter) % 1000000000;

            char buf[type_var::fixed_capacity];
            int n = snprintf(buf, sizeof(buf), "%s:%u", prefix.c_str(), s_counter);
            (void)n;;

            assert(n < static_cast<int>(type_var::fixed_capacity));

            // not necessary, but to remove all doubt.
            buf [sizeof(buf) - 1] = '\0';

            return type_var(buf);
        }

        bool
        TypeRef::is_concrete() const noexcept
        {
            return (td_ != nullptr);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end TypeRef.cpp */
