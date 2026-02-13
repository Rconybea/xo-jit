/** @file SchematikaParser.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/reader2/SchematikaParser.hpp>
#include <xo/reader2/DDefineSsm.hpp>
#include <xo/reader2/DExpectExprSsm.hpp>
#include <xo/reader2/ssm/ISyntaxStateMachine_DExpectExprSsm.hpp>
#include <xo/reader2/ssm/ISyntaxStateMachine_DDefineSsm.hpp>
#include <xo/reader2/init_reader2.hpp>
#include <xo/expression2/DefineExpr.hpp>
#include <xo/expression2/ApplyExpr.hpp>
#include <xo/expression2/Constant.hpp>
#include <xo/procedure2/Primitive_gco_2_gco_gco.hpp>
#include <xo/object2/Float.hpp>
#include <xo/object2/Integer.hpp>
#include <xo/object2/String.hpp>
#include <xo/alloc2/arena/IAllocator_DArena.hpp>
#include <catch2/catch.hpp>

namespace xo {
    using xo::scm::SchematikaParser;
    using xo::scm::ASyntaxStateMachine;
    using xo::scm::syntaxstatetype;
//    using xo::scm::DDefineSsm;
    using xo::scm::DExpectExprSsm;

    using xo::scm::AExpression;
    using xo::scm::DDefineExpr;
    using xo::scm::DApplyExpr;
    using xo::scm::DConstant;

    //using xo::scm::ParserResult;
    using xo::scm::Token;
    using xo::mm::AGCObject;
    using xo::scm::DPrimitive_gco_2_gco_gco;
    using xo::scm::DString;
    using xo::scm::DFloat;
    using xo::scm::DInteger;

    using xo::mm::ArenaConfig;
    using xo::mm::AAllocator;
    using xo::mm::DArena;
    using xo::facet::with_facet;

    static InitEvidence s_init = (InitSubsys<S_reader2_tag>::require());

    namespace ut {
        void
        utest_tokenizer_loop(SchematikaParser * parser, std::vector<Token> & tk_v, bool debug_flag)
        {
            scope log(XO_DEBUG(debug_flag));

            size_t i_tk = 0;
            size_t n_tk = tk_v.size();
            for (const auto & tk : tk_v) {
                INFO(tostr(xtag("i_tk", i_tk), xtag("tk", tk)));

                auto & result = parser->on_token(tk);

                log && log("after token", xtag("i_tk", i_tk), xtag("tk", tk));
                log && log(xtag("parser", parser));
                log && log(xtag("result", result));

                if (i_tk + 1 < n_tk) {
                    REQUIRE(parser->has_incomplete_expr() == true);
                    REQUIRE(!result.is_error());
                    REQUIRE(result.is_incomplete());
                } else {
                    /* last token in tk_v[] */

                    REQUIRE(parser->has_incomplete_expr() == false);
                    REQUIRE(!result.is_error());
                    REQUIRE(result.is_expression());
                    REQUIRE(result.result_expr());
                }

                ++i_tk;
            }
        }

        TEST_CASE("SchematikaParser-ctor", "[reader2][SchematikaParser]")
        {
            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            REQUIRE(parser.debug_flag() == false);
            REQUIRE(parser.is_at_toplevel() == true);
        }

        TEST_CASE("SchematikaParser-begin-interactive", "[reader2][SchematikaParser]")
        {
            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            // after begin_interactive_session, parser has toplevel exprseq
            // but is still "at toplevel" in the sense of ready for input
            REQUIRE(parser.has_incomplete_expr() == false);
        }

        TEST_CASE("SchematikaParser-begin-batch", "[reader2][SchematikaParser]")
        {
            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_batch_session();

            // after begin_translation_unit, parser has toplevel exprseq
            // but is still "at toplevel" in the sense of ready for input
            REQUIRE(parser.has_incomplete_expr() == false);
        }

        TEST_CASE("SchematikaParser-batch-def", "[reader2][SchematikaParser]")
        {
            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_batch_session();

            /**  Walkthrough parsing input equivalent to:
             *
             *     def foo : f64 = 3.141593 ;
             *
             **/

            std::vector<Token> tk_v{
                Token::def_token(),
                Token::symbol_token("foo"),
                Token::colon_token(),
                Token::symbol_token("f64"),
                Token::singleassign_token(),
                Token::f64_token("3.141593"),
                Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            const auto & result = parser.result();
            {
                auto expr = obj<AExpression,DDefineExpr>::from(result.result_expr());
                REQUIRE(expr);
            }
        }

        TEST_CASE("SchematikaParser-interactive-integer", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    1011 ;
             *
             **/

            {
                auto & result = parser.on_token(Token::i64_token("1011"));

                log && log("after integer token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::semicolon_token());

                log && log("after semicolon token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == false);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_expression());
                REQUIRE(result.result_expr());

                auto expr = obj<AExpression,DConstant>::from(result.result_expr());
                REQUIRE(expr);

                REQUIRE(expr->value());

                auto value_i64 = obj<AGCObject,DInteger>::from(expr->value());

                REQUIRE(value_i64);

                REQUIRE(value_i64->value() == 1011);
            }

            //REQUIRE(result.is_error());
            //// illegal input on token
            //REQUIRE(result.error_description());
        }

        TEST_CASE("SchematikaParser-interactive-float", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    3.14159265 ;
             *
             **/

            {
                auto & result = parser.on_token(Token::f64_token("3.14159265"));

                log && log("after float token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::semicolon_token());

                log && log("after semicolon token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == false);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_expression());
                REQUIRE(result.result_expr());

                auto expr = obj<AExpression,DConstant>::from(result.result_expr());
                REQUIRE(expr);

                REQUIRE(expr->value());

                auto value_f64 = obj<AGCObject,DFloat>::from(expr->value());

                REQUIRE(value_f64);

                REQUIRE(value_f64->value() == 3.14159265);
            }

            //REQUIRE(result.is_error());
            //// illegal input on token
            //REQUIRE(result.error_description());
        }

        TEST_CASE("SchematikaParser-interactive-string", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    "hello world" ;
             *
             **/

            {
                auto & result = parser.on_token(Token::string_token("hello world"));

                log && log("after string token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::semicolon_token());

                log && log("after semicolon token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == false);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_expression());
                REQUIRE(result.result_expr());

                auto expr = obj<AExpression,DConstant>::from(result.result_expr());
                REQUIRE(expr);

                REQUIRE(expr->value());

                auto value_str = obj<AGCObject,DString>::from(expr->value());

                REQUIRE(value_str);

                REQUIRE(strcmp(value_str->chars(), "hello world") == 0);
            }

            //REQUIRE(result.is_error());
            //// illegal input on token
            //REQUIRE(result.error_description());
        }

        TEST_CASE("SchematikaParser-interactive-arith", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    3.14159265 * 0.5 ;
             *
             **/

            std::vector<Token> tk_v{
                Token::f64_token("3.14159265"),
                Token::star_token(),
                Token::f64_token("0.5"),
                Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            const auto & result = parser.result();
            {
                auto expr = obj<AExpression,DApplyExpr>::from(result.result_expr());
                REQUIRE(expr);

                REQUIRE(expr->n_args() == 2);

                auto fn = obj<AExpression,DConstant>::from(expr->fn());
                REQUIRE(fn);

                auto pm = obj<AGCObject,DPrimitive_gco_2_gco_gco>::from(fn->value());
                REQUIRE(pm);
                REQUIRE(pm->name() == "_mul");

                auto lhs = obj<AExpression,DConstant>::from(expr->arg(0));
                REQUIRE(lhs);

                auto lhs_f64 = obj<AGCObject,DFloat>::from(lhs->value());
                REQUIRE(lhs_f64);
                REQUIRE(lhs_f64->value() == 3.14159265);

                auto rhs = obj<AExpression,DConstant>::from(expr->arg(1));
                REQUIRE(rhs);

                auto rhs_f64 = obj<AGCObject,DFloat>::from(rhs->value());
                REQUIRE(rhs_f64);
                REQUIRE(rhs_f64->value() == 0.5);
            }
        }

        TEST_CASE("SchematikaParser-interactive-cmp", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    312 == 312 ;
             *
             **/

            std::vector<Token> tk_v{
                Token::i64_token("312"),
                Token::cmpeq_token(),
                Token::i64_token("312"),
                Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            const auto & result = parser.result();
            {
                auto expr = obj<AExpression,DApplyExpr>::from(result.result_expr());
                REQUIRE(expr);

                REQUIRE(expr->n_args() == 2);

                auto fn = obj<AExpression,DConstant>::from(expr->fn());
                REQUIRE(fn);

                auto pm = obj<AGCObject,DPrimitive_gco_2_gco_gco>::from(fn->value());
                REQUIRE(pm);
                REQUIRE(pm->name() == "_equal");

                auto lhs = obj<AExpression,DConstant>::from(expr->arg(0));
                REQUIRE(lhs);

                auto lhs_i64 = obj<AGCObject,DInteger>::from(lhs->value());
                REQUIRE(lhs_i64);
                REQUIRE(lhs_i64->value() == 312);

                auto rhs = obj<AExpression,DConstant>::from(expr->arg(1));
                REQUIRE(rhs);

                auto rhs_i64 = obj<AGCObject,DInteger>::from(rhs->value());
                REQUIRE(rhs_i64);
                REQUIRE(rhs_i64->value() == 312);
            }
        }

        TEST_CASE("SchematikaParser-interactive-lambda", "[reader2][SchematikaParser]")
        {
            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    lambda (n : i64, r : i64) -> i64 { 123 }
             *
             **/

            {
                auto & result = parser.on_token(Token::lambda_token());

                log && log("after lambda token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::leftparen_token());

                log && log("after lparen token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::symbol_token("n"));

                log && log("after symbol(n) token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::colon_token());

                log && log("after colon token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::symbol_token("i64"));

                log && log("after symbol(i64) token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::comma_token());

                log && log("after comma token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::symbol_token("r"));

                log && log("after symbol(r) token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::colon_token());

                log && log("after colon token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::symbol_token("i64"));

                log && log("after symbol(i64) token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::rightparen_token());

                log && log("after rightparen token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::yields_token());

                log && log("after yields token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::symbol_token("i64"));

                log && log("after symbol(i64) token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::leftbrace_token());

                log && log("after leftbrace token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::i64_token("123"));

                log && log("after f64(123) token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::rightbrace_token());

                log && log("after rightbrace token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == false);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_expression());
                REQUIRE(result.result_expr());
            }

            //REQUIRE(result.is_error());
            //// illegal input on token
            //REQUIRE(result.error_description());
        }

        TEST_CASE("SchematikaParser-interactive-if", "[reader2][SchematikaParser]")
        {
            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    if true then 777 else "fooey" ;
             *
             **/

            std::vector<Token> tk_v{
                Token::if_token(),
                Token::bool_token("true"),
                Token::then_token(),
                Token::i64_token("777"),
                Token::else_token(),
                Token::string_token("fooey"),
                Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);
        }

        TEST_CASE("SchematikaParser-interactive-lambda2", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    lambda (x : i64) -> i64 { x * x }
             *    ^      ^^ ^  ^  ^  ^ ^  ^ ^ ^ ^ ^
             *    0      1| 3  4  5  6 7  8 9 a b c
             *            2
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::lambda_token(),
                /* [ 1] */   Token::leftparen_token(),
                /* [ 2] */     Token::symbol_token("x"),
                /* [ 3] */     Token::colon_token(),
                /* [ 4] */     Token::symbol_token("i64"),
                /* [ 5] */   Token::rightparen_token(),
                /* [ 6] */ Token::yields_token(),
                /* [ 7] */ Token::symbol_token("i64"),
                /* [ 8] */ Token::leftbrace_token(),
                /* [ 9] */   Token::symbol_token("x"),
                /* [ a] */   Token::star_token(),
                /* [ b] */   Token::symbol_token("x"),
                /* [ c] */ Token::rightbrace_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);
        }

        TEST_CASE("SchematikaParser-interactive-apply", "[reader2][SchematikaParser]")
        {
            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    (lambda (x : i64) { x * x })(13);
             *
             **/

            std::vector<Token> tk_v{
                Token::leftparen_token(),

                /**/ Token::lambda_token(),
                /**/   Token::leftparen_token(),
                /**/     Token::symbol_token("x"),
                /**/     Token::colon_token(),
                /**/     Token::symbol_token("i64"),
                /**/   Token::rightparen_token(),
                /**/   Token::leftbrace_token(),
                /**/     Token::symbol_token("x"),
                /**/     Token::star_token(),
                /**/     Token::symbol_token("x"),
                /**/   Token::rightbrace_token(),
                /**/ Token::rightparen_token(),
                /**/ Token::leftparen_token(),
                /**/   Token::i64_token("13"),
                /**/ Token::rightparen_token(),

                /**/ Token::semicolon_token(),
            };

            size_t i_tk = 0;
            size_t n_tk = tk_v.size();
            for (const auto & tk : tk_v) {
                INFO(tostr(xtag("i_tk", i_tk), xtag("tk", tk)));

                auto & result = parser.on_token(tk);

                log && log("after token", xtag("i_tk", i_tk), xtag("tk", tk));
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                if (i_tk + 1 < n_tk) {
                    REQUIRE(parser.has_incomplete_expr() == true);
                    REQUIRE(!result.is_error());
                    REQUIRE(result.is_incomplete());
                } else {
                    /* last token in tk_v[] */

                    REQUIRE(parser.has_incomplete_expr() == false);
                    REQUIRE(!result.is_error());
                    REQUIRE(result.is_expression());
                    REQUIRE(result.result_expr());
                }

                ++i_tk;
            }
        }

        TEST_CASE("SchematikaParser-interactive-apply2", "[reader2][SchematikaParser]")
        {
            // top-level apply, with multiple arguments

            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    (lambda (x : i64, y : i64) { x * y })(13, 15);
             *
             **/

            std::vector<Token> tk_v{
                Token::leftparen_token(),

                /**/ Token::lambda_token(),
                /*  */ Token::leftparen_token(),
                /*    */ Token::symbol_token("x"),
                /*    */ Token::colon_token(),
                /*    */ Token::symbol_token("i64"),
                /*  */ Token::comma_token(),
                /*    */ Token::symbol_token("y"),
                /*    */ Token::colon_token(),
                /*    */ Token::symbol_token("i64"),
                /*  */ Token::rightparen_token(),

                /**/ Token::leftbrace_token(),
                /*  */ Token::symbol_token("x"),
                /*  */ Token::star_token(),
                /*  */ Token::symbol_token("y"),
                /**/ Token::rightbrace_token(),

                Token::rightparen_token(),

                Token::leftparen_token(),
                /**/ Token::i64_token("13"),
                /**/ Token::comma_token(),
                /**/ Token::i64_token("15"),
                Token::rightparen_token(),

                Token::semicolon_token(),
            };

            size_t i_tk = 0;
            size_t n_tk = tk_v.size();
            for (const auto & tk : tk_v) {
                INFO(tostr(xtag("i_tk", i_tk), xtag("tk", tk)));

                auto & result = parser.on_token(tk);

                log && log("after token", xtag("i_tk", i_tk), xtag("tk", tk));
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                if (i_tk + 1 < n_tk) {
                    REQUIRE(parser.has_incomplete_expr() == true);
                    REQUIRE(!result.is_error());
                    REQUIRE(result.is_incomplete());
                } else {
                    /* last token in tk_v[] */

                    REQUIRE(parser.has_incomplete_expr() == false);
                    REQUIRE(!result.is_error());
                    REQUIRE(result.is_expression());
                    REQUIRE(result.result_expr());
                }

                ++i_tk;
            }

        }
    } /*namespace ut*/
} /*namespace xo*/

/* end SchematikaParser.test.cpp */
