/** @file TypeRef.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/type/Type.hpp>
#include <xo/reflect/TypeDescr.hpp>
#include <xo/alloc2/Collector.hpp>
#include <xo/alloc2/GCObjectVisitor.hpp>
#include <xo/flatstring/flatstring.hpp>
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    namespace scm {
        /** @class TypeRef
         *  @brief name and (when established) resolution for type
         *  associated with an expression
         *
         *  Type inference / unification operates on
         *  @ref xo::scm::TypeBlueprint instances.  See also!
         **/
        class TypeRef {
        public:
            using TypeDescr = xo::reflect::TypeDescr;
            using type_var = flatstring<20>;
            using prefix_type = flatstring<8>;
            using ACollector = xo::mm::ACollector;
            using AGCObjectVisitor = xo::mm::AGCObjectVisitor;
            using ppindentinfo = xo::print::ppindentinfo;

        public:
            TypeRef() = default;
            TypeRef(const type_var & id, obj<AType> type);

            /** trivial typeref, where already resolved.
             *  Require: @p td non-null
             **/
            static TypeRef resolved(TypeDescr td);

            /** trivial typeref, where already resolved **/
            static TypeRef resolved(obj<AType> type);

            /** if @p type is non-null
             *   -> type already resolved
             *  else
             *   -> generate unique typevar name, starting with @p prefix
             **/
            static TypeRef dwim(prefix_type prefix, obj<AType> type);

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

            /** pretty-printer support **/
            bool pretty(const ppindentinfo & ppii) const;

            /** gc support **/
            void visit_gco_children(obj<AGCObjectVisitor> gc) noexcept;

        private:
            TypeRef(const type_var & id, TypeDescr td);

        private:
            /** unique (probably generated) name for type at this location **/
            type_var id_;

            /** Type, when resolved **/
            obj<AType> type_;

            /** Description for concrete type, once resolved.
             *  May be null when this TypeRef created,
             *  but expected to be immutable once established.
             **/
            TypeDescr td_ = nullptr;
        };
    } /*namespace scm*/

    namespace print {
        /** pretty printer in <xo/indentlog/print/pretty.hpp> relies on this specialization
         *  to handle TypeRef instances
         **/
        template <>
        struct ppdetail<xo::scm::TypeRef> {
            static inline bool print_pretty(const ppindentinfo & ppii, const xo::scm::TypeRef x) {
                return x.pretty(ppii);
            }
        };
    }
} /*namespace xo*/

/* end TypeRef.hpp */
