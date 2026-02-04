/** @file VsmFrame.hpp
 *
 *  @author Roland Conybeare, Feb 2026
 **/

#pragma once

#include "VsmInstr.hpp"
#include <xo/procedure2/Procedure.hpp>
#include <xo/object2/DArray.hpp>

namespace xo {
    namespace scm {
        class VsmFrame {
        public:
            using AGCObject = xo::mm::AGCObject;

        public:
            VsmFrame(obj<AGCObject> parent,
                     VsmInstr cont) : parent_{parent}, cont_{cont} {}

            //obj<AGCObject> parent() const noexcept { return parent_; }
            obj<AGCObject> parent() const noexcept { return parent_; }
            VsmInstr cont() const noexcept { return cont_; }

        protected:
            /** saved VSM stack; restore when this frame consumed **/
            obj<AGCObject> parent_;
            /** saved continuation; restore when this frame consumed **/
            VsmInstr cont_;
        };

    }  /*namespace scm*/
} /*namespace xo*/

/* end VsmFrame.hpp */
