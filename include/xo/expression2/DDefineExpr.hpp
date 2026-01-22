/** @file DDefineExpr.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Expression.hpp"
#include "DVariable.hpp"
#include <xo/alloc2/Allocator.hpp>
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    namespace scm {
        class DUniqueString; // see DUniqueString.hpp

        /** @class DDefineExpr
         *  @brief an expression that introduces a variable.
         *
         *  Variable may optionally be declared with a type,
         *  and may come with an expression specifying an initial value
         **/
        class DDefineExpr {
        public:
            using ppindentinfo = xo::print::ppindentinfo;
            using AAllocator = xo::mm::AAllocator;
            using TypeDescr = xo::reflect::TypeDescr;

        public:
            /** @defgroup scm-defineexpr-constructors **/
            ///@{

            /** create instance: define-expr using memory from @p mm
             *  with lhs name @p lhs_name and rhs expression @p rhs_expr
             **/
            static DDefineExpr * make(obj<AAllocator> mm,
                                      const DUniqueString * lhs_name,
                                      obj<AExpression> rhs_expr);
            /** create empty skeleton. Rely on this for parsing
             **/
            static DDefineExpr * make_empty(obj<AAllocator> mm);

            DDefineExpr(DVariable * lhs_var,
                        obj<AExpression> rhs);

            ///@}
            /** @defgroup scm-definexpr-access-methods **/
            ///@{

            DVariable * lhs() const { return lhs_var_; }
            obj<AExpression> rhs() const noexcept { return rhs_; }
            const DUniqueString * name() const noexcept;

            ///@}
            /** @defgroup scm-definexpr-bookkeeping-methods **/
            ///@{

            void assign_lhs_name(const DUniqueString * name);
            void assign_rhs(obj<AExpression> rhs);

            ///@}
            /** @defgroup scm-defineexpr-expression-facet **/
            ///@{

            exprtype extype() const noexcept { return exprtype::define; }
            TypeRef typeref() const noexcept { return lhs_var_->typeref(); }
            TypeDescr valuetype() const noexcept { return lhs_var_->typeref().td(); }
            void assign_valuetype(TypeDescr td) noexcept;

            ///@}
            /** @defgroup scm-defineexpr-printable-facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** @defgrouop scm-defineexpr-instance-vars **/
            ///@{

            /** variable being defined by this expression.
             **/
            DVariable * lhs_var_ = nullptr;

            /** expression for initial value of this expression
             **/
            obj<AExpression> rhs_;

            // std::set<std::string> free_var_set_;

            ///@}
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DDefineExpr.hpp */
