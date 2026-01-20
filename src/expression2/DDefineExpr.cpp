/** @file DDefineExpr.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DDefineExpr.hpp"
#include "detail/IPrintable_DVariable.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
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

            TypeRef rhs_tref;
            if (rhs_expr)
                rhs_tref = rhs_expr.typeref();

            auto lhs_var = DVariable::make(mm,
                                           lhs_name,
                                           rhs_tref);

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
        DDefineExpr::assign_valuetype(TypeDescr td) noexcept
        {
            lhs_var_->assign_valuetype(td);
        }

        bool
        DDefineExpr::pretty(const ppindentinfo & ppii) const
        {
            auto lhs = obj<APrintable,DVariable>(lhs_var_);
            auto rhs = FacetRegistry::instance().try_variant<APrintable,
                                                             AExpression>(rhs_);

            if (rhs_) {
                assert(rhs);

                return ppii.pps()->pretty_struct
                           (ppii,
                            "DDefineExpr",
                            refrtag("lhs", lhs),
                            refrtag("rhs", rhs));
            } else {
                return ppii.pps()->pretty_struct
                           (ppii,
                            "DDefineExpr",
                            refrtag("lhs", lhs),
                            refrtag("rhs", "nullptr"));
            }
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DDefineExpr.cpp */
