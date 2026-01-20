/** @file DDefineExpr.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DDefineExpr.hpp"
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::facet::typeseq;

    namespace scm {

        DDefineExpr::DDefineExpr(DVariable * lhs_var,
                                 obj<AExpression> rhs)
          : lhs_var_{lhs_var}, rhs_{rhs}
        {}

        DDefineExpr *
        DDefineExpr::make(obj<AAllocator> mm,
                          const DUniqueString * lhs_name,
                          obj<AExpression> rhs_expr)
        {
            void * mem = mm.alloc(typeseq::id<DDefineExpr>(),
                                  sizeof(DDefineExpr));

            auto lhs_var = DVariable::make(mm,
                                           lhs_name,
                                           rhs_expr.typeref());

            return new (mem) DDefineExpr(lhs_var, rhs_expr);
        }

        DDefineExpr *
        DDefineExpr::make_empty(obj<AAllocator> mm)
        {
            return make(mm,
                        nullptr /*lhs_name*/,
                        obj<AExpression>() /*rhs_expr*/);
        }

        const DUniqueString *
        DDefineExpr::name() const noexcept
        {
            return lhs_var_->name();
        }

        void
        DDefineExpr::assign_lhs_name(const DUniqueString * name)
        {
            lhs_var_->assign_name(name);
        }

        void
        DDefineExpr::assign_lhs_name(std::string_view name)
        {
            scope log(XO_DEBUG(true), "bogus impl - will require unique string");
            log && log(xtag("name", name));

            //lhs_var_->assign_name(name);
        }

        void
        DDefineExpr::assign_valuetype(TypeDescr td) noexcept
        {
            lhs_var_->assign_valuetype(td);
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DDefineExpr.cpp */
