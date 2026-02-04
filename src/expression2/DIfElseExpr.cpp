/** @file DIfElseExpr.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DIfElseExpr.hpp"
#include "detail/IExpression_DIfElseExpr.hpp"
#include <xo/gc/GCObject.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/reflectutil/typeseq.hpp>

namespace xo {
    using xo::mm::AGCObject;
    using xo::print::APrintable;
    using xo::reflect::typeseq;
    using xo::facet::FacetRegistry;

    namespace scm {
        DIfElseExpr::DIfElseExpr(TypeRef ifexpr_tref,
                                 obj<AExpression> test_expr,
                                 obj<AExpression> when_true,
                                 obj<AExpression> when_false)
            : typeref_{ifexpr_tref},
              test_{test_expr},
              when_true_{when_true},
              when_false_{when_false}
        {}

        obj<AExpression,DIfElseExpr>
        DIfElseExpr::make(obj<AAllocator> mm,
                          obj<AExpression> test,
                          obj<AExpression> when_true,
                          obj<AExpression> when_false)
        {
            return obj<AExpression,DIfElseExpr>
                (_make(mm,
                       test, when_true, when_false));
        }

        DIfElseExpr *
        DIfElseExpr::_make(obj<AAllocator> mm,
                          obj<AExpression> test,
                          obj<AExpression> when_true,
                          obj<AExpression> when_false)
        {
            void * mem = mm.alloc(typeseq::id<DIfElseExpr>(),
                                  sizeof(DIfElseExpr));

            // just crete typevar here, then rely on type checking
            // later

            auto prefix = TypeRef::prefix_type::from_chars("if");
            TypeRef tref = TypeRef::dwim(prefix, nullptr);

            return new (mem) DIfElseExpr(tref,
                                         test,
                                         when_true,
                                         when_false);
        }

        obj<AExpression,DIfElseExpr>
        DIfElseExpr::make_empty(obj<AAllocator> mm)
        {
            return obj<AExpression,DIfElseExpr>(_make_empty(mm));
        }

        DIfElseExpr *
        DIfElseExpr::_make_empty(obj<AAllocator> mm)
        {
            return _make(mm,
                         obj<AExpression>() /*test*/,
                         obj<AExpression>() /*when_true*/,
                         obj<AExpression>() /*when_false*/);
        }

        void
        DIfElseExpr::assign_valuetype(TypeDescr td) noexcept
        {
            typeref_.resolve(td);
        }

        bool
        DIfElseExpr::pretty(const ppindentinfo & ppii) const
        {
            auto test
                = FacetRegistry::instance().try_variant<APrintable,
                                                        AExpression>(test_);
            auto when_true
                = FacetRegistry::instance().try_variant<APrintable,
                                                        AExpression>(when_true_);
            auto when_false
                = FacetRegistry::instance().try_variant<APrintable,
                                                        AExpression>(when_false_);

            bool test_present = test;
            bool when_true_present = when_true;
            bool when_false_present = when_false;

            return ppii.pps()->pretty_struct
                       (ppii,
                        "DIfElseExpr",
                        refrtag("typeref", typeref_),
                        refrtag("test", test, test_present),
                        refrtag("when_true", when_true, when_true_present),
                        refrtag("when_false", when_false, when_false_present));
        }

        // GCObject facet

        std::size_t
        DIfElseExpr::shallow_size() const noexcept
        {
            return sizeof(DIfElseExpr);
        }

        DIfElseExpr *
        DIfElseExpr::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            DIfElseExpr * copy = (DIfElseExpr *)mm.alloc_copy((std::byte *)this);

            if (copy)
                *copy = *this;

            return copy;
        }

        std::size_t
        DIfElseExpr::forward_children(obj<ACollector> gc) noexcept
        {
            // GC needs to locate AGCObject iface for each member.
            {
                auto gco = FacetRegistry::instance().variant<AGCObject,AExpression>(test_);
                gc.forward_inplace(gco.iface(), (void **)&(test_.data_));
            }
            {
                auto gco = FacetRegistry::instance().variant<AGCObject,AExpression>(when_true_);
                gc.forward_inplace(gco.iface(), (void **)&(when_true_.data_));
            }
            {
                auto gco = FacetRegistry::instance().variant<AGCObject,AExpression>(when_false_);
                gc.forward_inplace(gco.iface(), (void **)&(when_false_.data_));
            }

            return shallow_size();
        }

        // ----------------------------------------------------------------

#ifdef NOPE
        auto IfExpr::check_consistent_valuetype(const rp<Expression> & when_true,
                                                const rp<Expression> & when_false) -> TypeDescr
        {
            if (when_true->valuetype() != when_false->valuetype())
                return nullptr;

            return when_true->valuetype();
        }

        void IfExpr::establish_valuetype()
        {
            if (this->when_true_.get() && this->when_false_.get())
                this->assign_valuetype(check_consistent_valuetype(this->when_true_, this->when_false_));
        }

        rp<IfExpr>
        IfExpr::make(const rp<Expression> & test,
                     const rp<Expression> & when_true,
                     const rp<Expression> & when_false)
        {
            /** TODO: verify test returns _boolean_ type **/

            if (when_true->valuetype() != when_false->valuetype()) {
                throw std::runtime_error
                    (tostr("IfExpr::make:"
                           " types {T1,T2} found for branches of if-expr"
                           " where equal types expected",
                           xtag("T1", when_true->valuetype()->canonical_name()),
                           xtag("T2", when_false->valuetype()->canonical_name())));
            }

            /* arbitrary choice here */
            auto ifexpr_type = when_true->valuetype();

            return new IfExpr(ifexpr_type,
                              test,
                              when_true,
                              when_false);
        } /*make*/

        void
        IfExpr::display(std::ostream & os) const {
            os << "<IfExpr"
               << xtag("test", test_)
               << xtag("when_true", when_true_);
            if (when_false_)
                os << xtag("when_false", when_false_);
            os << ">";
        } /*display*/

        std::uint32_t
        IfExpr::pretty_print(const ppindentinfo & ppii) const {
            return ppii.pps()->pretty_struct(ppii, "IfExpr");
#ifdef NOT_YET
            return ppii.pps()->pretty_struct(ppii, "IfExpr",
                                             refrtag("test", test_),
                                             refrtag("when_true", when_true_),
                                             refrtag("when_false", when_false_));
#endif
        }

        rp<IfExprAccess>
        IfExprAccess::make(rp<Expression> test,
                           rp<Expression> when_true,
                           rp<Expression> when_false)
        {
            auto ifexpr_type = check_consistent_valuetype(when_true, when_false);

            return new IfExprAccess(ifexpr_type, std::move(test), std::move(when_true), std::move(when_false));
        }

        rp<IfExprAccess>
        IfExprAccess::make_empty()
        {
            return new IfExprAccess(nullptr /*ifexpr_valuetype*/,
                                    nullptr /*test*/,
                                    nullptr /*when_true*/,
                                    nullptr /*when_false*/);
        }

        void
        IfExprAccess::assign_when_true(rp<Expression> x)
        {
            this->when_true_ = std::move(x);
        }

        void
        IfExprAccess::assign_when_false(rp<Expression> x)
        {
            this->when_false_ = std::move(x);
        }
#endif
    } /*namespace scm*/
} /*namespace xo*/

/* end DIfElseExpr.cpp */
