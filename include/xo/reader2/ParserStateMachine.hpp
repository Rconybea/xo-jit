/** @file ParserStateMachine.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "ParserResult.hpp"
#include <xo/tokenizer2/Token.hpp>
#include <xo/alloc2/Allocator.hpp>
#include <xo/arena/DArena.hpp>

namespace xo {
    namespace scm {
        // defined in ssm/ASyntaxStateMachine.hpp, but
        // including here would create include cycle
        //
        class ASyntaxStateMachine;

        // note: load-bearing to forward-declare ParserStack,
        //       because ASyntaxStateMachine.hpp includes ParserStateMachine.hpp;
        //       before obj<SyntaxStateMachine> is defined.
        class ParserStack;

        /** @brief State machine embodying Schematika parser
         **/
        class ParserStateMachine {
        public:
            using AAllocator = xo::mm::AAllocator;
            using ArenaConfig = xo::mm::ArenaConfig;
            using DArena = xo::mm::DArena;

        public:
            ParserStateMachine(const ArenaConfig & config);

            /** @defgroup scm-parserstatemachine-accessors accessor methods **/
            ///@{

            bool debug_flag() const noexcept { return debug_flag_; }
            ParserStack * stack() const noexcept { return stack_; }
            const ParserResult & result() const noexcept { return result_; }
            obj<AAllocator> expr_alloc() const noexcept { return expr_alloc_; }

            /** true iff state machine is currently idle (at top-level) **/
            bool is_at_toplevel() const noexcept;

            /** true iff state machine currently has incomplete expression **/
            bool has_incomplete_expr() const noexcept;

            ///@}

            /** @defgroup scm-parserstatemachine-bookkeeping bookkeeping methods **/
            ///@{

            /** establish toplevel @p ssm.  Must have empty stack **/
            void establish_toplevel_ssm(obj<ASyntaxStateMachine> ssm);

            /** push syntax @p ssm onto @ref stack_ **/
            void push_ssm(obj<ASyntaxStateMachine> ssm);

            /** reset result to none **/
            void reset_result();

            /** reset after reporting error **/
            void clear_error_reset();

            ///@}

            /** @defgroup scm-parserstatemachine-inputmethods input methods **/
            ///@{

            /** update state to respond to input token @p tk.
             *  record output (if any) in @ref result_
             **/
            void on_token(const Token & tk);

            /** update state for incoming if-token @p tk **/
            void on_if_token(const Token & tk);

            ///@}

            /** @defgroup scm-parserstatemachine-error-entrypoints error entry points **/
            ///@{

            /** capture error message @p errmsg from @p ssm_name,
             *  as current state machine output.
             *
             *  @p errmsg will have been allocated from the @p expr_alloc_ allocator
             **/
            void capture_error(std::string_view ssm_name,
                               const DString * errmsg);

            /** report illegal input from syntax state machine @p ssm_name
             *  recognized on input token @p tk. @p expect_str describes
             *  expected input in that state
             **/
            void illegal_input_on_token(std::string_view ssm_name,
                                        const Token & tk,
                                        std::string_view expect_str);

            ///@}

        private:

            /** Arena for internal parsing stack.
             *  Must be owned exclusively because destructively
             *  modified as parser completes parsing of each sub-expression
             *
             *  Contents will be a stack of ExprState instances
             **/
            DArena parser_alloc_;

            /** Checkpoint of toplevel parser allocator.
             *  Retore parser_alloc to this checkpoint to proceed
             *  after encountering a parsing error.
             **/
            DArena::Checkpoint parser_alloc_ckp_;

            /** parser stack. Memory from @ref parser_alloc_ **/
            ParserStack * stack_ = nullptr;

            /** Allocator for parsed expressions.
             *  Information available during subsequent execution
             *  (whether compiling or interpreting) must be stored here.
             *
             *  Also use this allocator for error messages arising
             *  during parsing
             *
             *  Memory use patterns for executions are not predictable,
             *  and benefit from garbage collection, e.g. DX1Collector.
             *
             *  May alternatively be able to use DArena in a compile-only
             *  scenario, where top-level Expressions can be discarded
             *  once compiled.
             **/
            obj<AAllocator> expr_alloc_;

            /** current output from parser **/
            ParserResult result_;

            /** true to enable debug output **/
            bool debug_flag_ = false;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStateMachine.hpp */
