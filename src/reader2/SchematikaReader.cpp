/** @file SchematikaReader.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "SchematikaReader.hpp"

namespace xo {
    namespace scm {
        SchematikaReader::SchematikaReader(const ReaderConfig & config,
                                           obj<AAllocator> expr_alloc)
                : tokenizer_{config.tk_buffer_config_,
                             config.tk_debug_flag_},
                  parser_{config.parser_arena_config_,
                          config.max_stringtable_cap_,
                          expr_alloc,
                          config.parser_debug_flag_},
                  debug_flag_{config.reader_debug_flag_}
        {
        }

        bool
        SchematikaReader::is_at_toplevel() const noexcept
        {
            return parser_.is_at_toplevel();
        }

        void
        SchematikaReader::begin_interactive_session()
        {
            parser_.begin_interactive_session();
        }

        // TODO:
        // Schematika::end_interactive_session()

        const ReaderResult &
        SchematikaReader::read_expr(span_type input_ext, bool eof)
        {
            scope log(XO_DEBUG(debug_flag_));

            if (log) {
                log(xtag("input_ext", input_ext));
                log(xtag("eof", eof));
            }

            if (!input_ext.empty()) {
                auto [error, input]
                    = tokenizer_.buffer_input_line(input_ext, eof);

                if (log) {
                    log(xtag("msg", "before loop: buffered input line"));
                    log(xtag("input", input));
                }

                while (!input.empty()) {
                    log && log(xtag("msg", "loop"),
                               xtag("input", input));

                    auto [tk, consumed, error] = tokenizer_.scan(input);

                    log && log(xtag("tk", tk), xtag("consumed", consumed));

                    auto rem_input = input.after_prefix(consumed);

                    log && log(xtag("rem_input", rem_input));

                    if (!tk.is_valid() && error.is_error()) {
                        this->result_
                            = ReaderResult
                            { .expr_ = obj<AExpression>(),
                              .remaining_input_ = rem_input,
                              .tk_error_ = std::move(error)
                            };

                        return result_;
                    }

                    // log && log(xtag("consumed", consumed), xtag("tk", tk));

                    if (tk.is_valid()) {
                        // presult {
                        //   result_type :: parser_result_type = none|expression|error
                        //   result_expr :: obj<AExpression>
                        //   error_src_function :: string_view
                        //   error_description :: const DString *
                        // }
                        //
                        const ParserResult & presult = parser_.on_token(tk);

                        if (presult.is_error()) {
                            // tk_error {
                            //   src_function :: const char *
                            //   error_description :: string
                            //   input_state {
                            //     current_line :: span
                            //     tk_start :: size_t
                            //     current_pos :: size_t
                            //     whitespace :: size_t
                            //    debug_flag :: bool
                            //   }
                            //   error_pos :: size_t
                            // }
                            //
                            // tk_error.report(cout);

                            this->result_
                                = ReaderResult
                                { .expr_     = obj<AExpression>(),
                                  .remaining_input_ = rem_input,
                                  .tk_error_ = std::move(error) };

                            assert(presult.error_description());

                            // carefully created error description, maybe
                            this->result_.tk_error_
                                = result_.tk_error_.with_error
                                (presult.error_src_fn_,
                                 std::string
                                 (std::string_view(*(presult.error_description()))));

                            return result_;
                        } else if (presult.is_expression()) {
                            this->result_
                                = ReaderResult
                                {
                                    .expr_     = presult.result_expr(),
                                    .remaining_input_ = rem_input,
                                    .tk_error_ = TokenizerError()
                                };

                            return result_;
                        }
                    }

                    input = rem_input;
                }
            }

            this->result_ = ReaderResult();

            return this->result_;
        }

        void
        SchematikaReader::reset_result()
        {
            this->parser_.reset_result();
        }

        void
        SchematikaReader::reset_to_idle_toplevel()
        {
            this->tokenizer_.discard_current_line();
            this->parser_.reset_to_idle_toplevel();
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end SchematikaReader.cpp */
