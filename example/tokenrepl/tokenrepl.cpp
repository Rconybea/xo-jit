/** @file tokenrepl.cpp **/

#include <xo/tokenizer2/Tokenizer.hpp>
#include <xo/tokenizer2/Token.hpp>
#include <xo/tokenizer2/tokentype.hpp>
#include <xo/tokenizer2/span.hpp>
#include <xo/indentlog/log_config.hpp>
#include <replxx.hxx>
#include <iostream>
#include <unistd.h> // for isatty

// presumeably replxx assumes input is a tty
//
bool replxx_getline(bool interactive,
                    std::size_t parser_stack_size,
                    replxx::Replxx & rx,
                    std::string& input)
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

        input = input_cstr;

    } else {
        //cerr << "got retval->false" << endl;
    }

    rx.history_add(input);

    // we want tokenizer to see newline, it's syntax
    input.push_back('\n');

    return retval;
}

#ifdef OBSOLETE
bool repl_getline(bool interactive,
                  std::istream & in,
                  std::ostream & out,
                  std::string & input)
{
    if (interactive) {
        out << "> ";
        std::flush(out);
    }

    return static_cast<bool>(std::getline(in, input));
}
#endif

int
main() {
    using xo::scm::Tokenizer;
    using xo::scm::span;
    using xo::scm::operator<<;
    using replxx::Replxx;

    using namespace std;

    using span_type = span<const char>;

    xo::log_config::min_log_level = xo::log_level::severe;

    bool interactive = isatty(STDIN_FILENO);

    Replxx rx;
    rx.set_max_history_size(1000);
    rx.history_load("repl_history.txt");

    Tokenizer tkz(xo::log_config::min_log_level <= xo::log_level::info);

    string input_str;

    size_t line_no = 1;

    constexpr std::size_t c_maxlines = 25;

    while (
        //repl_getline(interactive, cin, cout, input_str)  // once upon a time
        replxx_getline(interactive, 0 /*parser_stack_size*/, rx, input_str))
    {
        span_type input = span_type::from_string(input_str);

        //cout << "input: " << input << endl;

        // reminder: input may contain multiple tokens
        while (!input.empty()) {
            auto [tk, consumed, error] = tkz.scan(input, false /*!eof*/);

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
