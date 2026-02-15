/** @file VirtualSchematikaMachine.hpp
 *
 *  @author Roland Conybare, Jan 2026
 **/

#pragma once

#include "VsmConfig.hpp"
#include "VsmInstr.hpp"
#include "VsmFrame.hpp"
#include "DLocalEnv.hpp"
#include "DGlobalEnv.hpp"
#include <xo/object2/RuntimeError.hpp>
#include <xo/reader2/SchematikaReader.hpp>
#include <xo/expression2/Expression.hpp>
#include <xo/gc/GCObject.hpp>
#include <xo/alloc2/abox.hpp>

namespace xo {
    namespace scm {
#ifdef OBSOLETE // see DVsmError
        // TODO: move error to collected space?
        //       or special arena?
        //
        struct EvaluationError {
            /** source location (in vsm implementation) at which error identified **/
            std::string_view src_function_;
            /** error description (allocated from ErrorArena) **/
            std::string_view error_description_;
            // TODO: info about location in schematika source
        };
#endif

        /** similar to @ref xo::scm::ReaderResult **/
        struct VsmResult {
            using AGCObject = xo::mm::AGCObject;
            using span_type = xo::mm::span<const char>;

            VsmResult() = default;
            explicit VsmResult(obj<AGCObject> value) : result_{value} {}
            explicit VsmResult(TokenizerError err) : result_{err} {}

            bool is_value() const { return std::holds_alternative<obj<AGCObject>>(result_); }
            bool is_tk_error() const { return std::holds_alternative<TokenizerError>(result_); }
            bool is_eval_error() const;

            const obj<AGCObject> * value() const { return std::get_if<obj<AGCObject>>(&result_); }

            /** result of evaluating first expression encountered in input **/
            std::variant<obj<AGCObject>, TokenizerError> result_;
        };

        /** vsm result + reamining span **/
        struct VsmResultExt : public VsmResult {
            using span_type = VsmResult::span_type;

            VsmResultExt() = default;
            VsmResultExt(const VsmResult & result, span_type rem) : VsmResult{result}, remaining_{rem} {}

            /** unconsumed portion of input **/
            VsmResult::span_type remaining_;
        };

        /** @class VirtualSchematikaMachine
         *  @brief virtual machine for schematika
         **/
        class VirtualSchematikaMachine {
        public:
            // will be DArenaVector<obj<StackFrame>> probably
            using Stack = void *;
            using AAllocator = xo::mm::AAllocator;
            using AGCObject = xo::mm::AGCObject;
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;
            using span_type = xo::mm::span<const char>;

        public:
            /** @p config. configuration
             *  @p aux_mm.  Allocator for miscellaneous dataN
             *  owned by this VSM.
             **/
            VirtualSchematikaMachine(const VsmConfig & config,
                                     obj<AAllocator> aux_mm);

            /** allocator for schematika data **/
            obj<AAllocator> allocator() const noexcept;
            /** allocator for runtime errors **/
            obj<AAllocator> error_allocator() const noexcept;

            /** visit vsm-owned memory pools; call visitor(info) for each **/
            void visit_pools(const MemorySizeVisitor & visitor) const;

            /** begin interactive session. **/
            void begin_interactive_session();
            /** begin batch session **/
            void begin_batch_session();

            /** consume input @p input_cstr.
             *  Require: must first start interactive/batch session
             **/
            VsmResultExt read_eval_print(span_type input_span, bool eof);

            /** evaluate expression @p expr
             *  Require: must first start interactive/batch session
             **/
            VsmResult start_eval(obj<AExpression> expr);

            /** borrow calling thread to run indefinitely,
             *  until halt instruction
             **/
            void run();

            /** execute vsm instruction in @ref pc_.
             *  @retval instruction count. 1 unless pc_ is halt.
             **/
            bool execute_one();

        private:
            /** Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_op();

            /** evaluate a constant expression
             *  Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_constant_op();

            /** evaluate a define-expression
             *  Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_define_op();

            /** evaluate a lambda expression
             *  Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_lambda_op();

            /** evaluate variable expression (definition)
             *  Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_variable_op();

            /** evaluate a variable reference (use after definition)
             *  Require:
             *  - expression in @ref expr_
             **/
            void _do_eval_varref_op();

            /** evaluate an apply expression
             *  Require:
             * - expression in @ref expr_
             **/
            void _do_eval_apply_op();

            /** evaluate an if-else expression
             *  Require:
             * - expression in @ref expr_
             **/
            void _do_eval_if_else_op();

            /** evaluate a sequence expression
             *  Require:
             * - expression in @ref expr_
             **/
            void _do_eval_sequence_op();

            /** apply a function to evaluated arguments **/
            void _do_apply_op();

            /** evaluate arguments on behalf of a function call
             *  Require:
             * - expression value in @ref value_
             * - stack:
             *    [0] VsmEvalArgsFrame
             *    [1] VsmApplyFrame
             *    ...
             **/
            void _do_evalargs_op();

            /** call closure @ref fn_ with arguments @ref args_ **/
            void _do_call_closure_op();
            /** call primitive @ref fn_ with arguments @ref args_ **/
            void _do_call_primitive_op();

            /** restore registers from stack frame
             *  (specifically: local_env_, stack_, cont_)
             *  after invoking a schematika closure
             **/
            void _do_apply_cont_op();

            /** proceed with if- or else- branch of an if-else expression
             *  after evaluating test condition
             **/
            void _do_ifelse_cont_op();

            /** loop continuation after evaluating element of a SequenceExpr **/
            void _do_seq_cont_op();

        private:
            /*
             * Some registers are preserved by evaluation:
             *   stack_
             *   cont_
             *   local_env_
             *
             * Other registers not preserved
             *   pc_
             *   expr_
             *   fn_
             *   args_
             *   value_
             */

            /** configuration **/
            VsmConfig config_;

#ifdef NOT_YET
            /** allocator (likely DArena) for globals.
             *  For example DArenaHashMap in global symtab,
             **/
            obj<AAllocator> aux_mm_;
#endif

            /** allocator (likely DX1Collector or similar) for
             *  expressions and values
             **/
            abox<AAllocator> mm_;

            /** Sidecar allocator for error reporting.
             *  Separate to mitigate interference with @ref mm_
             *  (separate memory so we can for example report
             *   an out-of-memory error).
             *  Likely DArena or similar
             **/
            abox<AAllocator> error_mm_;

            /** runtime context for this vsm.
             *  For example, provides allocator to primitives
             **/
            abox<ARuntimeContext> rcx_;

            // consider separate allocator (which _may_ turn out to be the same)
            // for VM stack. Only works for code that doesn't rely on fancy
            // lexical scoping

            // consider separate allocator for reader (i.e. program code)
            // and data (program execution)

            /** reader: text -> expression **/
            SchematikaReader reader_;

            /** program counter **/
            VsmInstr pc_ = VsmInstr::c_halt;

            /** stack pointer **/
            obj<AGCObject> stack_;

            /** expression register **/
            obj<AExpression> expr_;

            /** environment pointer.  Provides bindings
             *  for surrounding lexical scope at this point
             *  in execution
             **/
            DLocalEnv * local_env_ = nullptr;

        protected:  // temporarily, to appease compiler
            /** environment pointer.  Maintains bindings
             *  for global variables.
             **/
            DGlobalEnv * global_env_ = nullptr;

        private:
            /** evaluated function to call **/
            obj<AGCObject> fn_;
            /** evaluated argument list **/
            DArray * args_;

            /** result register **/
            VsmResult value_;

            /** continuation register **/
            VsmInstr cont_ = VsmInstr::c_halt;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end VirtualSchematikaMachine.hpp */
