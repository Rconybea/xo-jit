/** @file DConstant.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/reflect/TypeDescr.hpp>

namespace xo {
    namespace scm {
        /** @class DConstant
         *  @brief Schematika expression respresenting a literal constant
         **/
        struct DConstant {
        public:
            using TypeDescr = xo::reflect::TypeDescr;

        public:
            DConstant(TypeDescr td, void * value);

            TypeDescr value_td() const { return value_td; }

            TypeRef typeref() const noexcept;
            TypeDescr valuetype() const noexcept;
            void assign_valuetype(TypeDescr td) noexcept;

        private:
            /** type for value of this expression **/
            TypeRef type_ref_;
            /** type description for destination *value_ **/
            TypeDescr valuetype_;
            /** literal value **/
            void * value_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DConstant.hpp */
