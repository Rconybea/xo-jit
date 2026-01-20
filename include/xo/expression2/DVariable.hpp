/** @file DVariable.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "DUniqueString.hpp"
#include "Binding.hpp"
#include "TypeRef.hpp"
#include "exprtype.hpp"
#include <xo/reflect/TypeDescr.hpp>

namespace xo {
    namespace scm {

        /** @class DVariable*
         *  @brief syntax for a variable reference
         **/
        class DVariable {
        public:
            using AAllocator = xo::mm::AAllocator;
            using TypeDescr = xo::reflect::TypeDescr;

        public:
            /** create instance
             *  @p mm      memory allocator
             *  @p name    variable name
             *  @p typeref type information for legal values
             *             (possibly just placeholder when relying on inference)
             *  @p path    binding path to runtime value.
             *             This may be computed after parsing;
             *             mnust be resolved before execution.
             **/
            static DVariable * make(obj<AAllocator> mm,
                                    const DUniqueString * name,
                                    const TypeRef & typeref,
                                    Binding path = Binding());

            DVariable(const DUniqueString * name,
                      const TypeRef & typeref,
                      Binding path);

            const DUniqueString * name() const { return name_; }
            Binding path() const { return path_; }

            void assign_name(const DUniqueString * name) { this->name_ = name; }

            /** @defgroup scm-variable-expression-facet**/
            ///@{

            exprtype extype() const noexcept { return exprtype::variable; }
            TypeRef typeref() const noexcept { return typeref_; }
            TypeDescr valuetype() const noexcept { return typeref_.td(); };
            void assign_valuetype(TypeDescr td) noexcept;

            ///@}

        private:
            /** symbol name **/
            const DUniqueString * name_;
            /** variable value always has type consistent
             *  with this description
             **/
            TypeRef typeref_;
            /** at runtime: navigate environemnt via this path to get
             *  runtime memory location for this variable
             **/
            Binding path_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DVariable.hpp */
