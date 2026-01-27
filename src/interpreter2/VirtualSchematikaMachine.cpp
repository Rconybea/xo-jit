/** @file VirtualSchematikaMachine.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "VirtualSchematikaMachine.hpp"
#include <xo/expression2/detail/IExpression_DConstant.hpp>
#include <xo/expression2/DConstant.hpp>
#include <cassert>

namespace xo {
    namespace scm {

        VirtualSchematikaMachine::VirtualSchematikaMachine()
        {}

        void
        VirtualSchematikaMachine::run()
        {
            while (this->execute_one())
                ;
        }

        bool
        VirtualSchematikaMachine::execute_one()
        {
            switch (pc_.opcode()) {
            case vsm_opcode::halt:
            case vsm_opcode::N:
                return false;
            case vsm_opcode::eval:
                _do_eval_op();
            }

            return true;
        }

        void
        VirtualSchematikaMachine::_do_eval_op()
        {
            switch(expr_.extype()) {
            case exprtype::invalid:
            case exprtype::N:
                break;
            case exprtype::constant:
                _do_eval_constant_op();
                break;
            case exprtype::define:
                _do_eval_define_op();
                break;
            case exprtype::variable:
                _do_eval_variable_op();
                break;
            case exprtype::apply:
                _do_eval_apply_op();
                break;
            case exprtype::ifexpr:
                _do_eval_if_else_op();
                break;
            }
        }

        void
        VirtualSchematikaMachine::_do_eval_constant_op()
        {
            auto expr
                = obj<AExpression,DConstant>::from(expr_);

            this->value_ = expr.data()->value();
            this->pc_ = this->cont_;
        }

        void
        VirtualSchematikaMachine::_do_eval_define_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_variable_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_apply_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_if_else_op()
        {
            // not implemented
            assert(false);
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end VirtualSchematikaMachine.hpp */
