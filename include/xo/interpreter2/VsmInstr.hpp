/** @file VsmInstr.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "VsmOpcode.hpp"

namespace xo {
    namespace scm {
        class VsmInstr {
        public:
            explicit VsmInstr(vsm_opcode oc) : opcode_{oc} {}

            static VsmInstr halt() { return VsmInstr{vsm_opcode::halt}; }
            static VsmInstr eval() { return VsmInstr{vsm_opcode::eval}; }

            vsm_opcode opcode() const noexcept { return opcode_; }

        private:
            vsm_opcode opcode_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmInstr.hpp */
