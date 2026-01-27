/** @file VirtualSchematikaMachine.hpp
 *
 *  @author Roland Conybare, Jan 2026
 **/

#pragma once

#include "VsmInstr.hpp"
#include <xo/expression2/Expression.hpp>
#include <xo/gc/GCObject.hpp>

namespace xo {
    namespace scm {
        /** @class VirtualSchematikaMachine
         *  @brief virtual machine for schematika
         **/
        class VirtualSchematikaMachine {
        public:
            // will be DArenaVector<obj<StackFrame>> probably
            using Stack = void *;
            using AGCObject = xo::mm::AGCObject;

        public:
            VirtualSchematikaMachine();

            /** borrow calling thread to run indefinitely,
             *  until halt instruction
             **/
            void run();

            /** execute vsm instruction in @ref pc_.
             *  @retval instruction count. 1 unless pc_ is halt.
             **/
            bool execute_one();

        private:
            /** Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_op();

            /** evaluate a constant expression
             *  Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_constant_op();

            /** evaluate a define-expression
             *  Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_define_op();

            /** evaluate a variable expression
             *  Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_variable_op();

            /** evaluate an apply expression
             *  Require:
             * - expression in @ref expr_
             **/
            void _do_eval_apply_op();

            /** evaluate an if-else expression
             *  Require:
             * - expression in @ref expr_
             **/
            void _do_eval_if_else_op();

        private:
            /*
             * Some registers are preserved by evaluation:
             *   stack_
             *   cont_
             *
             * Other registers are not preserved
             *   pc_
             *   expr_
             *   value_
             */

            /** program counter **/
            VsmInstr pc_ = VsmInstr::halt();

#ifdef NOT_YET
            /** stack pointer **/
            Stack stack_;
#endif

            /** expression register **/
            obj<AExpression> expr_;

            /** result register **/
            obj<AGCObject> value_;

            /** continuation register **/
            VsmInstr cont_ = VsmInstr::halt();
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end VirtualSchematikaMachine.hpp */
