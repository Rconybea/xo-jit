/** @file ParserStateMachine.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "ParserResult.hpp"
#include <xo/expression2/DVariable.hpp>
#include <xo/expression2/StringTable.hpp>
#include <xo/tokenizer2/Token.hpp>
#include <xo/alloc2/Allocator.hpp>
#include <xo/arena/DArena.hpp>

namespace xo {
    namespace scm {
        // defined in ssm/ASyntaxStateMachine.hpp, but
        // including here would create include cycle
        //
        class ASyntaxStateMachine;

        // note: it's load-bearing here to forward-declare ParserStack,
        //       see ParserStack.hpp for impl
        //       because ASyntaxStateMachine.hpp includes ParserStateMachine.hpp;
        //       before obj<SyntaxStateMachine> is defined.
        class ParserStack;

        /** @brief State machine embodying Schematika parser
         **/
        class ParserStateMachine {
        public:
            using TypeDescr = xo::reflect::TypeDescr;
            using AAllocator = xo::mm::AAllocator;
            using ArenaConfig = xo::mm::ArenaConfig;
            using DArena = xo::mm::DArena;
            using size_type = std::size_t;

        public:
            ParserStateMachine(const ArenaConfig & config,
                               size_type max_stringtable_capacity,
                               obj<AAllocator> expr_alloc);

            /** @defgroup scm-parserstatemachine-accessors accessor methods **/
            ///@{

            bool debug_flag() const noexcept { return debug_flag_; }
            ParserStack * stack() const noexcept { return stack_; }
            obj<AAllocator> expr_alloc() const noexcept { return expr_alloc_; }
            const ParserResult & result() const noexcept { return result_; }

            /** true iff state machine is currently idle (at top-level) **/
            bool is_at_toplevel() const noexcept;

            /** true iff state machine currently has incomplete expression **/
            bool has_incomplete_expr() const noexcept;

            /** top of parser stack **/
            obj<ASyntaxStateMachine> top_ssm() const;

            ///@}

            /** @defgroup scm-parserstatemachine-bookkeeping bookkeeping methods **/
            ///@{

            /** allocator for parsing stack and ssm's **/
            DArena & parser_alloc() noexcept { return parser_alloc_; }

            /** establish toplevel @p ssm.  Must have empty stack **/
            void establish_toplevel_ssm(obj<ASyntaxStateMachine> ssm);

            /** push syntax @p ssm onto @ref stack_ **/
            void push_ssm(obj<ASyntaxStateMachine> ssm);

            /** pop syntax state machine from top of @ref stack_ **/
            void pop_ssm();

            /** get unique string copy of @p str. Idempotent for each @p str.
             **/
            const DUniqueString * intern_string(std::string_view str);

            /** add variable to current local environment (innermost lexical scope) **/
            void upsert_var(DVariable * var);

            /** reset result to none **/
            void reset_result();

            /** reset after reporting error **/
            void clear_error_reset();

            ///@}

            /** @defgroup scm-parserstatemachine-inputmethods input methods **/
            ///@{

            /** update state to respond to parsed symbol @p sym
             *  (from nested parsing state)
             **/
            void on_parsed_symbol(std::string_view sym);

            /** update state to respond to parsed type-description @p td
             *  (from nested parsing state)
             **/
            void on_parsed_typedescr(TypeDescr td);

            /** update state to respond to input token @p tk.
             *  record output (if any) in @ref result_
             **/
            void on_token(const Token & tk);

            /** operate state machine for incoming symbol-token @p tk **/
            void on_symbol_token(const Token & tk);

            /** operate state machine for incoming define-token @p tk **/
            void on_def_token(const Token & tk);

            /** operate state machine for incoming if-token @p tk **/
            void on_if_token(const Token & tk);

            /** operate state machine for incoming colon-token @p tk **/
            void on_colon_token(const Token & tk);

            /** operate state machine for incoming singleassign-token @p tk **/
            void on_singleassign_token(const Token & tk);

            /** operate state machine for incoming f64-token @p tk **/
            void on_f64_token(const Token & tk);

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
             *  expected input in current ssm state
             **/
            void illegal_input_on_token(std::string_view ssm_name,
                                        const Token & tk,
                                        std::string_view expect_str);

            /** report illegal input from syntax state machine @p ssm_name
             *  receiving parsed symbol @p sym. @p expect_str describes
             *  expected input in current ssm state
             **/
            void illegal_input_on_symbol(std::string_view ssm_name,
                                         std::string_view sym,
                                         std::string_view expect_str);

            /** report illegal input arriving in syntax state machine (ssm) @p ssm_name
             *  receiving assembled type-description @p td.
             *  @p expect_str sketches expected input in current ssm state
             **/
            void illegal_input_on_typedescr(std::string_view ssm_name,
                                            TypeDescr td,
                                            std::string_view expect_str);

            ///@}

        private:

            /** Table containing interned strings + symbols.
             **/
            StringTable stringtable_;

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
