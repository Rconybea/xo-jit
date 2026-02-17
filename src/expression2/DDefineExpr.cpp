/** @file DDefineExpr.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DDefineExpr.hpp"
#include "Variable.hpp"
#include <xo/gc/GCObject.hpp>
#include <xo/gc/PolyForwarderUtil.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/indentlog/scope.hpp>
#include <xo/indentlog/print/cond.hpp>

namespace xo {
    using xo::mm::poly_forward_inplace;
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

        void
        DDefineExpr::assign_rhs(obj<AExpression> x)
        {
            this->rhs_ = x;
        }

        // ----- GCObject facet -----

        std::size_t
        DDefineExpr::shallow_size() const noexcept
        {
            return sizeof(*this);
        }

        DDefineExpr *
        DDefineExpr::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            return mm.std_copy_for(this);
        }

        std::size_t
        DDefineExpr::forward_children(obj<ACollector> gc) noexcept
        {
            gc.forward_inplace(&lhs_var_);
            //gc.forward_inplace(&rhs_);  // complicated - need to access via GCObject facet
            poly_forward_inplace(gc, &rhs_);

            return this->shallow_size();
        }

        bool
        DDefineExpr::pretty(const ppindentinfo & ppii) const
        {
            assert(lhs_var_);

            auto lhs = obj<APrintable,DVariable>(lhs_var_);
            auto rhs = FacetRegistry::instance().try_variant<APrintable,
                                                             AExpression>(rhs_);

            if (lhs_var_)
                assert(lhs.data());

            (void)lhs;
            (void)rhs;

            if (rhs_)
                assert(rhs.data());

            // note: xo::print::cond() doesn't resolve the way we want here

            if (rhs) {
                return ppii.pps()->pretty_struct
                           (ppii,
                            "DDefineExpr"
                            , refrtag("lhs", lhs)
                            , refrtag("rhs", rhs)
                               );
            } else {
                return ppii.pps()->pretty_struct
                           (ppii,
                            "DDefineExpr"
                            , refrtag("lhs", lhs)
                               );

            }
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DDefineExpr.cpp */
