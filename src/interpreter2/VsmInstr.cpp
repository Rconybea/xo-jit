/** @file VsmInstr.cpp
*
 *  @author Roland Conybeare, Feb 2026
 **/

#include "VsmInstr.hpp"

namespace xo {
    namespace scm {
        VsmInstr
        VsmInstr::c_halt = VsmInstr(vsm_opcode::halt);

        VsmInstr
        VsmInstr::c_eval = VsmInstr(vsm_opcode::eval);
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmInstr.cpp */
