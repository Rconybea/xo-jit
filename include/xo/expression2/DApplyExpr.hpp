/** @file DApplyExpr.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Expression.hpp"
#include "TypeRef.hpp"
#include "exprtype.hpp"
#include <xo/object2/DArray.hpp>
#include <xo/reflect/TypeDescr.hpp>
#include <xo/indentlog/print/pretty.hpp>

namespace xo {
    namespace scm {

        /** @class DApplyExpr
         *  @brief syntax for a procedure/function call
         **/
        class DApplyExpr {
        public:
            using ACollector = xo::mm::ACollector;
            using AAllocator = xo::mm::AAllocator;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::uint32_t;

        public:
            /** @defgroup scm-applyexpr-constructors **/
            ///@{

            /** construct empty instance, but with argument expressions empty **/
            DApplyExpr(TypeRef typeref,
                       obj<AExpression> fn_expr,
                       size_type n_args);

            /** create apply for function with 2 arguments **/
            static obj<AExpression,DApplyExpr> make2(obj<AAllocator> mm,
                                                     TypeRef typeref,
                                                     obj<AExpression> fn_expr,
                                                     obj<AExpression> arg1,
                                                     obj<AExpression> arg2);

            /** create apply for function with 2 arguments **/
            static DApplyExpr * _make2(obj<AAllocator> mm,
                                       TypeRef typeref,
                                       obj<AExpression> fn_expr,
                                       obj<AExpression> arg1,
                                       obj<AExpression> arg2);

            /** scaffold incomplete instance.
             *  apply-expr using memory from @p mm.
             *  will construct instance with space for @p n_args arguments
             *  but expressions left empty.
             *  use @ref assign_arg for all arguments to complete.
             **/
            static DApplyExpr * scaffold(obj<AAllocator> mm,
                                         TypeRef typeref,
                                         obj<AExpression> fn_expr,
                                         size_type n_args);
            void assign_arg(size_type i_arg, obj<AExpression> expr);

            ///@}
            /** @defgroup scm-applyexpr-access-methods **/
            ///@{

            obj<AExpression>  fn() const noexcept { return fn_; }
            size_type n_args() const noexcept { return n_args_; }
            obj<AExpression> arg(size_type i) const;

            ///@}
            /** @defgroup scm-applyexpr-expression-facet **/
            ///@{

            exprtype extype() const noexcept { return exprtype::apply; }
            TypeRef typeref() const noexcept { return typeref_; }
            TypeDescr valuetype() const noexcept { return typeref_.td(); }
            void assign_valuetype(TypeDescr td) noexcept;

            ///@}
            /** @defgroup scm-applyexpr-gcobject-facet **/
            ///@{

            std::size_t shallow_size() const noexcept;
            DApplyExpr * shallow_copy(obj<AAllocator> mm) const noexcept;
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            ///@}
            /** @defgroup scm-applyexpr-printable-facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** expression value always has type consistent
             *  with this description
             **/
            TypeRef typeref_;
            /** expression for function/procedure to invoke **/
            obj<AExpression> fn_;
            /** number of arguments (not counting @ref fn_ **/
            size_type n_args_ = 0;
            /** args_[i] is expression for i'th argument to @ref fn_ **/
            obj<AExpression> args_[];
        };
    }
}

/* end DApplyExpr.hpp */
