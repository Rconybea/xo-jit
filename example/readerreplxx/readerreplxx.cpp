/** @file readerreplxx.cpp **/

#include <xo/reader2/init_reader2.hpp>
#include <xo/reader2/SchematikaReader.hpp>
#include <xo/gc/DX1Collector.hpp>
#include <xo/gc/detail/IAllocator_DX1Collector.hpp>
#include <xo/alloc2/Allocator.hpp>
#include <xo/printable2/Printable.hpp>
#include <xo/facet/FacetRegistry.hpp>
#include <xo/facet/obj.hpp>
#include <xo/subsys/Subsystem.hpp>
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

    if (input_cstr)
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
    using xo::scm::AExpression;
    using xo::print::APrintable;
    using xo::print::ppstate_standalone;
    using xo::print::ppconfig;
    using xo::facet::FacetRegistry;
    using xo::facet::obj;
    using xo::xtag;
    using xo::scope;
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
               bool eof,
               bool debug_flag)
    {
        scope log(XO_DEBUG(debug_flag));

        if (!p_input || p_input->empty())
            return true;

        auto [expr, remaining, error] = p_reader->read_expr(*p_input, eof);

        obj<APrintable> expr_pr;

        if (expr) {
            expr_pr = FacetRegistry::instance().variant<APrintable,AExpression>(expr);
            assert(expr_pr);
        }

        if (log) {
            if (expr_pr) {
                log(xtag("expr", expr_pr));
            }
            log(xtag("remaining", remaining));
            log(xtag("error", error));
        }

        if (expr) {
            ppconfig ppc;
            ppstate_standalone pps(&cout, 0, &ppc);

            pps.prettyn(expr_pr);

            p_reader->reset_result();

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
            *p_input = remaining;

            /* partial expression or whitespace input, no error */
            return true;
        }
    }
}

struct AppConfig {
    using ReaderConfig = xo::scm::ReaderConfig;
    using CollectorConfig = xo::mm::CollectorConfig;

    AppConfig() {
        rdr_config_.reader_debug_flag_ = true;
        //rdr_config.parser_debug_flag_ = true;
        //rdr_config.tk_debug_flag_ = true;
    }

    std::size_t max_history_size_ = 1000;
    std::string repl_history_fname_ = "repl_history.txt";;
    CollectorConfig x1_config_ = (CollectorConfig().with_size(4*1024*1024));
    ReaderConfig rdr_config_;
};

struct AppContext {
    using DX1Collector = xo::mm::DX1Collector;
    using CollectorConfig = xo::mm::CollectorConfig;
    using Replxx = replxx::Replxx;

    AppContext(const AppConfig & cfg = AppConfig()) : config_{cfg},
                                                      x1_{CollectorConfig().with_size(4*1024*1024)},
                                                      rdr_{config_.rdr_config_, x1_.ref()}
    {
        rx_.set_max_history_size(config_.max_history_size_);
        rx_.history_load(config_.repl_history_fname_);
        //    rx.bind_key_internal(Replxx::KEY::control('p'), "history_previous");
        //    rx.bind_key_internal(Replxx::KEY::control('n'), "history_next");
    }

    AppConfig config_;
    Replxx rx_;
    DX1Collector x1_;
    SchematikaReader rdr_;
}; 

int
main()
{
    using namespace replxx;

    using xo::scm::SchematikaReader;
    using xo::scm::ReaderConfig;
    using xo::mm::AAllocator;
    using xo::mm::DX1Collector;
    using xo::mm::DArena;
    using xo::facet::with_facet;
    using xo::facet::obj;
    using xo::S_reader2_tag;
    using xo::InitSubsys;
    using xo::Subsystem;
    using xo::scope;
    using namespace std;

    bool interactive = isatty(STDIN_FILENO);

    InitSubsys<S_reader2_tag>::require();
    Subsystem::initialize_all();

    AppConfig cfg;
    AppContext cx(cfg);

    constexpr bool c_debug_flag = false;
    scope log(XO_DEBUG(c_debug_flag));

    using span_type = SchematikaReader::span_type;

    welcome(cerr);

    cx.rdr_.begin_interactive_session();

    bool eof = false;
    const char * input_str = nullptr;
    span_type input;

    while (replxx_getline(interactive, cx.rdr_.is_at_toplevel(), cx.rx_, &input_str)) {
        if (input_str && *input_str) {
            input = span_type::from_cstr(input_str);

            while (!input.empty()
                   && reader_seq(&cx.rdr_, &input, false /*eof*/, c_debug_flag))
                {
                    ;
                }

            /* here: either:
             * 1. input.empty() or
             * 2. error encountered
             */
        }
    }

    /* reminder: eof can complete at most one token */
    reader_seq(&cx.rdr_, &input, true /*eof*/, c_debug_flag);

    cx.rx_.history_save(cx.config_.repl_history_fname_);
}

/* end readerreplxx.cpp */
