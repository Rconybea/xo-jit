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

            {
                auto & result = parser.on_token(Token::def_token());

                // after begin_interactive_session, parser has toplevel exprseq
                // but is still "at toplevel" in the sense of ready for input
                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(result.is_incomplete());

                log && log("after def token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));
            }

            {
                auto & result = parser.on_token(Token::symbol_token("foo"));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(result.is_incomplete());

                log && log("after lhs symbol token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));
            }

            {
                auto & result = parser.on_token(Token::colon_token());

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(result.is_incomplete());

                log && log("after colon token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));
            }

            {
                auto & result = parser.on_token(Token::symbol_token("f64"));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(result.is_incomplete());

                log && log("after typename symbol token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));
            }

            {
                auto & result = parser.on_token(Token::singleassign_token());

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(result.is_incomplete());

                log && log("after typename symbol token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                auto exp_ssm
                    = obj<ASyntaxStateMachine,DExpectExprSsm>::from(parser.top_ssm());

                REQUIRE(exp_ssm);
                REQUIRE(exp_ssm.data()->ssm_type() == syntaxstatetype::expect_rhs_expression);
                REQUIRE(exp_ssm.data()->allow_defs() == false);
                REQUIRE(exp_ssm.data()->cxl_on_rightbrace() == false);
            }

            {
                // future-proofing for Token only holding a string_view
                const DString * str = DString::from_cstr(expr_alloc, "3.141593");

                auto & result = parser.on_token(Token::f64_token(std::string(*str)));

                REQUIRE(parser.has_incomplete_expr() == true);

                log && log("after f64 token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));
            }

            {
                auto & result = parser.on_token(Token::semicolon_token());

                log && log("after semicolon token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == false);

                auto expr = obj<AExpression,DDefineExpr>::from(result.result_expr());
                REQUIRE(expr);
            }

            // define-expressions not properly implemented

            //REQUIRE(result.error_description());
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

            {
                auto & result = parser.on_token(Token::f64_token("3.14159265"));

                log && log("after float(3.14159265) token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::star_token());

                log && log("after star token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::f64_token("0.5"));

                log && log("after float(0.5) token:");
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

            //REQUIRE(result.is_error());
            //// illegal input on token
            //REQUIRE(result.error_description());
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

            {
                auto & result = parser.on_token(Token::if_token());

                log && log("after if token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::bool_token("true"));

                log && log("after true token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::then_token());

                log && log("after then token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::i64_token("777"));

                log && log("after i64 token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::else_token());

                log && log("after else token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::string_token("fooey"));

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
                REQUIRE(!result.is_incomplete());
            }

            //REQUIRE(result.is_error());
            //// illegal input on token
            //REQUIRE(result.error_description());
        }

        TEST_CASE("SchematikaParser-interactive-lambda2", "[reader2][SchematikaParser]")
        {
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
             *    lambda (x : i64) -> i64 { x * x };
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
                auto & result = parser.on_token(Token::symbol_token("x"));

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
                auto & result = parser.on_token(Token::symbol_token("x"));

                log && log("after symbol(x) token:");
                log && log(xtag("parser", &parser));
                log && log(xtag("result", result));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(!result.is_error());
                REQUIRE(result.is_incomplete());
            }

#ifdef NOPE
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
#endif
            REQUIRE(false);
        }
    } /*namespace ut*/
} /*namespace xo*/

/* end SchematikaParser.test.cpp */
