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
#include <xo/expression2/IfElseExpr.hpp>
#include <xo/expression2/VarRef.hpp>
#include <xo/expression2/Constant.hpp>
#include <xo/procedure2/Primitive_gco_2_gco_gco.hpp>
#include <xo/object2/List.hpp>
#include <xo/object2/Float.hpp>
#include <xo/object2/Integer.hpp>
#include <xo/stringtable2/String.hpp>
#include <xo/alloc2/arena/IAllocator_DArena.hpp>
#include <xo/facet/TypeRegistry.hpp>
#include <catch2/catch.hpp>

namespace xo {
    using xo::scm::ParserConfig;
    using xo::scm::SchematikaParser;
//    using xo::scm::ASyntaxStateMachine;
//    using xo::scm::syntaxstatetype;
//    using xo::scm::DDefineSsm;
//    using xo::scm::DExpectExprSsm;

    using xo::scm::AExpression;
    using xo::scm::DDefineExpr;
    using xo::scm::DIfElseExpr;
    using xo::scm::DApplyExpr;
    using xo::scm::DVarRef;
    using xo::scm::DConstant;

    //using xo::scm::ParserResult;
    using xo::scm::Token;
    using xo::mm::AGCObject;
    using xo::scm::DPrimitive_gco_2_gco_gco;
    using xo::scm::DList;
    using xo::scm::DString;
    using xo::scm::DFloat;
    using xo::scm::DInteger;

    using xo::facet::FacetRegistry;

    using xo::mm::ArenaConfig;
    using xo::mm::AAllocator;
    using xo::mm::DArena;
    using xo::mm::MemorySizeInfo;
//    using xo::facet::with_facet;

    static InitEvidence s_init = (InitSubsys<S_reader2_tag>::require());

    namespace ut {
        struct ParserFixture {
            ParserFixture(const std::string & testname, bool debug_flag)
            {
                this->aux_arena_
                    = std::move(DArena(ArenaConfig()
                                       .with_name(testname)
                                       .with_size(4 * 1024)
                                       .with_store_header_flag(true)));
                obj<AAllocator,DArena> aux_mm(&aux_arena_);

                this->expr_arena_
                    = dp<DArena>::make(aux_mm,
                                       (ArenaConfig()
                                        .with_name("expr")
                                        .with_size(16 * 1024)
                                        .with_store_header_flag(true)));
                obj<AAllocator,DArena> expr_mm(expr_arena_.data());

                ParserConfig cfg;
                cfg.parser_arena_config_.size_ = 16 * 1024;
                /* editor bait: symbol table */
                cfg.symtab_var_config_.hint_max_capacity_ = 128;
                cfg.symtab_types_config_.hint_max_capacity_ = 64;
                cfg.max_stringtable_capacity_ = 512;
                cfg.debug_flag_ = false;

                this->parser_
                    = dp<SchematikaParser>::make(aux_mm, cfg, expr_mm, aux_mm);
            }

            ParserFixture(const ParserFixture & other) = delete;
            ParserFixture(const ParserFixture && other) = delete;

            bool log_memory_layout(scope * p_log) {
                using xo::facet::TypeRegistry;
                using xo::mm::MemorySizeDetail;

                auto visitor = [p_log](const MemorySizeInfo & info) {
                    *p_log && (*p_log)(xtag("name",   info.resource_name_),
                                       xtag("used",   info.used_),
                                       xtag("alloc",  info.allocated_),
                                       xtag("commit", info.committed_),
                                       xtag("resv",   info.reserved_));
                    if (*p_log && info.detail_) {
                        (*p_log)("detail",
                                 xtag("n", (*info.detail_)[0].n_alloc_),
                                 xtag("z", (*info.detail_)[0].z_alloc_));
                        for (size_t i = 1; i < info.detail_->size(); ++i) {
                            const MemorySizeDetail & d = (*info.detail_)[i];

                            if (d.tseq_.is_sentinel())
                                break;

                            (*p_log)("[",i,"]",
                                     xtag("tseq",d.tseq_),
                                     xtag("type", TypeRegistry::id2name(d.tseq_)),
                                     xtag("n", d.n_alloc_),
                                     xtag("z", d.z_alloc_));
                        }
                    }
                };

                aux_arena_.visit_pools(visitor);
                FacetRegistry::instance().visit_pools(visitor);
                TypeRegistry::instance().visit_pools(visitor);
                expr_arena_->visit_pools(visitor);
                parser_->visit_pools(visitor);

                return true;
            }

            DArena aux_arena_;
            dp<DArena> expr_arena_;
            dp<SchematikaParser> parser_;
        };

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
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            REQUIRE(parser.debug_flag() == false);
            REQUIRE(parser.is_at_toplevel() == true);

            // baseline:
            //   SchematikaParser-ctor :used 1408
            //   facets-ctl            :used 73    // facet hashtable
            //   facets-slots          :used 1168  // facet hashtable
            //   expr                  :used 2056
            //   [1] :type xo::scm::DArray :n 1 :z 2056 // DArray of DUniqueString*
            //   [2] :type ? :n 1 : z 16
            //   strings               :used 0
            //   stringkeys-ctl        :used 0
            //   strinkeys-slots       :used 0
            //   parser-arena          :used 0
            //   global-symtab-ctl     :used 0
            //   global-symtab-slots   :used 0

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-begin-interactive",
                  "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            // after begin_interactive_session, parser has toplevel exprseq
            // but is still "at toplevel" in the sense of ready for input
            REQUIRE(parser.has_incomplete_expr() == false);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-begin-batch", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_batch_session();

            // after begin_translation_unit, parser has toplevel exprseq
            // but is still "at toplevel" in the sense of ready for input
            REQUIRE(parser.has_incomplete_expr() == false);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-batch-def", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

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

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-batch-deftype", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_batch_session();

            /**  Walkthrough parsing input equivalent to:
             *
             *     deftype foo :: f64;
             **/

            std::vector<Token> tk_v{
                Token::deftype_token(),
                Token::symbol_token("foo"),
                Token::doublecolon_token(),
                Token::symbol_token("f64"),
                Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            const auto & result = parser.result();
            {
                // placeholder for form's sake.
                // deftype doesn't actuallly produce any executable content

                auto expr = obj<AExpression,DConstant>::from(result.result_expr());
                REQUIRE(expr);
            }

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-batch-deftype-2", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_batch_session();

            /**  Walkthrough parsing input equivalent to:
             *
             *     deftype foo :: list<f64>;
             **/

            std::vector<Token> tk_v{
                Token::deftype_token(),
                Token::symbol_token("foo"),
                Token::doublecolon_token(),
                Token::symbol_token("list"),
                Token::leftangle_token(),
                Token::symbol_token("f64"),
                Token::rightangle_token(),
                Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            const auto & result = parser.result();
            {
                // placeholder for form's sake.
                // deftype doesn't actuallly produce any executable content

                auto expr = obj<AExpression,DConstant>::from(result.result_expr());
                REQUIRE(expr);
            }

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-def2", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            {
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

                parser.reset_result();
            }

            {
                /** Walkthrough parsing input equivalent to:
                 *
                 *    foo ;
                 *
                 **/

                std::vector<Token> tk_v{
//                    Token::f64_token("2.0"),
//                    Token::star_token(),
                    Token::symbol_token("foo"),
                    Token::semicolon_token(),
                };

                utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

                const auto & result = parser.result();
                {
                    auto expr = obj<AExpression,DVarRef>::from(result.result_expr());
                    REQUIRE(expr);
                }
            }

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-integer", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

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

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-float", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

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

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-string", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

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

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-nil", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    nil ;
             *
             **/

            {
                auto & result = parser.on_token(Token::nil_token());

                log && log("after nil token:");
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

                auto value_list = obj<AGCObject,DList>::from(expr->value());

                REQUIRE(value_list);

                REQUIRE(value_list->is_empty());
            }

            //REQUIRE(result.is_error());
            //// illegal input on token
            //REQUIRE(result.error_description());

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-arith", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

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

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-arith2", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    3.14159265 / 10.0 ;
             *
             **/

            std::vector<Token> tk_v{
                Token::f64_token("3.14159265"),
                Token::slash_token(),
                Token::f64_token("10.0"),
                Token::semicolon_token(),
            };

            INFO(testname);

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
                REQUIRE(pm->name() == "_div");

                auto lhs = obj<AExpression,DConstant>::from(expr->arg(0));
                REQUIRE(lhs);

                auto lhs_f64 = obj<AGCObject,DFloat>::from(lhs->value());
                REQUIRE(lhs_f64);
                REQUIRE(lhs_f64->value() == 3.14159265);

                auto rhs = obj<AExpression,DConstant>::from(expr->arg(1));
                REQUIRE(rhs);

                auto rhs_f64 = obj<AGCObject,DFloat>::from(rhs->value());
                REQUIRE(rhs_f64);
                REQUIRE(rhs_f64->value() == 10.0);
            }

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-arith3", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    7 * 2 - 3;
             *
             **/

            std::vector<Token> tk_v{
                Token::i64_token("7"),
                Token::star_token(),
                Token::i64_token("2"),
                Token::minus_token(),
                Token::i64_token("3"),
                Token::semicolon_token(),
            };

            INFO(testname);

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
                REQUIRE(pm->name() == "_sub");

                auto lhs = obj<AExpression,DApplyExpr>::from(expr->arg(0));
                REQUIRE(lhs);

                auto lhs_lhs = obj<AExpression,DConstant>::from(lhs->arg(0));
                REQUIRE(lhs_lhs);
                auto lhs_lhs_i64 = obj<AGCObject,DInteger>::from(lhs_lhs->value());
                REQUIRE(lhs_lhs_i64);
                REQUIRE(lhs_lhs_i64->value() == 7);

                auto lhs_rhs = obj<AExpression,DConstant>::from(lhs->arg(1));
                REQUIRE(lhs_rhs);
                auto lhs_rhs_i64 = obj<AGCObject,DInteger>::from(lhs_rhs->value());
                REQUIRE(lhs_rhs_i64);
                REQUIRE(lhs_rhs_i64->value() == 2);

                auto rhs = obj<AExpression,DConstant>::from(expr->arg(1));
                REQUIRE(rhs);

                auto rhs_i64 = obj<AGCObject,DInteger>::from(rhs->value());
                REQUIRE(rhs_i64);
                REQUIRE(rhs_i64->value() == 3);
            }

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-arith4", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    7 + 2 / 3;
             *
             **/

            std::vector<Token> tk_v{
                Token::i64_token("7"),
                Token::plus_token(),
                Token::i64_token("2"),
                Token::slash_token(),
                Token::i64_token("3"),
                Token::semicolon_token(),
            };

            INFO(testname);

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
                REQUIRE(pm->name() == "_add");

                auto lhs = obj<AExpression,DConstant>::from(expr->arg(0));
                REQUIRE(lhs);

                auto lhs_i64 = obj<AGCObject,DInteger>::from(lhs->value());
                REQUIRE(lhs_i64->value() == 7);

                auto rhs = obj<AExpression,DApplyExpr>::from(expr->arg(1));
                REQUIRE(rhs);

                auto rhs_lhs = obj<AExpression,DConstant>::from(rhs->arg(0));
                REQUIRE(rhs_lhs);
                auto rhs_lhs_i64 = obj<AGCObject,DInteger>::from(rhs_lhs->value());
                REQUIRE(rhs_lhs_i64);
                REQUIRE(rhs_lhs_i64->value() == 2);

                auto rhs_rhs = obj<AExpression,DConstant>::from(rhs->arg(1));
                REQUIRE(rhs_rhs);
                auto rhs_rhs_i64 = obj<AGCObject,DInteger>::from(rhs_rhs->value());
                REQUIRE(rhs_rhs_i64);
                REQUIRE(rhs_rhs_i64->value() == 3);
            }

            log && fixture.log_memory_layout(&log);
        }

#ifdef OBSOLETE
        TEST_CASE("SchematikaParser-interactive-arith3-bad", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    7 * 2 - 3 4;
             *
             **/

            std::vector<Token> tk_v{
                Token::i64_token("7"),
                Token::star_token(),
                Token::i64_token("2"),
                Token::minus_token(),
                Token::i64_token("3"),
                Token::i64_token("4"),
                Token::semicolon_token(),
            };

            INFO(testname);

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
                REQUIRE(pm->name() == "_sub");

                auto lhs = obj<AExpression,DApplyExpr>::from(expr->arg(0));
                REQUIRE(lhs);

                auto lhs_lhs = obj<AExpression,DConstant>::from(lhs->arg(0));
                REQUIRE(lhs_lhs);
                auto lhs_lhs_i64 = obj<AGCObject,DInteger>::from(lhs_lhs->value());
                REQUIRE(lhs_lhs_i64);
                REQUIRE(lhs_lhs_i64->value() == 7);

                auto lhs_rhs = obj<AExpression,DConstant>::from(lhs->arg(1));
                REQUIRE(lhs_rhs);
                auto lhs_rhs_i64 = obj<AGCObject,DInteger>::from(lhs_rhs->value());
                REQUIRE(lhs_rhs_i64);
                REQUIRE(lhs_rhs_i64->value() == 2);

                auto rhs = obj<AExpression,DConstant>::from(expr->arg(1));
                REQUIRE(rhs);

                auto rhs_i64 = obj<AGCObject,DInteger>::from(rhs->value());
                REQUIRE(rhs_i64);
                REQUIRE(rhs_i64->value() == 3);
            }

            log && fixture.log_memory_layout(&log);
        }
#endif

        TEST_CASE("SchematikaParser-interactive-cmp", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag),
                      xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    312 == 312 ;
             *    ^   ^  ^   ^
             *    0   1  2   3
             **/

            std::vector<Token> tk_v{
                /* [0] */ Token::i64_token("312"),
                /* [1] */ Token::cmpeq_token(),
                /* [2] */ Token::i64_token("312"),
                /* [3] */ Token::semicolon_token(),
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
                REQUIRE(pm->name() == "_cmpeq");

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

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-if1", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag),
                      xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            {
                /** Walkthrough parsing input equivalent to:
                 *
                 *    def n = 4 ;
                 *    ^   ^ ^ ^ ^
                 *    0   1 2 3 4
                 **/

                std::vector<Token> tk_v{
                    /* [0] */ Token::def_token(),
                    /* [1] */ Token::symbol_token("n"),
                    /* [2] */ Token::singleassign_token(),
                    /* [3] */ Token::i64_token("4"),
                    /* [4] */ Token::semicolon_token()
                };

                utest_tokenizer_loop(&parser, tk_v, c_debug_flag);
            }

            {
                const auto & result = parser.result();

                auto expr = obj<AExpression,DDefineExpr>::from(result.result_expr());
                REQUIRE(expr);
            }

            {
                parser.reset_result();

                /** Walkthrough parsing input equivalent to:
                 *
                 *    if (n == 4) then 1 else n * 5 ;
                 *    ^  ^^ ^  ^^ ^    ^ ^    ^ ^ ^ ^
                 *    0  1| 3  4| 6    7 8    9 a b c
                 *        2     5
                 **/

                std::vector<Token> tk_v{
                    /* [0] */ Token::if_token(),
                    /* [1] */   Token::leftparen_token(),
                    /* [2] */     Token::symbol_token("n"),
                    /* [3] */     Token::cmpeq_token(),
                    /* [4] */     Token::i64_token("4"),
                    /* [5] */   Token::rightparen_token(),
                    /* [6] */ Token::then_token(),
                    /* [7] */   Token::i64_token("1"),
                    /* [8] */ Token::else_token(),
                    /* [9] */   Token::symbol_token("n"),
                    /* [a] */   Token::star_token(),
                    /* [b] */   Token::i64_token("5"),
                    /* [c] */ Token::semicolon_token()
                };

                utest_tokenizer_loop(&parser, tk_v, c_debug_flag);
            }

            const auto & result = parser.result();
            {
                auto expr = obj<AExpression,DIfElseExpr>::from(result.result_expr());
                REQUIRE(expr);
            }

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-lambda", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    lambda (n : i64, r : i64) -> i64 { 123 }
             *    ^      ^^ ^ ^  ^ ^ ^ ^  ^ ^  ^   ^ ^   ^
             *    0      1| 3 4  5 6 7 8  9 a  b   c d   e
             *            2
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::lambda_token(),
                /* [ 1] */   Token::leftparen_token(),
                /* [ 2] */     Token::symbol_token("n"),
                /* [ 3] */     Token::colon_token(),
                /* [ 4] */     Token::symbol_token("i64"),
                /* [ 5] */   Token::comma_token(),
                /* [ 6] */     Token::symbol_token("r"),
                /* [ 7] */     Token::colon_token(),
                /* [ 8] */     Token::symbol_token("i64"),
                /* [ 9] */   Token::rightparen_token(),
                /* [ a] */ Token::yields_token(),
                /* [ b] */ Token::symbol_token("i64"),
                /* [ c] */ Token::leftbrace_token(),
                /* [ d] */   Token::i64_token("123"),
                /* [ e] */ Token::rightbrace_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-if2", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    if true then 777 else "fooey" ;
             *    ^  ^    ^    ^   ^    ^       ^
             *    0  1    2    3   4    5       6
             **/

            std::vector<Token> tk_v{
                /* [0] */ Token::if_token(),
                /* [1] */   Token::bool_token("true"),
                /* [2] */ Token::then_token(),
                /* [3] */   Token::i64_token("777"),
                /* [4] */ Token::else_token(),
                /* [5] */   Token::string_token("fooey"),
                /* [6] */ Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-lambda2", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

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

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-apply", "[reader2][SchematikaParser]")
        {
            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    (lambda (x : i64) { x * x })(13) ;
             *    ^^      ^^ ^  ^ ^ ^ ^ ^ ^ ^^^^ ^ ^
             *    0|      2| 4  5 6 7 8 9 a b|d| f g
             *     1       3                 c e
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::leftparen_token(),

                /* [ 1] */   Token::lambda_token(),
                /* [ 2] */     Token::leftparen_token(),
                /* [ 3] */       Token::symbol_token("x"),
                /* [ 4] */       Token::colon_token(),
                /* [ 5] */       Token::symbol_token("i64"),
                /* [ 6] */     Token::rightparen_token(),
                /* [ 7] */     Token::leftbrace_token(),
                /* [ 8] */       Token::symbol_token("x"),
                /* [ 9] */       Token::star_token(),
                /* [ a] */       Token::symbol_token("x"),
                /* [ b] */     Token::rightbrace_token(),
                /* [ c] */   Token::rightparen_token(),
                /* [ d] */   Token::leftparen_token(),
                /* [ e] */     Token::i64_token("13"),
                /* [ f] */   Token::rightparen_token(),
                /* [ g] */ Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-interactive-apply2", "[reader2][SchematikaParser]")
        {
            // top-level apply, with multiple arguments

            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    (lambda (x : i64, y : i64) { x * y })(13, 15) ;
             *    ^^      ^^ ^  ^  ^ ^^ ^  ^ ^ ^ ^ ^ ^^^^ ^ ^ ^ ^
             *    0|      2| 4  5  6 7| 9  a b c d e f|h| j k l m
             *     1       3          8               g i
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::leftparen_token(),

                /* [ 1] */   Token::lambda_token(),
                /* [ 2] */     Token::leftparen_token(),
                /* [ 3] */       Token::symbol_token("x"),
                /* [ 4] */       Token::colon_token(),
                /* [ 5] */       Token::symbol_token("i64"),
                /* [ 6] */     Token::comma_token(),
                /* [ 7] */       Token::symbol_token("y"),
                /* [ 8] */       Token::colon_token(),
                /* [ 9] */       Token::symbol_token("i64"),
                /* [ a] */     Token::rightparen_token(),

                /* [ b] */     Token::leftbrace_token(),
                /* [ c] */       Token::symbol_token("x"),
                /* [ d] */       Token::star_token(),
                /* [ e] */       Token::symbol_token("y"),
                /* [ f] */     Token::rightbrace_token(),

                /* [ g] */   Token::rightparen_token(),

                /* [ h] */   Token::leftparen_token(),
                /* [ i] */     Token::i64_token("13"),
                /* [ j] */     Token::comma_token(),
                /* [ k] */     Token::i64_token("15"),
                /* [ l] */   Token::rightparen_token(),

                /* [ m] */ Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-batch-def2", "[reader2][SchematikaParser]")
        {
            // top-level recursive function definition

            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    def fact = lambda (n) { if (n == 0) then 1 else n * fact(n - 1) };
             *    ^   ^    ^ ^      ^^^ ^ ^  ^^ ^  ^^ ^    ^ ^    ^ ^ ^   ^^ ^ ^^ ^^
             *    0   1    2 3      4|6 7 8  9| b  c| e    f g    h i j   k| m n| p|
             *                       5        a     d                      l    o  q
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::def_token(),

                /* [ 1] */ Token::symbol_token("fact"),
                /* [ 2] */ Token::singleassign_token(),
                /* [ 3] */ Token::lambda_token(),
                /* [ 4] */   Token::leftparen_token(),
                /* [ 5] */       Token::symbol_token("n"),
                /* [ 6] */   Token::rightparen_token(),
                /* [ 7] */ Token::leftbrace_token(),
                /* [ 8] */   Token::if_token(),
                /* [ 9] */     Token::leftparen_token(),
                /* [ a] */       Token::symbol_token("n"),
                /* [ b] */       Token::cmpeq_token(),
                /* [ c] */       Token::i64_token("0"),
                /* [ d] */     Token::rightparen_token(),
                /* [ e] */   Token::then_token(),
                /* [ f] */     Token::i64_token("1"),
                /* [ g] */   Token::else_token(),
                /* [ h] */     Token::symbol_token("n"),
                /* [ i] */     Token::star_token(),
                /* [ j] */     Token::symbol_token("fact"),
                /* [ k] */     Token::leftparen_token(),
                /* [ l] */       Token::symbol_token("n"),
                /* [ m] */       Token::minus_token(),
                /* [ n] */       Token::i64_token("1"),
                /* [ o] */     Token::rightparen_token(),
                /* [ p] */     Token::rightbrace_token(),
                /* [ q] */     Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-batch-qliteral1", "[reader2][SchematikaParser]")
        {
            // top-level recursive function definition

            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    #q{ 4.5 } ;
             *    ^ ^   ^ ^ ^
             *    0 1   2 3 4
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::quote_token(),

                /* [ 1] */ Token::leftbrace_token(),
                /* [ 2] */   Token::f64_token("4.5"),
                /* [ 3] */ Token::rightbrace_token(),
                /* [ 4] */ Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-batch-qliteral2", "[reader2][SchematikaParser]")
        {
            // top-level recursive function definition

            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    #q{ 4.5 } + #q { 7.2 };
             *    ^ ^ ^   ^ ^ ^  ^ ^   ^^
             *    0 1 2   3 4 5  6 7   8|
             *                          9
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::quote_token(),
                /* [ 1] */ Token::leftbrace_token(),
                /* [ 2] */   Token::f64_token("4.5"),
                /* [ 3] */ Token::rightbrace_token(),

                /* [ 4] */ Token::plus_token(),

                /* [ 5] */ Token::quote_token(),
                /* [ 6] */ Token::leftbrace_token(),
                /* [ 7] */   Token::f64_token("7.2"),
                /* [ 8] */ Token::rightbrace_token(),

                /* [ 9] */ Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-batch-qlist", "[reader2][SchematikaParser]")
        {
            // top-level recursive function definition

            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    #q{ (4.5 7.2) };
             *    ^ ^ ^^   ^  ^ ^^
             *    0 1 2|   4  5 6|
             *         3         7
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::quote_token(),
                /* [ 1] */ Token::leftbrace_token(),
                /* [ 2] */   Token::leftparen_token(),
                /* [ 3] */     Token::f64_token("4.5"),
                /* [ 4] */     Token::f64_token("7.2"),
                /* [ 5] */   Token::rightparen_token(),
                /* [ 6] */ Token::rightbrace_token(),
                /* [ 7] */ Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-batch-qlist2", "[reader2][SchematikaParser]")
        {
            // top-level recursive function definition

            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    #q{ (4 7.2) };
             *    ^ ^ ^^ ^  ^ ^^
             *    0 1 2| 4  5 6|
             *         3       7
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::quote_token(),
                /* [ 1] */ Token::leftbrace_token(),
                /* [ 2] */   Token::leftparen_token(),
                /* [ 3] */     Token::i64_token("4"),
                /* [ 4] */     Token::f64_token("7.2"),
                /* [ 5] */   Token::rightparen_token(),
                /* [ 6] */ Token::rightbrace_token(),
                /* [ 7] */ Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

        TEST_CASE("SchematikaParser-batch-qarray", "[reader2][SchematikaParser]")
        {
            // top-level recursive function definition

            const auto & testname = Catch::getResultCapture().getCurrentTestName();

            constexpr bool c_debug_flag = false;
            scope log(XO_DEBUG(c_debug_flag), xtag("test", testname));

            ParserFixture fixture(testname, c_debug_flag);
            auto & parser = *(fixture.parser_);

            parser.begin_interactive_session();

            /** Walkthrough parsing input equivalent to:
             *
             *    #q{ [4.5 7.2] };
             *    ^ ^ ^^   ^  ^ ^^
             *    0 1 2|   4  5 6|
             *         3         7
             **/

            std::vector<Token> tk_v{
                /* [ 0] */ Token::quote_token(),
                /* [ 1] */ Token::leftbrace_token(),
                /* [ 2] */   Token::leftbracket_token(),
                /* [ 3] */     Token::f64_token("4.5"),
                /* [ 4] */     Token::f64_token("7.2"),
                /* [ 5] */   Token::rightbracket_token(),
                /* [ 6] */ Token::rightbrace_token(),
                /* [ 7] */ Token::semicolon_token(),
            };

            utest_tokenizer_loop(&parser, tk_v, c_debug_flag);

            log && fixture.log_memory_layout(&log);
        }

    } /*namespace ut*/
} /*namespace xo*/

/* end SchematikaParser.test.cpp */
