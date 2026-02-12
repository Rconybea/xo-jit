/** @file DApplySsm.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
#include "syntaxstatetype.hpp"
#include <xo/expression2/ApplyExpr.hpp>
//#include <xo/facet/obj.hpp>
#include <string_view>


namespace xo {
    namespace scm {
        /**
         *    fn ( arg1 , arg2 , .. , argn )
         *   ^  ^ ^    ^ ^    ^      ^    ^ ^
         *   |  | |    | |    |      |    | (done)
         *   |  | |    | |    |      |    apply_3
         *   |  | |    | |    |      apply_2:expect_rhs_expression
         *   |  | |    | |    apply_3
         *   |  | |    | apply_2:expect_rhs_expression
         *   |  | |    apply_3
         *   |  | apply_2:expect_rhs_expression
         *   |  apply_1
         *   apply_0:expect_rhs_expression
         *
         *   apply_0 --on_expr()--> apply_1
         *   apply_1 --on_leftparen()--> apply_2
         *   apply_2 --on_expr()--> apply_3
         *   apply_3 --on_comma()--> apply_2
         *           --on_rightparen()-> (done)
         *
         *   apply_0: start
         *   apply_1: leftparen following expr allows parser to recognize apply
         *   apply_2: expect next argument
         *   apply_3: got argument, expect comma or rightparen to continue
         *   (done): apply complete, pop exprstate from stack
         *
         * In practice will start in state apply_1
         **/
        enum class applyexprstatetype {
            invalid = -1,

            apply_0,
            apply_1,
            apply_2,  // rename -> apply_2a
            apply_3,  // rename -> apply_2b

            N
        };

        extern const char * applyexprstatetype_descr(applyexprstatetype x);

        std::ostream &
        operator<<(std::ostream & os, applyexprstatetype x);

        /** @class DApplySsm
         *  @brief state machine for parsing a schematika function-call-expression
         **/
        class DApplySsm : public DSyntaxStateMachine<DApplySsm> {
        public:
            using Super = DSyntaxStateMachine<DApplySsm>;
            using TypeDescr = xo::reflect::TypeDescr;
            using AAllocator = xo::mm::AAllocator;
            using DArena = xo::mm::DArena;
            using ppindentinfo = xo::print::ppindentinfo;

            //using Apply = xo::scm::Apply;

        public:
            /** @defgroup scm-applyssm-ctors constructors **/
            ///@{

            /** construct apply ssm with @p fn_expr
             *  supplying function to be invoked.
             *
             *  Expect during parsing of input like f(...)
             *  that we parse f before parser knows that it will be
             *  followed by leftparen
             **/
            explicit DApplySsm(obj<AExpression> fn_expr,
                               DArray * args);

            /** create instance using memory from @p parser_mm.
             *  with function to be called supplied by @p fn_expr.
             **/
            static DApplySsm * make(DArena & parser_mm,
                                    obj<AExpression> fn_expr);

            /**
             * Start apply. Will trigger this after input like
             *   "fn("
             * or
             *   "makefn()()"
             *
             * apply_xs remains on expr stack until closing right paren
             *    fn(arg1-expr, arg2-expr, ...)
             *
             * @p fnex   expression in function position
             * @p p_psm  parser state machine
             **/
            static void start(obj<AExpression> fnex,
                              ParserStateMachine * p_psm);
            ///@}
            /** @defgroup scm-applyssm-access-methods access methods **/
            ///@{

            /** identify this nested state machine **/
            static const char * ssm_classname() { return "DApplySsm"; }

            /** current internal state **/
            applyexprstatetype applystate() const noexcept { return applystate_; }

            ///@}
            /** @defgroup scm-applyssm-methods general methods **/
            ///@{

            /** handle leftparen token @p tk for this ssm,
             *  with overall parser state in @p p_psm
             **/
            void on_leftparen_token(const Token & tk,
                                    ParserStateMachine * p_psm);

            ///@}
            /** @defgroup ssm-applyssm-facet syntaxstatemachine facet methods **/
            ///@{

            /** identifies the ssm implemented here **/
            syntaxstatetype ssm_type() const noexcept;

            /** mnemonic for expected remaining syntax for current parsing state **/
            std::string_view get_expect_str() const noexcept;

            /** update this apply-ssm for incoming token @p tk,
             *  with overall parsing state in @p p_psm
             **/
            void on_token(const Token & tk,
                          ParserStateMachine * p_psm);

            ///@}

#ifdef NOT_YET

            virtual void on_expr(bp<Expression> expr,
                                 parserstatemachine * p_psm) override;

            virtual void on_comma_token(const token_type & tk,
                                        parserstatemachine * p_psm) override;
            virtual void on_rightparen_token(const token_type & tk,
                                             parserstatemachine * p_psm) override;

        private:
            static std::unique_ptr<apply_xs> make();
#endif

            /** @defgroup ssm-applyssm-printable-facet printable facet **/
            ///@{

            /** pretty-printing support **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** current state of parser for this apply expression **/
            applyexprstatetype applystate_ = applyexprstatetype::apply_0;
            /** evaluates to function to be invoked **/
            obj<AExpression> fn_expr_;
            /** args_expr_v_[i] evaluates to the i'th argument to call.
             *  Not using flexible array here since we don't know size at
             *  construction time
             *
             *  (though could revisit +  realloc this DApplySsm instance in
             *  place to optimize)
             **/
            DArray * args_expr_v_ = nullptr;
        };
    } /*namespace scm */

    namespace print {
#ifndef ppdetail_atomic
        PPDETAIL_ATOMIC(xo::scm::applyexprstatetype);
#endif
    }
} /*namespace xo*/

/* end DApplySsm.hpp */
