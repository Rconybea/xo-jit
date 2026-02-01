/** @file DLambdaExpr.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DLambdaExpr.hpp"
#include "detail/IExpression_DLambdaExpr.hpp"
#include "DLocalSymtab.hpp"
#include "symtab/IPrintable_DLocalSymtab.hpp"
#include <xo/alloc2/Allocator.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/reflectutil/typeseq.hpp>

namespace xo {
    using xo::print::APrintable;
    using xo::facet::FacetRegistry;
    using xo::reflect::TypeDescr;
    using xo::reflect::TypeDescrBase;
    using xo::reflect::FunctionTdxInfo;
    using xo::reflect::typeseq;
    using xo::print::quot;

    namespace scm {

#ifdef NOT_YET
        TypeDescr
        assemble_lambda_td()
        {
            std::vector<TypeDescr> arg_td_v;
            {
                arg_td_v.reserve(local_symtab->size());

                for (DLocalSymtab::size_type i = 0, n = local_symtab->size(); i < n; ++i) {
                    const DVariable * var = local_symtab->lookup_var(i);

                    if (!var)
                        break;

                    TypeDescr arg_td = var->valuetype();

                    if (!arg_td)
                        break;

                    arg_td_v.push_back(arg_td);
                }
            }
        }
#endif

        DLambdaExpr::DLambdaExpr(TypeRef typeref,
                                 const DUniqueString * name,
                                 DLocalSymtab * local_symtab,
                                 obj<AExpression> body) : typeref_{typeref},
                                                          name_{name},
                                                          local_symtab_{local_symtab},
                                                          body_expr_{body}
        {
        }

        obj<AExpression,DLambdaExpr>
        DLambdaExpr::make(obj<AAllocator> mm,
                          TypeRef typeref,
                          const DUniqueString * name,
                          DLocalSymtab * local_symtab,
                          obj<AExpression> body)
        {
            return obj<AExpression,DLambdaExpr>(_make(mm, typeref,
                                                      name, local_symtab, body));
        }

        DLambdaExpr *
        DLambdaExpr::_make(obj<AAllocator> mm,
                           TypeRef typeref,
                           const DUniqueString * name,
                           DLocalSymtab * local_symtab,
                           obj<AExpression> body)
        {
            // in general we're not going to know argument types yet.
            // perhaps want to delay this until after type resolution.

            void * mem = mm.alloc(typeseq::id<DLambdaExpr>(), sizeof(DLambdaExpr));

            return new (mem) DLambdaExpr(typeref,
                                         name,
                                         local_symtab,
                                         body);
        }

        TypeDescr
        DLambdaExpr::assemble_lambda_td(DLocalSymtab * symtab,
                                        TypeDescr return_td)
        {
            assert(return_td);

            std::vector<TypeDescr> arg_td_v;
            {
                DLocalSymtab::size_type z = symtab->size();

                arg_td_v.reserve(z);

                for (DLocalSymtab::size_type i = 0; i < z; ++i) {
                    auto param = symtab->lookup_var(Binding::local(i));

                    assert(param);
                    arg_td_v.push_back(param->valuetype());
                }
            }

            auto function_tdx = FunctionTdxInfo(return_td, arg_td_v, false /*!is_noexcept*/);

            TypeDescr lambda_td = TypeDescrBase::require_by_fn_info(function_tdx);

            return lambda_td;
        }

        exprtype
        DLambdaExpr::extype() const noexcept {
            return exprtype::lambda;
        }

        TypeRef
        DLambdaExpr::typeref() const noexcept {
            return typeref_;
        }

        TypeDescr
        DLambdaExpr::valuetype() const noexcept {
            return typeref_.td();
        }

        void
        DLambdaExpr::assign_valuetype(TypeDescr td) noexcept {
            typeref_.resolve(td);
        }

        bool
        DLambdaExpr::pretty(const ppindentinfo & ppii) const
        {
            auto body
                = FacetRegistry::instance().try_variant<APrintable,
                                                        AExpression>(body_expr_);

            if (name_ && body) {
                auto local_symtab_pr
                    = obj<APrintable,DLocalSymtab>(local_symtab_);

                return ppii.pps()->pretty_struct(ppii,
                                                 "LambdaExpr",
                                                 refrtag("tref", typeref_),
                                                 refrtag("name", quot(std::string_view(*name_))),
                                                 refrtag("local_symtab", local_symtab_pr),
                                                 //refrtag("argv", local_env_->argv()),
                                                 refrtag("body", body));
            } else {
                return ppii.pps()->pretty_struct(ppii,
                                                 "LambdaExpr");
            }
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end DLambda.cpp */
