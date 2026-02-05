/** @file DClosure.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#include "LocalEnv.hpp"
#include <xo/expression2/LambdaExpr.hpp>

namespace xo {
    namespace scm {

        /** @brief runtime representation for a procedure
         *
         *  Maintains lambda + captured lexical context
         **/
        class DClosure {
        public:
            using AAllocator = xo::mm::AAllocator;
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

            /** for now, support just fixed-arity procedures **/
            bool is_nary() const noexcept { return false; }
            /** number of arguments expected by this procedure (-1 if nary) **/
            size_type n_args() const noexcept { return lambda_->n_args(); }

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
