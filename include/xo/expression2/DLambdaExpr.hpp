/** @file DLambdaExpr.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "Expression.hpp"
#include "TypeRef.hpp"
#include "exprtype.hpp"
#include "DLocalSymtab.hpp"
#include "DString.hpp"
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        /** @class DLambdaExpr
         *  @brief syntax tree for a function/procedure definition
         *
         **/
        class DLambdaExpr {
        public:
            using AAllocator = xo::mm::AAllocator;
            using TypeDescr = xo::reflect::TypeDescr;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = DLocalSymtab::size_type;

        public:

            /** @defgroup scm-lambdaexpr-ctors **/
            ///@{

            DLambdaExpr(TypeRef typeref,
                        const DUniqueString * name,
                        DLocalSymtab * local_symtab,
                        obj<AExpression> body);

            /** create instance using memory from @p mm **/
            static obj<AExpression,DLambdaExpr> make(obj<AAllocator> mm,
                                                     TypeRef typeref,
                                                     const DUniqueString * name,
                                                     DLocalSymtab * local_symtab,
                                                     obj<AExpression> body);

            /** create instance, using memory from @p mm **/
            static DLambdaExpr * _make(obj<AAllocator> mm,
                                       TypeRef typeref,
                                       const DUniqueString * name,
                                       DLocalSymtab * local_symtab,
                                       obj<AExpression> body);

            /** create type description for lambda with arguments described by @p symtab
             *  and return type @p return_td.
             *  Load-bearing for DLambdaSsm in xo-reader2/
             **/
            static TypeDescr assemble_lambda_td(DLocalSymtab * symtab,
                                                TypeDescr return_td);

            ///@}
            /** @defgroup scm-lambdaexpr-methods **/
            ///@{

            DLocalSymtab * local_symtab() const noexcept { return local_symtab_; }
            size_type n_args() const noexcept { return local_symtab_->size(); }

            // get_free_variables()
            // visit_preorder()
            // visit_layer()
            // xform_layer()
            // attach_envs(SymbolTable*)

            ///@}
            /** @defgroup scm-lambdaexpr-expression-facet **/
            ///@{

            exprtype extype() const noexcept;
            TypeRef typeref() const noexcept;
            TypeDescr valuetype() const noexcept;
            void assign_valuetype(TypeDescr td) noexcept;

            ///@}
            /** @defgroup scm-lambdaexpr-printable-facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** expression value always has type consistent
             *  with description here
             **/
            TypeRef typeref_;

            /** name for this lambda (generated if necessary) **/
            const DUniqueString * name_ = nullptr;

#ifdef NOT_YET
            /** e.g.
             *    i64(f64,string)
             *  for function of two arguments with types (f64, string) respectively,
             *  that returns an i64.
             **/
            const DUniqueString * type_name_str_ = nullptr;
#endif

            /** symbol table for lambda arguments **/
            DLocalSymtab * local_symtab_ = nullptr;;

            /** expression for function body **/
            obj<AExpression> body_expr_;

            // free_var_set
            // captured_var_set
            // layer_var_map
            // nested_lambda_map
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DLambdaExpr.hpp */
