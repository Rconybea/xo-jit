/** @file VirtualSchematikaMachine.hpp
 *
 *  @author Roland Conybare, Jan 2026
 **/

#pragma once

#include "VsmConfig.hpp"
#include "VsmInstr.hpp"
#include "VsmFrame.hpp"
#include "LocalEnv.hpp"
#include "GlobalEnv.hpp"
#include <xo/object2/RuntimeError.hpp>
#include <xo/object2/Array.hpp>
#include <xo/reader2/SchematikaReader.hpp>
#include <xo/expression2/Expression.hpp>
#include <xo/alloc2/GCObject.hpp>
#include <xo/alloc2/abox.hpp>
#include <variant>

namespace xo {
    namespace scm {
        /** similar to @ref xo::scm::ReaderResult **/
        struct VsmResult {
            using AGCObject = xo::mm::AGCObject;
            using span_type = xo::mm::span<const char>;

            VsmResult() = default;
            explicit VsmResult(obj<AGCObject> value) : result_{value} {}

            bool is_value() const { return result_; }
            bool is_error() const;

            const obj<AGCObject> * value() const { return &result_; }
            const obj<AGCObject> & value_ref() const { return result_; }

            /** result of evaluating first expression encountered in input **/
            obj<AGCObject> result_;
        };

        /** vsm result + reamining span
         *
         *  Preserves address of wrapped VsmResult
         *  (so it can continue to be owned by DVirtualSchematikaMachine,
         *   and to be known to gc without add'l effort)
         **/
        struct VsmResultExt {
            using AGCObject = xo::mm::AGCObject;
            using span_type = VsmResult::span_type;

            VsmResultExt() = default;
            VsmResultExt(const VsmResult & result, span_type rem) : p_result_{&result}, remaining_{rem} {}

            bool is_empty() const { return !p_result_; }
            bool is_value() const { return p_result_ ? p_result_->is_value() : false; }
            bool is_error() const { return p_result_ ? p_result_->is_error() : false; }

            const obj<AGCObject> * value() const { return p_result_ ? p_result_->value() : nullptr; }
            //const obj<AGCObject> & value_ref() { return result_.value_ref(); }

            const VsmResult * p_result_ = nullptr;

            /** unconsumed portion of input **/
            VsmResult::span_type remaining_;
        };

        /** @class VirtualSchematikaMachine
         *  @brief virtual machine for schematika
         **/
        class DVirtualSchematikaMachine {
        public:
            // will be DArenaVector<obj<StackFrame>> probably
            using Stack = void *;
            using AGCObject = xo::mm::AGCObject;
            using AGCObjectVisitor = xo::mm::AGCObjectVisitor;
            using VisitReason = xo::mm::VisitReason;
            using AAllocator = xo::mm::AAllocator;
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;
            using span_type = xo::mm::span<const char>;

        public:
            /** @p config. configuration
             *  @p aux_mm.  Allocator for miscellaneous dataN
             *  owned by this VSM.
             **/
            DVirtualSchematikaMachine(const VsmConfig & config,
                                      obj<AAllocator> aux_mm);

            /** non-trivial dtor because of @ref reader_
             *  indirect dependency on DGlobalSymtab
             **/
            ~DVirtualSchematikaMachine() = default;

            /** create instance using memory from @p mm.
             *  with configuration @p config.
             *  Machine will use @p aux_mm for auxiliary (non-GC, non-arena)
             *  memory.
             **/
            static DVirtualSchematikaMachine * _make(obj<AAllocator> mm,
                                                    const VsmConfig & config,
                                                    obj<AAllocator> aux_mm);

            /** like _make(), but create fop **/
            static obj<AGCObject,DVirtualSchematikaMachine> make(obj<AAllocator> mm,
                                                                const VsmConfig & config,
                                                                obj<AAllocator> aux_mm);

            /** allocator for schematika data **/
            obj<AAllocator> allocator() const noexcept;
            /** allocator for runtime errors **/
            obj<AAllocator> error_allocator() const noexcept;
            /** global unique-string table **/
            StringTable * stringtable() noexcept;

            /** true iff parser is at top-level -> does not contain
             *  state for a incomplete/partial expression
             **/
            bool is_at_toplevel() const noexcept;

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
            const VsmResult & start_eval(obj<AExpression> expr);

            /** borrow calling thread to run indefinitely,
             *  until halt instruction
             **/
            void run();

            /** execute vsm instruction in @ref pc_.
             *  @retval instruction count. 1 unless pc_ is halt.
             **/
            bool execute_one();

            /** @defgroup scm-virtualschematikamachine-gcobject-facet gcobject facet **/
            ///@{

            /** object size.  Not implemented.  Only intending to support VSM as virtual root  **/
            std::size_t shallow_size() const noexcept;

            /** shallow copy during gc cycle.  Not implemented!  Only intending to support
             *  VSM as virtual root
             **/
            DVirtualSchematikaMachine * gco_shallow_move(obj<AGCObjectVisitor> gc) noexcept;

            /** forward gc-aware child pointers
             **/
            void visit_gco_children(VisitReason reason, obj<AGCObjectVisitor> gc) noexcept;

            ///@}

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

            /** perform assignment after evaluating
             *  the rhs of a define-expr
             **/
            void _do_def_cont_op();

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

            /** virtual gc root.
             *  Arranges for gc to traverse+forward pointers exiting VSM.
             *  It won't (and can't) copy/move the VSM itself, since alloc'd
             *  outside gc.
             **/
            obj<AGCObject> self_vroot_;

            /** allocator (likely DArena) for globals.
             *  For example DArenaHashMap in global symta.
             **/
            obj<AAllocator> aux_mm_;

            /** allocator (likely DX1Collector or similar) for
             *  expressions and values. Schemaatika reader will use this also.
             *
             *  Allocations must represent a type that supports GCObject.
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

            /** environment pointer.  Maintains bindings
             *  for global variables.  Obtained from reader
             **/
            obj<AGCObject,DGlobalEnv> global_env_;

            /** environment pointer.  Provides bindings
             *  for surrounding lexical scope at this point
             *  in execution
             **/
            obj<AGCObject,DLocalEnv> local_env_;

            /** evaluated function to call **/
            obj<AGCObject> fn_;
            /** evaluated argument list **/
            obj<AGCObject,DArray> args_;

            /** result register **/
            VsmResult value_;

            /** continuation register **/
            VsmInstr cont_ = VsmInstr::c_halt;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end DVirtualSchematikaMachine.hpp */
