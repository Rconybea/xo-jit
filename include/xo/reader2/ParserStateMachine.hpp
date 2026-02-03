/** @file ParserStateMachine.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "ParserResult.hpp"
#include <xo/expression2/DGlobalSymtab.hpp>
#include <xo/expression2/DLocalSymtab.hpp>
#include <xo/expression2/DVariable.hpp>
#include <xo/expression2/StringTable.hpp>
#include <xo/tokenizer2/Token.hpp>
#include <xo/object2/DArray.hpp>
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
            using MemorySizeInfo = xo::mm::MemorySizeInfo;
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
            DLocalSymtab * local_symtab() const noexcept { return local_symtab_; }
            const ParserResult & result() const noexcept { return result_; }

            /** true iff state machine is currently idle (at top-level) **/
            bool is_at_toplevel() const noexcept;

            /** true iff state machine currently has incomplete expression **/
            bool has_incomplete_expr() const noexcept;

            /** top of parser stack **/
            obj<ASyntaxStateMachine> top_ssm() const;

            /** number of distinct memory pools owned by PS **/
            std::size_t _n_store() const noexcept;
            /** memory consumption for i'th memory pool **/
            MemorySizeInfo _store_info(std::size_t i) const noexcept;

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

            /** get unique (within stringtable) string, beginning with @p prefix **/
            const DUniqueString * gensym(std::string_view prefix);

            /** push nested local symtab while parsing the body of a lambda expression;
             *  restore previous symtab at the end of lambda-expression definition.
             *  See @ref pop_local_symtab
             **/
            void push_local_symtab(DLocalSymtab * symtab);

            /** pop nested symbol table from symbol-table stack **/
            void pop_local_symtab();

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

            /** update state to consume param (name, value) emitted by
             *  nested (expired) parsing state
             **/
            void on_parsed_formal(const DUniqueString * param_name,
                                  TypeDescr param_type);

            /** update state to consume formal arugment list
             *  emitted by nested (expired) parsing state
             **/
            void on_parsed_formal_arglist(DArray * arglist);

            /** update state to respond to parsed expression @p expr
             *  (from nested parsing state)
             **/
            void on_parsed_expression(obj<AExpression> expr);

            /** update state to respond to parsed expression @p expr
             *  (from nested parsing state), with trailing semicolon.
             *
             *  Need to distinguish cases like:
             *    6    // ; allowed
             *    f(6  // ) allowed    ; forbidden
             *    6 +  // ) forbidden  ; forbidden
             *
             **/
            void on_parsed_expression_with_semicolon(obj<AExpression> expr);

            /** update state to respond to parsed expression @p expr
             *  (from nested parsing state), with trailing token @p tk.
             *
             *  Need to distinguish cases like:
             *    6    // ) ?          ; allowed    } ?
             *    f(6  // ) allowed    ; forbidden  } forbidden
             *    6 +  // ) forbidden  ; forbidden  } forbidden
             *
             **/
            void on_parsed_expression_with_token(obj<AExpression> expr,
                                                 const Token & tk);

            /** update state to respond to input token @p tk.
             *  record output (if any) in @ref result_
             **/
            void on_token(const Token & tk);

            ///@}
            /** @defgroup scm-parserstatemachine-error-entrypoints error entry points **/
            ///@{

            /** capture result expression @p expr **/
            void capture_result(std::string_view ssm_anme,
                                obj<AExpression> expr);

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

            /** report illegal parsed formal (param_name, param_type) from nested ssm.
             *  Introducing as placeholder; not expected to be reachable in
             *  full parser
             **/
            void illegal_parsed_formal(std::string_view ssm_name,
                                       const DUniqueString * param_name,
                                       TypeDescr param_type,
                                       std::string_view expect_str);

            /** @p arglist stores obj<AGCObject,DVariable> pointers.
             **/
            void illegal_parsed_formal_arglist(std::string_view ssm_name,
                                               DArray * arglist,
                                               std::string_view expect_str);

            /** report illegal parsed expression from nested ssm.
             *  Introducing as placeholder; not clear if this will be reachable
             *  in full parser
             **/
            void illegal_parsed_expression(std::string_view ssm_name,
                                           obj<AExpression>,
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
            /** parser stack. Memory always from @ref parser_alloc_;
             *  elements that should survive parsing allocate from
             *  @ref expr_alloc_, see below.
             **/
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

            /** symbol table with local bindings.
             *  non-null during parsing of lambda expressions.
             *  Always allocated from @p expr_alloc_.
             *  Push local symbol table here to remember local params
             *  during the body of a lambda expression.
             **/
            DLocalSymtab * local_symtab_ = nullptr;

#ifdef NOT_YET
            /** global symbol table.
             *  Toplevel definitions go here.
             **/
            DGlobalSymtab * global_symtab_ = nullptr;
#endif

            /** current output from parser **/
            ParserResult result_;

            /** true to enable debug output **/
            bool debug_flag_ = false;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end ParserStateMachine.hpp */
