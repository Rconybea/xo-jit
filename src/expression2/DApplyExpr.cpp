/** @file DApplyExpr.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DApplyExpr.hpp"
#include "Expression.hpp"
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::mm::AGCObject;

    namespace scm {
        obj<AExpression>
        DApplyExpr::arg(size_type i) const
        {
            if (i >= args_->size()) [[unlikely]] {
                throw std::runtime_error(tostr("attempt to fetch argument i where [0..n) expected",
                                               xtag("i", i),
                                               xtag("n", args_->size()),
                                               xtag("src", "DApplyExpr::arg")));
            }

            obj<AGCObject> arg_i = args_->at(i);

            auto expr_i = FacetRegistry::instance().variant<AExpression>(arg_i);

            if (!expr_i) [[unlikely]] {
                throw std::runtime_error(tostr("expected expression interface on argument i",
                                               xtag("i", i),
                                               xtag("arg[i]", arg_i)));
            }

            return expr_i;
        }

        void
        DApplyExpr::assign_valuetype(TypeDescr td) noexcept {
            typeref_.resolve(td);
        }

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

                for (size_t i_arg = 0, n_arg = this->n_arg(); i_arg < n_arg; ++i_arg) {
                    obj<APrintable> arg_i
                        = FacetRegistry::instance().variant<APrintable>(this->arg(i_arg));

                    if (!pps->print_upto(refrtag(concat("arg", 1+i_arg), arg_i)))
                        return false;
                }

                return true;
            } else {
                pps->write("<ApplyExpr");

                obj<APrintable> fn
                    = FacetRegistry::instance().variant<APrintable>(fn_);

                pps->newline_indent(ppii.ci1());
                pps->pretty(refrtag("fn", fn));

                for (size_t i_arg = 0, n_arg = this->n_arg(); i_arg < n_arg; ++i_arg) {
                    obj<APrintable> arg_i
                        = FacetRegistry::instance().variant<APrintable>(fn_);

                    pps->newline_indent(ppii.ci1());
                    pps->pretty(refrtag(concat("arg", 1+i_arg), arg_i));
                }
                return false;
            }
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DApplyExpr.cpp */
