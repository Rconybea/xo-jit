/** @file DSequenceExpr.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DSequenceExpr.hpp"

namespace xo {
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

        size_type
        DSequenceExpr::size() const noexcept
        {
            return expr_v_->size();
        }

        obj<AExpression>
        DSequenceExpr::operator[](std::size_t i) const
        {
            return (*expr_v_)[i];
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

            this->expr_v_->push_back(expr);
        }

        void
        DSequenceExpr::assign_valuetype(TypeDescr td) noexcept
        {
            typeref_.resolve(td);
        }

        void
        DSequenceExpr::pretty(const ppindentinfo & ppii) const
        {
            using xo::print::ppstate;

            ppstate * pps = ppii.pps();

                        xxx;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DSequenceExpr.cpp */
