/** @file VirtualSchematikaMachine.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "VirtualSchematikaMachine.hpp"
#include "VsmApplyFrame.hpp"
#include "VsmEvalArgsFrame.hpp"
#include <xo/expression2/ApplyExpr.hpp>
#include <xo/expression2/Constant.hpp>
#include <xo/procedure2/RuntimeContext.hpp>
#include <xo/procedure2/SimpleRcx.hpp>
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

        // NOTE: using heap for {DX1Collector, DSimpleRcx} instances
        //       (though allocation from explictly mmap'd memory)
        //
        VirtualSchematikaMachine::VirtualSchematikaMachine(const VsmConfig & config)
        : config_{config},
          mm_(box<AAllocator,DX1Collector>(new DX1Collector(config.x1_config_))),
          rcx_(box<ARuntimeContext,DSimpleRcx>(new DSimpleRcx(mm_.to_op()))),
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
            //   DArray                 VsmApplyFrame                 VsmEvalArgsFrame
            //   v                      v                             v
            //   +----------------------+-------+-------+----+--------+-------+-------+-------+
            //   | argument expressions | par x | cont1 | fn | args x | par x | cont2 | i_arg |
            //   +----------------------+-----|-+-------+----+------|-+-----|-+-------+-------+
            //   ^                      ^     |                     |       |
            //   |                      \-----------------------------------/
            //   \                            |                     /
            //    \------------------------------------------------/
            //                                /
            //   <---------------------------/
            //
            // - VsmEvalArgsFrame: owned by VSM, state for evalargs loop
            // - VsmApplyFrame:    owned by VSM, state for transferring control to called function
            // - DArray:           contains evaluated args; owned by called primitive
            // - cont2:            always c_apply
            //

            auto apply = obj<AExpression,DApplyExpr>::from(expr_);

            // evaluated arguments
            DArray * args = DArray::empty(mm_.to_op(),
                                          apply->n_args());

            // TODO: check function signature

            DVsmApplyFrame * apply_frame
                = DVsmApplyFrame::make(mm_.to_op(), stack_, cont_, args);

            auto evalargs_frame
                = obj<AGCObject,DVsmEvalArgsFrame>
                      (DVsmEvalArgsFrame::make(mm_.to_op(),
                                               apply_frame, VsmInstr::c_apply, apply.data()));

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
            // rcx_  : runtime context
            // fn_   : function to call
            // args_ : array of arguments

            // TODO: check argument types

            this->value_ = fn_.apply_nocheck(rcx_.to_op(), args_);
            this->pc_ = cont_;

            return;
        }

        void
        VirtualSchematikaMachine::_do_evalargs_op()
        {
            scope log(XO_DEBUG(false));

            if (!value_.is_value()) {
                // error while evaluating function arg

                log.retroactively_enable();
                log && log("error in apply -> terminating app");

                this->pc_ = VsmInstr::c_halt;
                return;
            }

            // here: nested evaluation succeeded

            // value of one of {fn, arg(i), ..} in fn(arg0 .. arg(n-1))
            //
            obj<AGCObject> value = *(value_.value());

            //            value_ in [i_arg]              value_
            //            .   (if i_arg >= 0)            .  (if i_arg = -1)
            //            .                              .
            //   DArray   .             VsmApplyFrame    .            VsmEvalArgsFrame
            //   v        v             v                v            v
            //   +----------------------+-------+-------+----+--------+-------+-------+--------+-------+
            //   | argument expressions | par o | cont1 | fn | args x | par o | cont2 | applyx | i_arg |
            //   +----------------------+-----|-+-------+----+------|-+-----|-+-------+--------+-------+
            //   ^                      ^     |                     |       |
            //   |                      \-----------------------------------/
            //   \                            |                     /
            //    \------------------------------------------------/
            //                                /
            //   <---------------------------/
            //
            // - VsmEvalArgsFrame: owned by VSM, state for evalargs loop
            // - VsmApplyFrame:    owned by VSM, state for transferring control to called function
            // - DArray:           contains evaluated args; owned by called primitive

            // - i_arg
            //     if -1:  value_ register holds function
            //     if >=0: value_ register holds i'th function argument
            //

            auto evalargs_frame
                = obj<AGCObject,DVsmEvalArgsFrame>::from(stack_);

            assert(evalargs_frame);

            int32_t i_arg = evalargs_frame->i_arg();

            DVsmApplyFrame * apply_frame = evalargs_frame->parent();

            const DApplyExpr * apply_expr
                = evalargs_frame->apply_expr();

            if (i_arg == -1) {
                auto fn = value.to_facet<AProcedure>();

                if (fn) {
                    apply_frame->assign_fn(fn);

                    i_arg = evalargs_frame->increment_arg();

                    // now i_arg is 0 -> evaluate that argument

                    this->expr_ = apply_expr->arg(i_arg);
                    this->pc_ = VsmInstr::c_eval;
                    //this->cont_ = VsmInstra::c_evalargs;  // redundant, since preserved

                    return;
                } else {
                    // error - function position must deliver something with AProcedure?
                    // or DClosure, but we'll get to that.

                    log.retroactively_enable();
                    log("expected procedure in function position -> terminate");

                    assert(false);
                }
            } else {
                DArray * args = apply_frame->args();

                log && log(xtag("i_arg", i_arg), xtag("n_arg", args->size()), xtag("cap", args->capacity()));

                args->push_back(value);

                i_arg = evalargs_frame->increment_arg();

                if (i_arg == static_cast<int32_t>(apply_expr->n_args())) {
                    // all apply-arguments have been evaluated
                    //  -> done with VsmEvalArgsFrame
                    //

                    this->fn_ = apply_frame->fn();
                    this->args_ = apply_frame->args();

                    this->stack_ = apply_frame->parent();
                    this->pc_ = VsmInstr::c_apply;
                    this->cont_ = apply_frame->cont();

                    return;

                } else {
                    this->expr_ = apply_expr->arg(i_arg);
                    this->pc_ = VsmInstr::c_eval;
                    //this->cont_ = VsmInstra::c_evalargs;  // redundant, since preserved

                    return;
                }
            }

            // not implemented
            assert(false);
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end VirtualSchematikaMachine.hpp */
