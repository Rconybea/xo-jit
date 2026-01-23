/** @file tokenrepl.cpp **/

#include <xo/tokenizer2/Tokenizer.hpp>
#include <xo/tokenizer2/Token.hpp>
#include <xo/tokenizer2/tokentype.hpp>
#include <xo/arena/span.hpp>
#include <xo/indentlog/log_config.hpp>
#include <replxx.hxx>
#include <iostream>
#include <unistd.h> // for isatty

// presumeably replxx assumes input is a tty
//
bool replxx_getline(bool interactive,
                    std::size_t parser_stack_size,
                    replxx::Replxx & rx,
                    const char ** p_input)
{
    using namespace std;

    char const * prompt = "";

    if (interactive) {
        if (parser_stack_size <= 1)
            prompt = "> ";
        else
            prompt = ". ";
    }

    const char * input_cstr = rx.input(prompt);

    bool retval = (input_cstr != nullptr);

    if (retval) {
        //cerr << "got reval->true" << endl;

        *p_input = input_cstr;

    } else {
        //cerr << "got retval->false" << endl;
    }

    rx.history_add(input_cstr);

    return retval;
}

int
main() {
    using xo::scm::Tokenizer;
    using xo::scm::operator<<;
    using xo::mm::CircularBufferConfig;
    using xo::mm::span;
    using xo::scope;
    using xo::xtag;
    using replxx::Replxx;

    using namespace std;

    using span_type = span<const char>;

    xo::log_config::min_log_level = xo::log_level::severe;

    bool interactive = isatty(STDIN_FILENO);

    Replxx rx;
    rx.set_max_history_size(1000);
    rx.history_load("repl_history.txt");

    constexpr bool c_debug_flag = true;
    scope log(XO_DEBUG(c_debug_flag));

    Tokenizer tkz(CircularBufferConfig{.name_ = "tokenrepl-input",
                                       .max_capacity_ = 4*1024,
                                       .max_captured_span_ = 128},
                  c_debug_flag);

    const char * input_cstr = nullptr;;

    size_t line_no = 1;

    constexpr std::size_t c_maxlines = 25;

    while (replxx_getline(interactive, 0 /*parser_stack_size*/, rx, &input_cstr))
    {
        //cout << "input: " << input << endl;

        auto input_ext = Tokenizer::span_type::from_cstr(input_cstr);

        // reminder: input may contain multiple tokens
        auto [error, input] = tkz.buffer_input_line(input_ext, false /*!eof*/);

        if (log) {
            log(xtag("msg", "buffered input line"));
            log(xtag("input", input));
        }

        while (!input.empty())
        {
            auto [tk, consumed, error] = tkz.scan(input);

            log && log(xtag("consumed", consumed), xtag("tk", tk));

            if (tk.is_valid()) {
                cout << tk << endl;
            } else if (error.is_error()) {
                cout << "tokenizer error: " << endl;

                error.report(cout);

                break;
            }

            input = input.after_prefix(consumed);
        }

        /* here: input.empty() or error encountered */

        ++line_no;

        if (line_no > c_maxlines) {
            cout << "always exit after " << c_maxlines << " lines of input" << endl;
            break;
        }
    }
}

/** end tokenrepl.cpp */
