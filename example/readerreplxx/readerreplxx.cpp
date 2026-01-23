/** @file readerreplxx.cpp **/

#include <xo/reader2/SchematikaReader.hpp>
#include <xo/gc/DX1Collector.hpp>
#include <xo/gc/detail/IAllocator_DX1Collector.hpp>
#include <xo/alloc2/Allocator.hpp>
//#include <xo/facet/facet.hpp>
#include <xo/facet/obj.hpp>
#include <replxx.hxx>
#include <iostream>
#include <unistd.h> // for isatty

// presumeably replxx assumes input is a tty
//
bool replxx_getline(bool interactive,
                    bool is_at_toplevel,
                    replxx::Replxx & rx,
                    const char ** p_input)
{
    using namespace std;

    char const * prompt = "";

    if (interactive) {
        prompt = ((is_at_toplevel) ? "> " : ". ");
    }

    const char * input_cstr = rx.input(prompt);

    bool retval = (input_cstr != nullptr);

    if (retval)
        *p_input = input_cstr;

    rx.history_add(input_cstr);

    return retval;
}

void
welcome(std::ostream & os)
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

namespace {
    using xo::scm::SchematikaReader;
    using xo::print::ppstate_standalone;
    using xo::print::ppconfig;
    using std::cout;
    using std::endl;

    /** body of read-parse-print loop
     *
     *  true -> no errors;
     *  false -> reader encountered error
     **/
    bool
    reader_seq(SchematikaReader * p_reader,
               SchematikaReader::span_type * p_input,
               bool eof)
    {
        auto [expr, remaining, error] = p_reader->read_expr(*p_input, eof);

        if (expr) {
            ppconfig ppc;
            ppstate_standalone pps(&cout, 0, &ppc);

            pps.prettyn(expr);

            *p_input = remaining;

            return true;
        } else if (error.is_error()) {
            cout << "parsing error (detected in " << error.src_function() << "): " << endl;
            error.report(cout);

            /* discard stashed remainder of input line
             * (for nicely-formatted errors)
             */
            p_reader->reset_to_idle_toplevel();

            return false;
        } else {
            /* partial expression or whitespace input, no error */
            return true;
        }
    }
}

int
main()
{
    using namespace replxx;

    using xo::scm::SchematikaReader;
    using xo::scm::ReaderConfig;
    using xo::mm::AAllocator;
    using xo::mm::DX1Collector;
    using xo::mm::CollectorConfig;
    using xo::mm::DArena;
    //using xo::print::ppconfig;
    //using xo::print::ppstate_standalone;
    using xo::facet::with_facet;
    using xo::facet::obj;
    using xo::scope;
    using namespace std;

    bool interactive = isatty(STDIN_FILENO);

    Replxx rx;
    rx.set_max_history_size(1000);
    rx.history_load("repl_history.txt");
//    rx.bind_key_internal(Replxx::KEY::control('p'), "history_previous");
//    rx.bind_key_internal(Replxx::KEY::control('n'), "history_next");

    constexpr bool c_debug_flag = false;
    scope log(XO_DEBUG(c_debug_flag));

    CollectorConfig x1_config = (CollectorConfig()
                                 .with_size(4*1024*1024));
    DX1Collector x1(x1_config);
    obj<AAllocator> expr_alloc = with_facet<AAllocator>::mkobj(&x1);

    // accepting defaults too
    ReaderConfig rdr_config = ReaderConfig();

    SchematikaReader rdr(rdr_config, expr_alloc);
    using span_type = SchematikaReader::span_type;

    welcome(cerr);

    rdr.begin_interactive_session();

    bool eof = false;
    const char * input_str;
    span_type input;

    while (replxx_getline(interactive, rdr.is_at_toplevel(), rx, &input_str)) {
        input = span_type::from_cstr(input_str);

        while (!input.empty() && reader_seq(&rdr, &input, false /*eof*/)) {
            ;
        }

        /* here: either:
         * 1. input.empty() or
         * 2. error encountered
         */
    }

    /* reminder: eof can complete at most one token */
    reader_seq(&rdr, &input, true /*eof*/);

    rx.history_save("repl_history.txt");
}

/* end readerreplxx.cpp */
