/** @file VsmOpcode.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <ostream>
#include <cstdint>

namespace xo {
    namespace scm {
        /** Opcode for a virtual schematika expression;
         *  exeucted by VirtualSchematikaMachine
         **/
        enum class vsm_opcode {
            /** Flags bad state (defect in VSM itself) **/
            sentinel,
            /** Immediately halt virtual schematika machine. **/
            halt,
            /** Evaluate expression in expr register **/
            eval,

            /** Apply function in stack frame
             *  See diagram in VirtualSchematikaMachine::_do_eval_apply_op
             **/
            apply,
            /** Eval arguments to function.
             *  See diagram in VirtualSchematikaMachine::_do_eval_apply_op
             **/
            evalargs,

            /** continuation to complete execution of define-expression,
             *  after evaluting rhs expression
             **/
            def_cont,

            /** continuation to restore vsm registers (local_env, stack, cont)
             *  after invoking a closure
             **/
            apply_cont,

            /** continuation to act on a branch **/
            ifelse_cont,

            /** Loop over elements of a SequenceExpr **/
            seq_cont,

            /** sentinel, counts number of opcodes **/
            N,
        };

        static constexpr uint32_t n_opcode = static_cast<uint32_t>(vsm_opcode::N);

        /** stringified enum value **/
        const char *
        vsm_opcode_descr(vsm_opcode x);

        inline std::ostream &
        operator<<(std::ostream & os, vsm_opcode x) {
            os << vsm_opcode_descr(x);
            return os;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmOpcode.hpp */
