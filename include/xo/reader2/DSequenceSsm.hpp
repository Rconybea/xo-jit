/** @file DSequenceSsm.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "DSyntaxStateMachine.hpp"
//#include "exprstate.hpp"

namespace xo {
    namespace scm { class Sequence; }
    namespace scm { class Lambda; }

    namespace scm {
        class DSequenceSsm : public DSyntaxStateMachine<DSequenceSsm> {
        public:
            using Sequence = xo::scm::Sequence;
            using Lambda = xo::scm::Lambda;

        public:
            const char * ssm_classname() const noexcept { return "DSequenceSsm"; }

#ifdef NOT_YET
            /** start parsing a sequence-expr.
             *  input begins with first expression in the sequence.
             **/
            static void start(parserstatemachine * p_psm);

            /** named ctor idiom **/
            static std::unique_ptr<sequence_xs> make();

            virtual void on_expr(bp<Expression> expr,
                                 parserstatemachine * p_psm) override;
            virtual void on_expr_with_semicolon(bp<Expression> expr,
                                                parserstatemachine * p_psm) override;

            virtual void on_rightbrace_token(const token_type & tk,
                                             parserstatemachine * p_psm) override;

            virtual void print(std::ostream & os) const override;
            virtual bool pretty_print(const xo::print::ppindentinfo & ppii) const override;
#endif

        private:
            DSequenceSsm();

        private:
            /** will build SequenceExpr from in-order contents of this array **/
            DArray * expr_v_;
            //std::vector<rp<Expression>> expr_v_;
        };
    } /*namespace scm*/
} /*namespace xo*/


/* end DSequenceSsm.hpp */
