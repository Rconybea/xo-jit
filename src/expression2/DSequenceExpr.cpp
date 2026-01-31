/** @file DSequenceExpr.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DSequenceExpr.hpp"
#include "detail/IExpression_DSequenceExpr.hpp"
#include <xo/object2/array/IGCObject_DArray.hpp>
#include <xo/gc/GCObject.hpp>
#include <xo/alloc2/Allocator.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/facet/obj.hpp>
#include <xo/reflectutil/typeseq.hpp>

namespace xo {
    using xo::mm::AGCObject;
    using xo::facet::FacetRegistry;
    using xo::reflect::typeseq;

    namespace scm {

        obj<AExpression,DSequenceExpr>
        DSequenceExpr::make_empty(obj<AAllocator> mm)
        {
            return obj<AExpression,DSequenceExpr>(_make_empty(mm));
        }

        DSequenceExpr *
        DSequenceExpr::_make_empty(obj<AAllocator> mm)
        {
            void * mem = mm.alloc(typeseq::id<DSequenceExpr>(),
                                  sizeof(DSequenceExpr));

            DSequenceExpr * expr = new (mem) DSequenceExpr();

            constexpr size_type c_hint_capacity = 8;

            /** allocate 2nd, so it comes after DSequenceExpr in
             *  memory.  This may later allow realloc
             **/
            DArray * expr_v = DArray::empty(mm,
                                            c_hint_capacity);

            expr->expr_v_ = expr_v;

            return expr;
        }

        auto
        DSequenceExpr::size() const noexcept -> size_type
        {
            return expr_v_->size();
        }

        obj<AExpression>
        DSequenceExpr::operator[](std::size_t i) const
        {
            obj<AGCObject> gco = (*expr_v_)[i];

            return FacetRegistry::instance().variant<AExpression,AGCObject>(gco);
        }

        void
        DSequenceExpr::push_back(obj<AAllocator> mm,
                                 obj<AExpression> expr)
        {
            if (expr_v_->size() == expr_v_->capacity()) {
                /* reallocate+expand */

                DArray * expr_2x_v
                    = DArray::empty(mm, 2 * expr_v_->capacity());

                for (size_type i = 0, z = expr_v_->size(); i < z; ++i) {
                    expr_2x_v->push_back((*expr_2x_v)[i]);
                }

                this->expr_v_ = expr_2x_v;
            }

            obj<AGCObject> expr_gco
                = FacetRegistry::instance().variant<AGCObject,AExpression>(expr);

            this->expr_v_->push_back(expr_gco);
        }

        void
        DSequenceExpr::assign_valuetype(TypeDescr td) noexcept
        {
            typeref_.resolve(td);
        }

        bool
        DSequenceExpr::pretty(const ppindentinfo & ppii) const
        {
            using xo::print::ppstate;

            return ppii.pps()->pretty_struct
                (ppii,
                 "DSequenceExpr");
        }

        // gc hooks for IGCObject_DSequenceExpr

        std::size_t
        DSequenceExpr::shallow_size() const noexcept
        {
            return sizeof(DSequenceExpr);
        }

        DSequenceExpr *
        DSequenceExpr::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            DSequenceExpr * copy = (DSequenceExpr *)mm.alloc_copy((std::byte *)this);

            if (copy)
                *copy = *this;

            return copy;
        }

        std::size_t
        DSequenceExpr::forward_children(obj<ACollector> gc) noexcept
        {
            // WARNING.
            //   if this proves problematic,
            //   may resort to obj<AGCObject,DArray> for expr_v_ member

            auto iface = facet::impl_for<AGCObject,DArray>();

            gc.forward_inplace(&iface, (void**)&expr_v_);

            return shallow_size();
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DSequenceExpr.cpp */
