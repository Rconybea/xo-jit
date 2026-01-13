/** @file TypeRef.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/reflect/TypeDescr.hpp>
#include <xo/flatstring/flatstring.hpp>

namespace xo {
    namespace scm {
        /** @class TypeRef
         *  @brief name and (when established) resolution for type associate with an expression
         *
         *  Type inference / unification operates on
         *  @ref xo::scm::TypeBlueprint instances.  See also!
         **/
        class TypeRef {
        public:
            using TypeDescr = xo::reflect::TypeDescr;
            using type_var = flatstring<20>;
            using prefix_type = flatstring<8>;

        public:
            TypeRef() = default;
            TypeRef(const type_var & id, TypeDescr td);

            /** trivial typeref, where already resolved.
             *  Require: @p td non-null
             **/
            static TypeRef resolved(TypeDescr td);

            /** if @p td is non-null
             *   -> type is already resolved
             *
             *  if type is not determined (i.e. @p td is nullptr):
             *   -> generate and store type variable name.
             **/
            static TypeRef dwim(prefix_type prefix, TypeDescr td);

            /** generate a unique type-variable name,
             *  that begins with @p prefix
             **/
            static type_var generate_unique(prefix_type prefix);

            const type_var & id() const noexcept { return id_; }
            TypeDescr td() const noexcept { return td_; }

            /** true iff type at this location has been resolved **/
            bool is_resolved() const noexcept;

            /** resolve TypeRef by supplying final type-description **/
            void resolve(TypeDescr td) noexcept { td_ = td; }

        private:
            /** unique (probably generated) name for type at this location **/
            type_var id_;
            /** Description for concrete type, once resolved.
             *  May be null when this TypeRef created,
             *  but expected to be immutable once established.
             **/
            TypeDescr td_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end TypeRef.hpp */
