/** @file SchematikaReader.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include "ReaderConfig.hpp"
#include "SchematikaParser.hpp"
#include <xo/tokenizer2/Tokenizer.hpp>

namespace xo {
    namespace scm {
        struct ReaderResult {
            using span_type = xo::mm::span<const char>;

            bool is_tk_error() const { return tk_error_.is_error(); }

            /** schematika expression parsed from input **/
            obj<AExpression> expr_;
            /** input span up to end of expression.
             *  only relevant when result type is expression.
             *  (otherwise treat entire input as consumed)
             **/
            span_type consumed_;

            /** {src_function, error_description, input_state, error_pos} **/
            TokenizerError tk_error_;
        };

        /** @class SchematikaReader
         *  @brief Pipeline comprising Schematika tokenizer and parser
         *
         *  Consumes text; produces expressions
         **/
        class SchematikaReader {
        public:
            using AAllocator = xo::mm::AAllocator;

        public:
            SchematikaReader(const ReaderConfig & config,
                             obj<AAllocator> expr_alloc);

            /** prepare interactive session
             *  (allows rvalue expressions at toplevel)
             **/
            void begin_interactive_session();

            /** consume input @p input_cstr **/
            const ReaderResult & read_expr(const char * input_cstr, bool eof);

        private:
            /** tokenizer converts a stream of chars
             *  to a stream of lexical tokens
             **/
            Tokenizer tokenizer_;

            /** parser converts a stream of tokens
             *  to a stream of expressions
             **/
            SchematikaParser parser_;

            /** current output from reader **/
            ReaderResult result_;

        };
    } /*namespace scm*/
} /*namespace xo*/

/* end SchematikaReader.hpp */
