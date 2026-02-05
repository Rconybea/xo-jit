/** @file DClosure.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "LocalEnv.hpp"
#include <xo/expression2/LambdaExpr.hpp>
#include <xo/procedure2/RuntimeContext.hpp>

namespace xo {
    namespace scm {

        /** @brief runtime representation for a procedure
         *
         *  Maintains lambda + captured lexical context
         **/
        class DClosure {
        public:
            using ARuntimeContext = xo::scm::ARuntimeContext;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::int32_t;

        public:
            DClosure(const DLambdaExpr * lm,
                     const DLocalEnv * env);

            /** create instance using memory from @p mm
             *  for lambda @p lm with captured environment @p env.
             **/
            static DClosure * make(obj<AAllocator> mm,
                                   const DLambdaExpr * lm,
                                   const DLocalEnv * env);

            /** @defgroup scm-closure-general-methods **/
            ///@{

            const DLambdaExpr * lambda() const noexcept { return lambda_; }
            const DLocalEnv * env() const noexcept { return env_; }

            ///@}
            /** @defgroup scm-closure-procedure-facet **/
            ///@{

            /** for now, support just fixed-arity procedures **/
            bool is_nary() const noexcept { return false; }
            /** number of arguments expected by this procedure (-1 if nary) **/
            size_type n_args() const noexcept { return lambda_->n_args(); }

            obj<AGCObject> apply_nocheck(obj<ARuntimeContext> rcx, const DArray * args);

            ///@}
            /** @defgroup scm-closure-gcobject-facet **/
            ///@{

            ///@}
            /** @defgroup scm-closure-printable-facet **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** lambda expression **/
            const DLambdaExpr * lambda_ = nullptr;
            /** bindings for captured variables
             *  (from lexical context where lambda evaluated)
             **/
            const DLocalEnv * env_ = nullptr;
        };

    } /*namespace scm*/
} /*namespace xo*/

/* end DClosure.hpp */
