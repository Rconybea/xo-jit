/** @file VsmOpcode.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <cstdint>

namespace xo {
    namespace scm {
        /** Opcode for a virtual schematika expression;
         *  exeucted by VirtualSchematikaMachine
         **/
        enum class vsm_opcode {
            /** Immediately halt virtual schematika machine. **/
            halt,
            /** Evaluate expression in expr register **/
            eval,

            /** sentinel, counts number of opcodes **/
            N,
        };

        static constexpr uint32_t n_opcode = static_cast<uint32_t>(vsm_opcode::N);
    } /*namespace scm*/
} /*namespace xo*/

/* end VsmOpcode.hpp */
