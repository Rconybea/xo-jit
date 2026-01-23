/** @file SchematikaReader.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "SchematikaReader.hpp"

namespace xo {
    namespace scm {
        SchematikaReader::SchematikaReader(const ReaderConfig & config,
                                           obj<AAllocator> expr_alloc)
                : tokenizer_{config.tk_buffer_config_, config.tk_debug_flag_},
                  parser_{config.parser_arena_config_,
                          config.max_stringtable_cap_,
                          expr_alloc,
                          config.parser_debug_flag_}
        {
        }

        void
        SchematikaReader::begin_interactive_session()
        {
            parser_.begin_interactive_session();
        }

        // TODO:
        // Schematika::end_interactive_session()

        const ReaderResult &
        SchematikaReader::read_expr(const char * input_cstr, bool eof)
        {
            if (input_cstr && *input_cstr) {
                auto [error, input]
                    = tokenizer_.buffer_input_line(input_cstr,
                                                   false /*!eof*/);
                // log && log(xtag("msg", "buffered input line"));
                // log && log(xtag("input", input));



                while (!input.empty()) {
                    auto [tk, consumed, error] = tkz.scan(input);

                    if (!tk.is_valid() && error.is_error()) {
                        this->result_ = ReaderResult { .expr_ = obj<AExpression>(),
                                                       .tk_error_ = std::move(error),
                                                       .consumed_ = nullptr };
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
                        const ParserResult & presult = parser_include_token(tk);

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

                            this->result_ = ReaderResult { .expr = obj<AExpression>(),
                                                           .tk_error_ = std::move(error),
                                                           .consumed_ = nullptr };

                            // carefully created error description, maybe
                            this->result.tk_error_.error_description_ = presult.error_description_;

                        }

                        xxxx;
                    } else if (error.is_error()) {
                        xxxx;
                        // error.report(cout);
                        break;
                    }

                    input = input.after_prefix(consumed);
                }
            }

            ++line_no;
        }

    } /*namespace scm*/
} /*namespace xo*/

/* end SchematikaReader.cpp */
