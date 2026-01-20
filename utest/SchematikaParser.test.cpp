/** @file SchematikaParser.test.cpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/reader2/SchematikaParser.hpp>
#include <xo/alloc2/arena/IAllocator_DArena.hpp>
#include <catch2/catch.hpp>

namespace xo {
    using xo::scm::SchematikaParser;
    using xo::scm::ParserResult;
    using xo::scm::parser_result_type;
    using xo::scm::Token;
    using xo::mm::ArenaConfig;
    using xo::mm::AAllocator;
    using xo::mm::DArena;
    using xo::facet::with_facet;

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
            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_batch_session();

            {
                auto & result = parser.on_token(Token::def_token());

                // after begin_interactive_session, parser has toplevel exprseq
                // but is still "at toplevel" in the sense of ready for input
                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(result.is_incomplete());
            }

            {
                auto & result = parser.on_token(Token::symbol_token("foo"));

                REQUIRE(parser.has_incomplete_expr() == true);
                REQUIRE(result.is_incomplete());
            }

            // define-expressions not properly implemented


            //REQUIRE(result.error_description());
        }

        TEST_CASE("SchematikaParser-interactive-if", "[reader2][SchematikaParser]")
        {
            ArenaConfig config;
            config.name_ = "test-arena";
            config.size_ = 16 * 1024;

            DArena expr_arena = DArena::map(config);
            obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);

            SchematikaParser parser(config, 4096, expr_alloc, false /*debug_flag*/);

            parser.begin_interactive_session();

            auto & result = parser.on_token(Token::if_token());

            // after begin_interactive_session, parser has toplevel exprseq
            // but is still "at toplevel" in the sense of ready for input
            REQUIRE(parser.has_incomplete_expr() == false);

            REQUIRE(result.is_error());

            // illegal input on token
            REQUIRE(result.error_description());
        }

    } /*namespace ut*/
} /*namespace xo*/

/* end SchematikaParser.test.cpp */
