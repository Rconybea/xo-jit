/** @file VsmInstr.cpp
*
 *  @author Roland Conybeare, Feb 2026
 **/

#include "VsmInstr.hpp"

namespace xo {
    namespace scm {
        const char *
        vsm_opcode_descr(vsm_opcode x)
        {
            switch (x) {
            case vsm_opcode::halt: return "halt";
            case vsm_opcode::eval: return "eval";
            case vsm_opcode::apply: return "apply";
            case vsm_opcode::evalargs: return "evalargs";
            case vsm_opcode::N:
                break;
            }

            return "opcode?";
        }

        VsmInstr
        VsmInstr::c_halt = VsmInstr(vsm_opcode::halt);

        VsmInstr
        VsmInstr::c_eval = VsmInstr(vsm_opcode::eval);

        VsmInstr
        VsmInstr::c_apply = VsmInstr(vsm_opcode::apply);

        VsmInstr
        VsmInstr::c_evalargs = VsmInstr(vsm_opcode::evalargs);
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmInstr.cpp */
