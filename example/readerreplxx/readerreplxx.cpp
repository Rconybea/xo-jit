/** @file readerreplxx.cpp **/

#include "xo/reader/reader.hpp"
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

        input = input_cstr;

    } else {
        //cerr << "got retval->false" << endl;
    }

    rx.history_add(input);

    return retval;
}

void
welcome(std::ostream& os)
{
    using namespace std;

    os << "read-eval-print loop for schematika expressions" << endl;
    os << "  ctrl-a/ctrl-e   beginning/end of line" << endl;
    os << "  ctrl-u          delete entire line" << endl;
    os << "  ctrl-k          delete to end of line" << endl;
    os << "  meta-<bs>       backward delete word" << endl;
    os << "  <up>|meta-p     previous command from history" << endl;
    os << "  <down>|meta-n   next command from history" << endl;
    os << "  <pgup>/<pgdown> page through history faster" << endl;
    os << "  ctrl-s/ctrl-r   forward/backward history search" << endl;
    os << endl;
}

int
main()
{
    using namespace replxx;
    using namespace xo::scm;
    using xo::scm::Expression;
    using xo::print::ppconfig;
    using xo::print::ppstate_standalone;
    using xo::rp;
    using namespace std;

    using span_type = xo::scm::span<const char>;

    bool interactive = isatty(STDIN_FILENO);

    Replxx rx;
    rx.set_max_history_size(1000);
    rx.history_load("repl_history.txt");
//    rx.bind_key_internal(Replxx::KEY::control('p'), "history_previous");
//    rx.bind_key_internal(Replxx::KEY::control('n'), "history_next");

    constexpr bool c_debug_flag = false;
    scope log(XO_DEBUG(c_debug_flag));

    DArena expr_arena = DArena::map(ArenaConfig{ .name_ = "expr-arena", .size_ = 2*1024*1024; });
    obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&expr_arena);
    constexpr size_t c_max_stringtable_cap = 1024*1024;
    SchematikaParser parser(expr_arena.config_, c_max_stringtable_cap, expr_alloc, c_debug_flag);

    parser.begin_interactive_session();

    string input_str;

    bool eof = false;

    span_type input;
    std::size_t parser_stack_size = 0;

    welcome(cerr);

    while (replxx_getline(interactive, parser_stack_size, rx, input_str)) {
        input = span_type::from_string(input_str);

        while (!input.empty()) {
            auto [expr, consumed, psz, error] = rdr.read_expr(input, eof);

            if (expr) {
                ppconfig ppc;
                ppstate_standalone pps(&cout, 0, &ppc);

                pps.prettyn(expr);
            } else if (error.is_error()) {
                cout << "parsing error (detected in " << error.src_function() << "): " << endl;
                error.report(cout);

                /* discard stashed remainder of input line
                 * (for nicely-formatted errors)
                 */
                rdr.reset_to_idle_toplevel();
                break;
            }

            input = input.after_prefix(consumed);
            parser_stack_size = psz;
        }

        /* here: input.empty() or error encountered */

    }

    auto [expr, _1, _2, error] = rdr.read_expr(input, true /*eof*/);

    if (expr) {
        ppconfig ppc;
        ppstate_standalone pps(&cout, 0, &ppc);

        pps.prettyn<rp<Expression>>(rp<Expression>(expr));
    } else if (error.is_error()) {
        cout << "parsing error (detected in " << error.src_function() << "): " << endl;
        error.report(cout);
    }

    rx.history_save("repl_history.txt");
}

/* end readerreplxx.cpp */
