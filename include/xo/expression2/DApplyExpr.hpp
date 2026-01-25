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
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::size_t;

        public:

            obj<AExpression>  fn() const noexcept { return fn_; }
            const DArray * args() const noexcept { return args_; }

            size_type n_arg() const noexcept { return args_->size(); }
            obj<AExpression> arg(size_type i) const;

            /** @defgroup scm-applyexpr-expression-facet **/
            ///@{

            exprtype extype() const noexcept { return exprtype::apply; }
            TypeRef typeref() const noexcept { return typeref_; }
            TypeDescr valuetype() const noexcept { return typeref_.td(); }
            void assign_valuetype(TypeDescr td) noexcept;

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
            /** expression for each argument vector **/
            const DArray * args_;
        };
    }
}

/* end DApplyExpr.hpp */
