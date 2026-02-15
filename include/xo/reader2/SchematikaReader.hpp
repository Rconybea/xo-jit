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
            /** unconsumed portion of input span
             *  only relevant when result type is expression.
             *  (otherwise input consumed)
             **/
            span_type remaining_input_;

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
            using MemorySizeVisitor = xo::mm::MemorySizeVisitor;
            using span_type = xo::mm::span<const char>;
            using size_type = std::size_t;

        public:
            /**
             *  @p expr_alloc.  allocator for Schematika expressions
             *  @p aux_alloc.   allocator for miscellaneous objects
             *                  (e.g. DArenaHashMap for global symtab)
             *  that have lifetime bounded by Schematika reader itself.
             **/
            SchematikaReader(const ReaderConfig & config,
                             obj<AAllocator> expr_alloc,
                             obj<AAllocator> fixed_alloc);

            /** visit reader-owned memory pools; call visitor(info) for each.
             *  Specifically exclude expr_alloc, since we don't consider
             *  that reader-owned
             **/
            void visit_pools(const MemorySizeVisitor & visitor) const;

            /** true iff parser is at top-level.
             *  false iff parser is working on incomplete expression
             **/
            bool is_at_toplevel() const noexcept;

            /** prepare interactive session
             *  (allows rvalue expressions at toplevel)
             **/
            void begin_interactive_session();

            /** prepare batch session
             *  (limits expression types at toplevel)
             **/
            void begin_batch_session();

            /** consume input @p input_cstr **/
            const ReaderResult & read_expr(span_type input_span, bool eof);

            /** reset @ref result_ to nominal value **/
            void reset_result();

            /** reset to known starting point after encountering an error.
             *  - remainder of stashed current line.
             *    Necesary for well-formatted error reporting.
             *  - current parsing state
             **/
            void reset_to_idle_toplevel();

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

            /** true to enable reader debug logging **/
            bool debug_flag_ = false;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end SchematikaReader.hpp */
