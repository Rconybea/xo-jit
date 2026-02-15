/** @file VirtualSchematikaMachine.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include "VirtualSchematikaMachine.hpp"
#include "VsmApplyFrame.hpp"
#include "VsmEvalArgsFrame.hpp"
#include "VsmApplyClosureFrame.hpp"
#include "VsmIfElseContFrame.hpp"
#include "VsmSeqContFrame.hpp"
#include "VsmRcx.hpp"
#include "Closure.hpp"
#include <xo/expression2/ApplyExpr.hpp>
#include <xo/expression2/LambdaExpr.hpp>
#include <xo/expression2/Constant.hpp>
#include <xo/expression2/SequenceExpr.hpp>
#include <xo/object2/Boolean.hpp>
#include <xo/procedure2/RuntimeContext.hpp>
//#include <xo/procedure2/SimpleRcx.hpp>
#include <xo/gc/DX1Collector.hpp>
#include <xo/gc/detail/IAllocator_DX1Collector.hpp>
#include <xo/alloc2/Arena.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <cassert>

namespace xo {
    using xo::print::APrintable;
    using xo::print::ppconfig;
    using xo::print::ppstate_standalone;
    using xo::mm::AGCObject;
    //using xo::mm::MemorySizeInfo;  // not used yet
    using xo::mm::AAllocator;
    using xo::mm::DX1Collector;
    using xo::mm::DArena;
    using xo::facet::FacetRegistry;
    using std::cout;

    namespace scm {

        bool
        VsmResult::is_eval_error() const
        {
            if (std::holds_alternative<obj<AGCObject>>(result_)) {
                auto err = obj<AGCObject,DRuntimeError>::from(*(this->value()));

                return err;
            } else {
                return false;
            }
        }

        // NOTE: using heap here for {DX1Collector, DArena, DVsmRcx} instances
        //       (though DX1Collector allocations will be from explictly mmap'd memory)
        //
        VirtualSchematikaMachine::VirtualSchematikaMachine(const VsmConfig & config,
                                                           obj<AAllocator> aux_mm)
        : config_{config},
          mm_(abox<AAllocator,DX1Collector>::make(aux_mm, config.x1_config_)),
          rcx_(abox<ARuntimeContext,DVsmRcx>::make(aux_mm, this)),
          reader_{config.rdr_config_, mm_.to_op(), aux_mm}
        {
            {
                DArena * arena = new DArena(config_.error_config_);
                assert(arena);

                error_mm_.adopt(obj<AAllocator,DArena>(arena));
            }

            // TODO: allocate global_env
        }

        obj<AAllocator>
        VirtualSchematikaMachine::allocator() const noexcept
        {
            return mm_.to_op();
        }

        obj<AAllocator>
        VirtualSchematikaMachine::error_allocator() const noexcept
        {
            return error_mm_.to_op();
        }

        void
        VirtualSchematikaMachine::visit_pools(const MemorySizeVisitor & visitor) const
        {
            mm_.visit_pools(visitor);
            error_mm_.visit_pools(visitor);
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
            this->value_ = VsmResult(obj<AGCObject>());
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
            scope log(XO_DEBUG(config_.debug_flag_));
            log && log(xtag("pc", pc_),
                       xtag("cont", cont_));

            obj<APrintable> stack_pr = stack_.to_facet<APrintable>();
//                = (FacetRegistry::instance()
//                   .try_variant<APrintable,AGCObject>(stack_));

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
            case vsm_opcode::apply_cont:
                _do_apply_cont_op();
                break;
            case vsm_opcode::ifelse_cont:
                _do_ifelse_cont_op();
                break;
            case vsm_opcode::seq_cont:
                _do_seq_cont_op();
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
            case exprtype::varref:
                _do_eval_varref_op();
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

            this->value_ = VsmResult(expr.data()->value());
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
            // assuming bump allocator
            //
            //   +----------- DArray---------+ +-------------DLocalEnv-----------+ +-----DClosure-------+
            //   | .cap |.size | .elts_[]... |h| .parent x | .symtab x | .args x |h| .lambda x | .env x |
            //   +------+------+-------------+ +---------|-+---------|-+-------|-+ +---------|-+------|-+
            //   ^                             ^         |           |         |             |        |
            //   \-----------------------------|---------|-----------|---------/             |        |
            //                                 |         |           |                       |        |
            //                                 \---------|-----------|-----------------------|--------/
            //                                           |           |                       |
            //    <--------------------------------------/           |                       |
            //                                                       |                       |
            //                                                       v                       v
            //                                                 DLocalSymtab             DLambdaExpr
            //
            //    DClosure      runtime procedure (created below)
            //    DArray        bound non-local variables (established by VSM)
            //    DLocalEnv     local environment (copy ref from VSM state)
            //    h             alloc header
            //    DLocalSymtab  local symbol table (created by parser)
            //    DLambdaExpr   lambda expression (created by parser)
            //

            // will create DClosure with local_env_

            // local_env_
            // global_env_

            auto lambda
                = obj<AExpression,DLambdaExpr>::from(expr_);

            DClosure * closure = DClosure::make(mm_.to_op(),
                                                lambda.data(),
                                                local_env_);

            this->value_
                = VsmResult(obj<AGCObject>(obj<AGCObject,DClosure>(closure)));
            this->pc_ = this->cont_;
        }

        void
        VirtualSchematikaMachine::_do_eval_variable_op()
        {
            // not implemented
            assert(false);
        }

        void
        VirtualSchematikaMachine::_do_eval_varref_op()
        {
            auto var = obj<AExpression,DVarRef>::from(expr_);

            Binding b = var->path();

            if (!local_env_) {
                // need lookup on global_env_
                assert(false);
            }

            auto value = local_env_->lookup_value(b);

            if (value) {
                this->value_ = VsmResult(value);
                this->pc_ = this->cont_;
                return;
            }

            // no binding

            auto error = DRuntimeError::make(mm_.to_op(),
                                             "_do_eval_varref_op",
                                             "no binding for variable");
            this->value_ = VsmResult(error);

            // for now: halt VSM execution
            // TODO: some combination of
            // 1. emit stack trace
            // 2. go to debugger
            // 3. have every vsm instruction check inputs for errors

            this->pc_ = VsmInstr::c_halt;
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

            // accumulate evaluated arguments here
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
            // control:
            //   self -> eval(test) -> ifelse_cont -> eval(when_true)
            //                                     -> eval(when_false)

            auto ifelse_expr = obj<AExpression,DIfElseExpr>::from(expr_);

            obj<AGCObject,DVsmIfElseContFrame> ifelse_frame
                (DVsmIfElseContFrame::make(mm_.to_op(),
                                           stack_, cont_, ifelse_expr.data()));

            this->stack_ = ifelse_frame;
            this->cont_ = VsmInstr::c_ifelse_cont;
            this->expr_ = ifelse_expr->test();
            this->pc_ = VsmInstr::c_eval;
        }

        void
        VirtualSchematikaMachine::_do_eval_sequence_op()
        {
            // stack:
            //
            //   VsmEvalSequence
            //   v
            //   +-------+------+-------+-------+
            //   | par x | cont |  seq  | i_elt |
            //   +-----|-+------+-------+-------+
            //         |
            //   <-----/
            //

            auto seq_expr = obj<AExpression,DSequenceExpr>::from(expr_);

            if (seq_expr->size() == 0) {
                /* empty sequence expression does not produce a value */

                this->value_ = VsmResult(obj<AGCObject>());
                this->pc_ = this->cont_;
                return;
            }

            auto seqexpr_frame
                = obj<AGCObject,DVsmSeqContFrame>
                      (DVsmSeqContFrame::make(mm_.to_op(),
                                              this->stack_ /*saved stack*/,
                                              this->cont_ /*saved cont*/,
                                              seq_expr.data() /*saved expr*/,
                                              0 /*index of seq element*/));

            this->stack_ = seqexpr_frame;

            // Setup evaluation of first sequence element

            this->cont_ = VsmInstr::c_seq_cont;
            this->expr_ = (*seq_expr.data())[0];
            this->pc_ = VsmInstr::c_eval;
        }

        void
        VirtualSchematikaMachine::_do_apply_op()
        {
            // rcx_  : runtime context
            // fn_   : function to call
            // args_ : array of arguments

            // TODO: check argument types

            auto closure = obj<AGCObject,DClosure>::from(fn_);

            if (closure) {
                _do_call_closure_op();
                return;
            } else {
                _do_call_primitive_op();
                return;
            }
        }

        void
        VirtualSchematikaMachine::_do_call_closure_op()
        {
            // We need to preserve registers while evaluating
            // lambda body

            auto closure = obj<AGCObject,DClosure>::from(fn_);

            assert(closure);

            // TODO: for tail recursion:
            //       check whether stack_ already refers to a
            //       DVsmApplyClosureFrame instance, in which case
            //       we can just refer to it instead of pushing a new one

            if (cont_ == VsmInstr::c_apply_cont) {
                // we are making a tail call.
                // No need to preserve (stack, cont, local_env),
                // since continuation will restore on top of them
                // frame top stackframe anyway
            } else {
                obj<AGCObject,
                    DVsmApplyClosureFrame> frame(
                        DVsmApplyClosureFrame::make(mm_.to_op(),
                                                    stack_,
                                                    cont_,
                                                    local_env_));

                // push frame w/ saved vsm registers
                this->stack_ = frame;
                this->cont_ = VsmInstr::c_apply_cont;
            }

            auto lambda = closure->lambda();

            auto local_env
                = DLocalEnv::_make(mm_.to_op(),
                                   local_env_,
                                   lambda->local_symtab(),
                                   args_);

            this->local_env_ = local_env;
            this->expr_ = lambda->body_expr();
            this->pc_ = VsmInstr::c_eval;
        }

        void
        VirtualSchematikaMachine::_do_call_primitive_op()
        {
            auto fn = fn_.to_facet<AProcedure>();

            this->value_ = VsmResult(fn.apply_nocheck(rcx_.to_op(), args_));
            this->pc_ = cont_;
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
                bool is_closure = obj<AGCObject,DClosure>::from(value);
                bool is_native_fn = value.try_to_facet<AProcedure>();

                if (is_native_fn || is_closure) {
                    apply_frame->assign_fn(value);

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

        void
        VirtualSchematikaMachine::_do_apply_cont_op()
        {
            // see DVsmApplyClosureFrame

            auto frame = obj<AGCObject,DVsmApplyClosureFrame>::from(stack_);

            assert(frame);

            this->stack_ = frame->parent();
            this->local_env_ = frame->local_env();
            this->pc_ = frame->cont();
        }

        void
        VirtualSchematikaMachine::_do_ifelse_cont_op()
        {
            // pre: result of evaluating test condition in value_ register

            auto frame = obj<AGCObject,DVsmIfElseContFrame>::from(stack_);

            assert(frame);
            assert(value_.is_value());

            auto flag = obj<AGCObject,DBoolean>::from(*value_.value());

            if (flag.data()) {
                obj<AExpression> next_expr;
                {
                    if (flag->value()) {
                        // proceed with if-branch
                        next_expr = frame->ifelse_expr()->when_true();
                    } else {
                        // proceed with else-branch
                        next_expr = frame->ifelse_expr()->when_false();
                    }
                }

                this->stack_ = frame->parent();
                this->cont_ = frame->cont();
                this->expr_ = next_expr;
                this->pc_ = VsmInstr::c_eval;
            } else {
                auto error = DRuntimeError::make(mm_.to_op(),
                                                 "_do_ifelse_cont_op",
                                                 "expected boolean for test condition");
                this->value_ = VsmResult(error);

                // for now: halt VSM execution
                // TODO: some combination of
                // 1. emit stack trace
                // 2. go to debugger
                // 3. have every vsm instruction check inputs for errors

                this->pc_ = VsmInstr::c_halt;
            }
        }

        void
        VirtualSchematikaMachine::_do_seq_cont_op()
        {
            auto frame = obj<AGCObject,DVsmSeqContFrame>::from(stack_);

            assert(frame);

            uint32_t i_seq = 1 + frame->i_seq();

            auto seq_expr = frame->seq_expr();

            assert(seq_expr);

            if (i_seq == seq_expr->size()) {
                /* done with sequence
                 * value of sequence-expr is the value of the last expression in that sequence,
                 * which is already in the value_ register
                 */

                this->stack_ = frame->parent();
                this->pc_ = frame->cont();
                return;
            } else {
                frame->incr_i_seq();

                this->cont_ = VsmInstr::c_seq_cont;
                this->expr_ = (*seq_expr)[i_seq];
                this->pc_ = VsmInstr::c_eval;
                return;
            }
         }

    } /*namespace scm*/
} /*namespace xo*/

/* end VirtualSchematikaMachine.hpp */
