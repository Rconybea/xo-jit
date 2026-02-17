/** @file SchematikaParser.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "ParserConfig.hpp"
#include "ParserStateMachine.hpp"
#include "ParserResult.hpp"
#include <xo/tokenizer2/Token.hpp>
#include <xo/alloc2/Allocator.hpp>

namespace xo {
    namespace scm {
        /** schematica parser
         *
         *  Examples:
         *
         *    decltype point
         *
         *    // forward declarations
         *    decl pi : f64;
         *    decl fib(n : i32) -> i32;
         *
         *    def pi = 3.14159265;  // constant. = is single assignment
         *
         *    def fib(n : i32) -> i32 {
         *      // nested defs ok
         *      def aux(n : i32, s1 : i32, s2 : i32) -> i32 {
         *        // or:
         *        //   (n == 0) ? s1 : aux(n - 1, s1 + s2, s1)
         *        //
         *        if (n == 0) {
         *          s1;
         *        } else {
         *          aux(n - 1, s1 + s2, s1);
         *        }
         *
         *        // or:
         *        //  if (n == 0) ? s1 : aux(n - 1, s1 + s2, s1)
         *      }
         *
         *      aux(n=n, s1=1, s2=0);
         *    }
         *
         *    def x := "fu"; // non-constant
         *    x += "bar";
         *
         *    def anotherfib = lambda(n : i32) { fib(n) };
         *
         *    def any : object;
         *    def l : list<object> = '();
         *
         *    deftype point :: {x : f64, y : f64};
         *    deftype polar :: {arg : f64, mag : f64};
         *    deftype converter :: (point -> polar);
         *
         *    def polar2rect(pt : polar) -> point {
         *      point(x = pt.mag * cos(arg),
         *            y = pt.mag * sin(arg));
         *    }
         *
         * Grammar:
         *   toplevel-program = $toplevel-expression(1); ..; $toplevel-expression(n)
         *
         * if interactive:
         *   toplevel-expression = expression
         * else
         *   toplevel-expression = type-decl | define-expr
         *
         *   type-decl        = decltype $typename [<$tp1 .. $tpn>]
         *   expression       = type-decl
         *                       | define-expr
         *                       | literal-expr
         *                       | variable-expr
         *                       | apply-expr
         *                       | if-expr
         *                       | lambda-expr
         *                       | arithmetic-expr
         *                       | block
         *
         *   define-expr      = type-decl
         *                       | type-def
         *                       | variable-def
         *                       | function-decl
         *                       | function-def
         *
         *   type-def         = deftype $typename [<$tp1 .. $tpn>] :: type-def-rhs
         *   type-def-rhs     = object
         *                       | bool
         *                       | i128 | i64 | i32 | i16 | i8
         *                       | f128 | f64 | f32 | f16
         *                       | struct $typename { ($membername(i) : $typename(i))* }
         *                          [end $typename]
         *                       | tuple $typename { $typename(1), .., $typename(n) }
         *                          [end $typename]
         *                       | copytype $typename
         *                       | subtype $typename { ($member(i) : $typename(i))* }
         *
         *   variable-def     = decl $varname [: $typename] [= expression]
         *   function-decl    = decl $functionname($varname(1) : $typename(1),
         *                                         ..,
         *                                         $varname(n) : $typename(n)) -> $typename[ret]
         *   function-def     = def $functionname($varname(1) : $typename(1),
         *                                        ..,
         *                                        $varname(n) : $typename(n)) [-> $typename[ret]]
         *                       body-expr
         *                       [ end $functionname ]
         *   literal-expr     = boolean-literal
         *                       | integer-literal
         *                       | fp-literal
         *                       | string-literal
         *                       | symbol-literal
         *                       | struct-literal
         *
         *
         *   boolean-literal  = true | false
         *
         *   variable-expr    = $varname
         *   apply-expr       = fn-expr(arg-expr(1), .., arg-expr(n))
         *     fn-expr          = expression
         *     arg-expr(i)      = expression
         *
         *   if-expr          = if (test-expr) then-block else else-block
         *                       | ((test-expr) ? then-expr : else-expr)
         *     test-expr        = expression
         *     then-block       = block
         *     else-block       = block
         *
         *   block            = { (definition | expression)* }
         *
         *   lambda-expr      = lambda ($paramname(1) : $type(1),
         *                              ..,
         *                              $paramname(n) : $type(n)) body-expr
         *    body-expr       = expression
         *
         *   arithmetic-expr  = expression binop expression
         *
         *    binop           = +
         *                       | -
         *                       | *
         *                       | /
         *                       | |
         *                       | &
         *                       | ^
         *                       | ==
         *                       | !=
         *                       | <
         *                       | <=
         *                       | =>
         *                       | >
         *
         **/
        class SchematikaParser {
        public:
            using token_type = Token;
            using ArenaHashMapConfig = xo::map::ArenaHashMapConfig;
            using ArenaConfig = xo::mm::ArenaConfig;
            using AAllocator = xo::mm::AAllocator;
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;
            using ppindentinfo = xo::print::ppindentinfo;
            using size_type = std::size_t;

        public:
            /** create parser in initial state;
             *  parser is ready to receive tokens via @ref include_token
             *
             *  @p config        parser configuration
             *  @p expr_alloc    allocator for schematika expressions.
             *                   Probably shared with execution.
             *  @p aux_alloc     aux allocator for non-copyable memory
             *                   with lifetime bounded by this
             *                   SchematikeParser itself
             **/
            SchematikaParser(const ParserConfig & config,
                             obj<AAllocator> expr_alloc,
                             obj<AAllocator> aux_alloc);

            /** non-trivial dtor because of @ref psm_ **/
            ~SchematikaParser() = default;

            /** scm-schematikaparser-access-methods **/
            ///@{

            DGlobalSymtab * global_symtab() const noexcept;

            bool debug_flag() const { return debug_flag_; }

            /** true if parser is at top-level,
             *  i.e. ready for next top-level expression
             **/
            bool is_at_toplevel() const;

            /** true iff parser contains state for an incomplete expression.
             *  For this to be true, parser must have consumed at least one token
             *  since end of last toplevel expression
             **/
            bool has_incomplete_expr() const;

            /** top of parser stack **/
            obj<ASyntaxStateMachine> top_ssm() const;

            /** current parser result. Value of last return from @ref on_token **/
            const ParserResult & result() const;

            /** visit parser-owned memory pools; invoke visitor(info) for each **/
            void visit_pools(const MemorySizeVisitor & visitor) const;

            ///@}
            /** scm-schematikaparser-general-methods **/
            ///@{

            /** put parser into state for beginning an interactive session.
             **/
            void begin_interactive_session();

            /** put parser into state for beginning of a translation unit
             *  (i.e. input stream)
             **/
            void begin_batch_session();

            /** include next token @p tk and increment parser state.
             *
             *  @param tk  next input token
             *  @return parsed expression, if @p tk completes an expression.
             *  otherwise nullptr
             **/
            const ParserResult & on_token(const token_type & tk);

            /** reset parsed result expression; use using return value from
             *  @ref include_token. Complicating api here to avoid copying ParserResult
             *  on each token
             **/
            void reset_result();

            /** reset to starting parsing state.
             *  use this after encountering an error, to avoid cascade of
             *  spurious secondary errors. particularly important when
             *  invoked as part of a REPL.
             **/
            void reset_to_idle_toplevel();

            ///@}
            /** scm-schematikaparser-pretty-methods **/
            ///@{

            /** print human-readable representation on stream @p os **/
            void print(std::ostream & os) const;
            /** pretty-printer support **/
            bool pretty(const ppindentinfo & ppii) const;

            ///@}

        private:
            /** state machine **/
            ParserStateMachine psm_;

            /** debug flag (also stored in psm_) **/
            bool debug_flag_ = false;
        }; /*SchematikaParser*/

        inline std::ostream &
        operator<< (std::ostream & os,
                    const SchematikaParser * x) {
            if (x) {
                x->print(os);
            } else {
                os << "nullptr";
            }
            return os;
        }

    } /*namespace scm*/

    namespace print {
        /** pretty printer in <xo/indentlog/print/pretty.hpp> relies on this specialization
         *  to handle ParserResult instances
         **/
        template <>
        struct ppdetail<xo::scm::SchematikaParser*> {
            static inline bool print_pretty(const ppindentinfo & ppii, const xo::scm::SchematikaParser* p) {
                if (p)
                    return p->pretty(ppii);
                else
                    return ppii.pps()->print_upto("nullptr");
            }
        };
    }
} /*namespace xo*/

/* end SchematikaParser.hpp */
