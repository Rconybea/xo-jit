/** @file VirtualSchematikaMachine.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "VirtualSchematikaMachine.hpp"
#include "VsmApplyFrame.hpp"
#include "VsmEvalArgsFrame.hpp"
#include <xo/expression2/ApplyExpr.hpp>
#include <xo/expression2/Constant.hpp>
#include <xo/gc/DX1Collector.hpp>
#include <xo/gc/detail/IAllocator_DX1Collector.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <cassert>

namespace xo {
    using xo::print::APrintable;
    using xo::print::ppconfig;
    using xo::print::ppstate_standalone;
    using xo::mm::AGCObject;
    using xo::mm::MemorySizeInfo;
    using xo::mm::DX1Collector;
    using xo::facet::FacetRegistry;
    using std::cout;

    namespace scm {

        VirtualSchematikaMachine::VirtualSchematikaMachine(const VsmConfig & config)
        : config_{config},
          mm_(box<AAllocator,DX1Collector>(new DX1Collector(config.x1_config_))),
          reader_{config.rdr_config_, mm_.to_op()}
        {}

        void
        VirtualSchematikaMachine::visit_pools(const MemorySizeVisitor & visitor) const
        {
            mm_.visit_pools(visitor);
            reader_.visit_pools(visitor);
        }

        void
        VirtualSchematikaMachine::begin_interactive_session()
        {
            reader_.begin_interactive_session();
        }

        void
        VirtualSchematikaMachine::begin_batch_session()
        {
            reader_.begin_batch_session();
        }

        VsmResultExt
        VirtualSchematikaMachine::read_eval_print(span_type input, bool eof)
        {
            if (input.empty()) {
                return VsmResultExt();
            }

            auto [expr, remaining, error1]
                = reader_.read_expr(input, eof);

            if (!expr) {
                /* tokenizer error */

                return VsmResultExt(VsmResult(error1), remaining);
            }

            VsmResult evalresult = this->start_eval(expr);

            if (evalresult.is_eval_error() || evalresult.is_tk_error()) {
                return VsmResultExt(evalresult, remaining);
            }

            assert(evalresult.is_value());

            obj<AGCObject> * p_value = std::get_if<obj<AGCObject>>(&(evalresult.result_));

            assert(p_value);

            obj<APrintable> value_pr
                = FacetRegistry::instance().variant<APrintable,AGCObject>(*p_value);

            // pretty_toplevel(value_pr, &cout, ppconfig());
            ppconfig ppc;
            ppstate_standalone pps(&cout, 0, &ppc);
            pps.prettyn(value_pr);

            return VsmResultExt(VsmResult(*p_value), remaining);
        }

        VsmResult
        VirtualSchematikaMachine::start_eval(obj<AExpression> expr)
        {
            this->pc_ = VsmInstr::c_eval;
            this->expr_ = expr;
            this->value_ = obj<AGCObject>();
            this->cont_ = VsmInstr::c_halt;

            this->run();

            return value_;
        }

        void
        VirtualSchematikaMachine::run()
        {
            while (this->execute_one())
                ;
        }

        bool
        VirtualSchematikaMachine::execute_one()
        {
            scope log(XO_DEBUG(true));
            log && log(xtag("pc", pc_),
                       xtag("cont", cont_));

            obj<APrintable> stack_pr
                = (FacetRegistry::instance()
                   .try_variant<APrintable,AGCObject>(stack_));

            if (stack_pr)
                log && log(xtag("stack", stack_pr));

            switch (pc_.opcode()) {
            case vsm_opcode::halt:
            case vsm_opcode::N:
                return false;
            case vsm_opcode::eval:
                _do_eval_op();
                break;
            case vsm_opcode::apply:
                _do_apply_op();
                break;
            case vsm_opcode::evalargs:
                _do_evalargs_op();
                break;
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
            case exprtype::lambda:
                _do_eval_lambda_op();
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
            case exprtype::sequence:
                _do_eval_sequence_op();
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
        VirtualSchematikaMachine::_do_eval_lambda_op()
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
            // ApplyExpr in expr_ register

            // assuming bump allocator:
            //
            //   DArray                 VsmApplyFrame                VsmEvalArgsFrame
            //   v                      v                            v
            //   +----------------------+-------+------+----+--------+-------+------+-------+
            //   | argument expressions | par x | cont | fn | args x | par x | cont | i_arg |
            //   +----------------------+-----|-+------+----+------|-+-----|-+------+-------+
            //   ^                      ^     |                    |       |
            //   |                      \----------------------------------/
            //   \                            |                    /
            //    \-----------------------------------------------/
            //                                /
            //   <---------------------------/
            //
            // - VsmEvalArgsFrame: owned by VSM, state for evalargs loop
            // - VsmApplyFrame:    owned by VSM, state for transferring control to called function
            // - DArray:           contains evaluated args; owned by called primitive

            auto apply = obj<AExpression,DApplyExpr>::from(expr_);

            // evaluated arguments
            DArray * args = DArray::empty(mm_.to_op(),
                                          apply->n_args());

            // TODO: check function signature

            auto apply_frame
                = obj<AGCObject,DVsmApplyFrame>
                (DVsmApplyFrame::make(mm_.to_op(), stack_, cont_, args));

            auto evalargs_frame
                = obj<AGCObject,DVsmEvalArgsFrame>
                (DVsmEvalArgsFrame::make(mm_.to_op(), apply_frame, VsmInstr::c_apply));

            this->stack_ = evalargs_frame;

            // Setup evaluation of first argument.  No new stack for this.

            this->cont_ = VsmInstr::c_evalargs;
            this->expr_ = apply->fn();
            this->pc_   = VsmInstr::c_eval;
        }

        void
        VirtualSchematikaMachine::_do_eval_if_else_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_sequence_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_apply_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_evalargs_op()
        {
            // not implemented
            assert(false);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end VirtualSchematikaMachine.hpp */
