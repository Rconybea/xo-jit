/** @file DApplyExpr.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "Expression.hpp"
#include "DApplyExpr.hpp"
#include "detail/IExpression_DApplyExpr.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::reflect::typeseq;
    using xo::mm::AGCObject;

    namespace scm {
        obj<AExpression,DApplyExpr>
        DApplyExpr::make2(obj<AAllocator> mm,
                          TypeRef typeref,
                          obj<AExpression> fn_expr,
                          obj<AExpression> arg1,
                          obj<AExpression> arg2)
        {
            return obj<AExpression,DApplyExpr>
                       (DApplyExpr::_make2(mm, typeref, fn_expr, arg1, arg2));
        }

        DApplyExpr *
        DApplyExpr::_make2(obj<AAllocator> mm,
                           TypeRef typeref,
                           obj<AExpression> fn_expr,
                           obj<AExpression> arg1,
                           obj<AExpression> arg2)
        {
            DApplyExpr * result
                = DApplyExpr::scaffold(mm, typeref, fn_expr, 2 /*n_args*/);

            result->assign_arg(0, arg1);
            result->assign_arg(1, arg2);

            return result;
        }

        /* incomplete, in the sense that does not populate args_[] */
        DApplyExpr::DApplyExpr(TypeRef typeref,
                               obj<AExpression> fn_expr,
                               size_type n_args) : typeref_{typeref},
                                                   fn_{fn_expr},
                                                   n_args_{n_args}
        {}

        DApplyExpr *
        DApplyExpr::scaffold(obj<AAllocator> mm,
                             TypeRef typeref,
                             obj<AExpression> fn_expr,
                             size_type n_args)
        {
            void * mem = mm.alloc(typeseq::id<DApplyExpr>(),
                                  sizeof(DApplyExpr) + (n_args * sizeof(obj<AExpression>)));

            DApplyExpr * result = new (mem) DApplyExpr(typeref,
                                                       fn_expr,
                                                       n_args);

            return result;
        }

        void
        DApplyExpr::assign_arg(size_type i_arg,
                               obj<AExpression> expr)
        {
            if (i_arg < n_args_) {
                this->args_[i_arg] = expr;
            } else {
                assert(false);

                throw std::runtime_error(tostr("assign out-of-range argument i_arg where [0..n_args) expected",
                                               xtag("i_arg", i_arg),
                                               xtag("expr", expr),
                                               xtag("n_args", n_args_)));

            }
        }

        obj<AExpression>
        DApplyExpr::arg(size_type i) const
        {
            if (i >= n_args_) [[unlikely]] {
                throw std::runtime_error(tostr("attempt to fetch argument i where [0..n) expected",
                                               xtag("i", i),
                                               xtag("n", n_args_),
                                               xtag("src", "DApplyExpr::arg")));
            }

            return args_[i];
        }

        void
        DApplyExpr::assign_valuetype(TypeDescr td) noexcept {
            typeref_.resolve(td);
        }

        // ----- gcobject facet -----

        std::size_t
        DApplyExpr::shallow_size() const noexcept {
            return sizeof(DApplyExpr) + (n_args_ * sizeof(obj<AExpression>));
        }

        DApplyExpr *
        DApplyExpr::shallow_copy(obj<AAllocator> mm) const noexcept {
            DApplyExpr * copy = (DApplyExpr *)mm.alloc_copy((std::byte *)this);

            if (copy) {
                copy->typeref_ = typeref_;
                copy->fn_ = fn_;
                copy->n_args_ = n_args_;

                constexpr auto c_obj_z = sizeof(obj<AExpression>);

                ::memcpy((void*)&(copy->args_[0]), (void*)&(args_[0]), n_args_ * c_obj_z);
            }

            return copy;
        }

        std::size_t
        DApplyExpr::forward_children(obj<ACollector> gc) noexcept
        {
            for (size_type i = 0; i < n_args_; ++i) {
                obj<AExpression> & arg = args_[i];

                // runtime poly here
                obj<AGCObject> arg_gco = arg.to_facet<AGCObject>();

                // need the data address within *this
                gc.forward_inplace(arg_gco.iface(), (void **)(&arg.data_));
            }

            return shallow_size();
        }

        // ----- printable facet -----

        bool
        DApplyExpr::pretty(const ppindentinfo & ppii) const {
            using xo::print::ppstate;

            ppstate * pps = ppii.pps();

            if (ppii.upto()) {
                /* perhaps print on one line */
                if (!pps->print_upto("<ApplyExpr"))
                    return false;

                {
                    obj<APrintable> fn
                        = FacetRegistry::instance().variant<APrintable>(fn_);
                    if (!pps->print_upto(refrtag("fn", fn)))
                        return false;
                }

                for (size_t i_arg = 0; i_arg < n_args_; ++i_arg) {
                    obj<APrintable> arg_i
                        = FacetRegistry::instance().variant<APrintable>(args_[i_arg]);

                    if (!pps->print_upto(refrtag(concat("arg", 1+i_arg), arg_i)))
                        return false;
                }

                pps->write(">");

                return true;
            } else {
                pps->write("<ApplyExpr");

                obj<APrintable> fn
                    = FacetRegistry::instance().variant<APrintable>(fn_);

                pps->newline_indent(ppii.ci1());
                pps->pretty(refrtag("fn", fn));

                for (size_t i_arg = 0; i_arg < n_args_; ++i_arg) {
                    obj<APrintable> arg_i
                        = FacetRegistry::instance().variant<APrintable>(args_[i_arg]);

                    pps->newline_indent(ppii.ci1());
                    pps->pretty(refrtag(concat("arg", 1+i_arg), arg_i));
                }

                pps->write(">");

                return false;
            }
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DApplyExpr.cpp */
