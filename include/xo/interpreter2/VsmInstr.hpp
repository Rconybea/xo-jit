/** @file VsmInstr.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "VsmOpcode.hpp"

namespace xo {
    namespace scm {
        /**
         *  Thin instruction wrapper for VSM (virtual schematika machine) instructions.
         *  For exeuction see VirtualSchematikaMachine.cpp
         **/
        class VsmInstr {
        public:
            explicit VsmInstr(vsm_opcode oc) : opcode_{oc} {}

            // instructions

            static VsmInstr c_sentinel;
            static VsmInstr c_halt;
            static VsmInstr c_eval;

            /** proceed to assignment after evaluating rhs
             *  of define-expression
             **/
            static VsmInstr c_def_cont;

            static VsmInstr c_apply;
            static VsmInstr c_evalargs;
            /** restore VSM state for continuation of an apply expression **/
            static VsmInstr c_apply_cont;

            /** proceed to branch of if-else expression after evaluating
             *  test condition
             **/
            static VsmInstr c_ifelse_cont;

            /** loop to evaluate members of a SequenceExpr **/
            static VsmInstr c_seq_cont;

            vsm_opcode opcode() const noexcept { return opcode_; }

        private:
            vsm_opcode opcode_;
        };

        inline bool
        operator==(VsmInstr x, VsmInstr y) noexcept {
            return x.opcode() == y.opcode();
        }

        inline std::ostream &
        operator<<(std::ostream & os, VsmInstr x) {
            os << x.opcode();
            return os;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmInstr.hpp */
