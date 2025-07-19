/* @file scope.hpp */

#pragma once

#include "log_state.hpp"
#include "print/ppconfig.hpp"
#include "print/filename.hpp"
#include "print/ppstr.hpp"
#include "print/tostr.hpp"

#include <stdexcept>
#include <cstdint>
#include <memory>   // for std::unique_ptr

namespace xo {

    template <typename ChartT, typename Traits>
    class state_impl;

#  define XO_ENTER0(lvl) xo::scope_setup(xo::log_level::lvl, xo::log_config::style, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#  define XO_ENTER1(lvl, debug_flag) XO_ENTER2(lvl, debug_flag, __PRETTY_FUNCTION__)
#  define XO_ENTER2(lvl, debug_flag, name1) xo::scope_setup((debug_flag ? xo::log_level::lvl : xo::log_level::never), xo::log_config::style, name1, __FILE__, __LINE__)

#  define XO_DEBUG(debug_flag) XO_ENTER1(always, debug_flag)
#  define XO_DEBUG2(debug_flag, name1) XO_ENTER2(always, debug_flag, name1)

#  define XO_LITERAL(lvl, name1, name2) xo::scope_setup(lvl, function_style::literal, name1, name2, __FILE__, __LINE__)

//#  define XO_SSETUP0() xo::scope_setup(__FUNCTION__)
//#  define XO_SSETUP0(lvl) xo::scope_setup(xo::log_level::lvl, xo::log_config::style, __PRETTY_FUNCTION__, __FILE__, __LINE__)

    /* throw exception if condition not met*/
#  define XO_EXPECT(f,msg) if(!(f)) { throw std::runtime_error(msg); }
    /* establish scope using current function name */
#  define XO_SCOPE(varname, lvl) xo::scope varname(xo::scope_setup(xo::log_level::lvl, xo::log_config::style, __PRETTY_FUNCTION__, __FILE__, __LINE__))
    /* like XO_SCOPE(name),  but also set enabled flag */
//#  define XO_SCOPE2(name, debug_flag) xo::scope name(xo::scope_setup(xo::log_config::style, __PRETTY_FUNCTION__, __FILE__, __LINE__, debug_flag))
#  define XO_SCOPE_DISABLED(name) xo::scope name(xo::scope_setup(xo::log_level::never, xo::log_config::style, __PRETTY_FUNCTION__, __FILE__, __LINE__))
#  define XO_STUB() { XO_SCOPE(logr); logr.log("STUB"); }

    /** @class scope_setup
     *  @brief Collect code-location information.
     *
     *  Typically used with logging macros like @ref XO_SCOPE
     *  Application code isn't expected to interact with this class directly
     **/
    struct scope_setup {
        scope_setup(log_level level, function_style style, std::string_view name1, std::string_view name2,
                    std::string_view file, std::uint32_t line)
            : log_level_{level}, style_{style}, name1_{name1}, name2_{name2}, file_{file}, line_{line} {}
        scope_setup(log_level level, function_style style,
                    std::string_view name1, std::string_view file, std::uint32_t line)
            : scope_setup(level, style, name1, "" /*name2*/, file, line) {}

        bool is_enabled() const { return (this->log_level_ >= log_config::min_log_level); }

        //static scope_setup literal(std::string_view name1, bool enabled_flag = true) { return scope_setup(FS_Literal, name1, enabled_flag); }
        //static scope_setup literal(std::string_view name1, std::string_view name2, bool enabled_flag = true) { return scope_setup(FS_Literal, name1, name2, enabled_flag); }

        /** @defgroup scope_setup-instance-vars scope_setup instance variables **/
        ///@{

        /** minimum severity level for logging -- write messages with severity >= this level **/
        log_level log_level_ = log_level::error;
        /** @c FS_Pretty | @c FS_Streamlined | @c FS_Simple **/
        function_style style_ = function_style::pretty;
        /** name extracted from left-hand side of symbol split (e.g. foo in method foo::bar) **/
        std::string_view name1_ = "<.name1>";
        /** name extracted from right-hand side of symbol split (e.g. bar in method foo::bar) **/
        std::string_view name2_ = "<.name2>";
        /** captured value of `__FILE__` **/
        std::string_view file_ = "<.file>";
        /** captured value of `__LINE__` **/
        std::uint32_t line_ = 0;

        ///@}
    }; /*scope_setup*/

    /** @class basic_scope
     *  @brief Track nesting level across participating function calls use to drive indentation.
     *
     *  @tparam CharT character representation type.  Usually @c char
     *  @tparam Traits character traits,  usually @c std::char_traits<ChartT>
     *
     * Example:
     *  @code
     *   using xo::scope;
     *
     *   void myfunc() {
     *     XO_SCOPE(log);  //or scope x("myfunc")
     *     log(a,b,c);
     *     anotherfunc();
     *     log(d,e,f);
     *   }
     *
     *   void anotherfunc() {
     *     XO_SCOPE(x); // or scope x("anotherfunc").
     *     x.log(y);
     *   }
     *
     * or:
     *   void myfunc() {
     *     bool log_flag = true;
     *     XO_SCOPE2(log, log_flag);    // create local variable 'log'
     *     log && log(a,b,c);           // log iff enabled
     *     log.end_scope();             // optional protection against compiler destroying 'log' early
     *   }
     *
     * output like:
     *    +myfunc:
     *     a,b,c
     *     +anotherfunc:
     *      y
     *     -anotherfunc:
     *     d,e,f
     *    -myfunc:
     *  @endcode
     **/
    template <typename CharT, typename Traits = std::char_traits<CharT>>
    class basic_scope {
    public:
        using state_impl_type = state_impl<CharT, Traits>;
        using log_streambuf_type = log_streambuf<CharT, Traits>;

    public:
        template <typename... Tn>
        basic_scope(scope_setup setup, Tn&&... rest);
        ~basic_scope();

        bool enabled() const { return !finalized_; }

        operator bool() const { return this->enabled(); }

        /* report current nesting level */
        std::uint32_t nesting_level() const;

        void set_dest_sbuf(std::streambuf * x) { this->dest_sbuf_ = x; }

        /** Log arguments in pack @p rest **/
        template<typename... Tn>
        bool log(Tn&&... rest) {
            if(this->finalized_) {
                throw std::runtime_error("basic_scope: attempt to use finalized scope");
            } else {
                state_impl_type * logstate = require_indent_thread_local_state();

                /* indent for timestamp (not printed on this line) */
                logstate->time_indent();

                if (log_config::pretty_print_enabled) {
                    print::ppconfig ppc = {
                        .right_margin_ = log_config::right_margin,
                        .indent_width_ = log_config::indent_width
                    };
                    std::ostream& ss = logstate2stream(logstate);
                    log_streambuf_type & sbuf = logstate2streambuf(logstate);

                    /* use 0 for indent because flush2sbuf responsible for basic_scope toplevel indent */
                    print::ppstate pps(0 /*ci*/, &ppc, &ss, &sbuf);

                    pps.prettyn(ppconcat<std::decay_t<Tn>...>{{std::forward<Tn>(rest)...}});

                } else {
                    /* log to per-thread stream to prevent data races */
                    tosn(logstate2stream(logstate), std::forward<Tn>(rest)...);
                }

                this->flush2sbuf(logstate);
            }

            return true;
        } /*log*/

        /** Log argument in pack @p args **/
        template<typename... Tn>
        bool operator()(Tn&&... args) { return this->log(std::forward<Tn>(args)...); }

        /** Optionally, call once to end scope before dtor.
         *  Logs arguments in pack @p args
         **/
        template<typename... Tn>
        void end_scope(Tn&&... args);

    private:
        /** establish stream for logging;  use thread-local storage for threadsafetỵ
         *  stream, if recycled (ịẹ after 1st call to scopẹlog() from a particular thread),
         *  will be in 'reset-to-beginning of buffer' statẹ
         **/
        static state_impl_type * require_indent_thread_local_state();

        /* establish logging state;  use thread-local storage for threadsafety */
        static state_impl_type * require_thread_local_state();

        /* retrieve permanently-associated ostream for logging-state */
        static std::ostream & logstate2stream(state_impl_type * logstate);

        /* retreive permanently-associated streambuf for logging-state */
        static log_streambuf_type & logstate2streambuf(state_impl_type * logstate);

        /* write collected output to std::clog,  or chosen streambuf */
        void flush2sbuf(state_impl_type * logstate);

    private:
        /* keep logging state separately for each thread */
        static thread_local std::unique_ptr<state_impl_type> s_threadlocal_state;

        /* send indented output to this streambuf (e.g. std::clog.rdbuf()) */
        std::streambuf * dest_sbuf_ = std::clog.rdbuf();
        /* style for displaying .name1 */
        function_style style_ = function_style::pretty;
        /* name of this scope (part 1) */
        std::string_view name1_ = "<name1>";
        /* name of this scope (part 2) */
        std::string_view name2_ = "::<name2>";
        /* captured value of __FILE__ */
        std::string_view file_ = "<file>";
        /* captured value of __LINE__ */
        std::uint32_t line_ = 0;
        /* set once per scope .finalized=true <-> logging disabled */
        bool finalized_ = false;
    }; /*basic_scope*/

    template <typename CharT, typename Traits>
    template <typename... Tn>
    basic_scope<CharT, Traits>::basic_scope(scope_setup setup, Tn&&... args)

        : style_{setup.style_},
          name1_{std::move(setup.name1_)},
          name2_{std::move(setup.name2_)},
          file_{std::move(setup.file_)},
          line_{setup.line_},
          finalized_{!(setup.is_enabled())}
    {
        if(setup.is_enabled()) {
            state_impl_type * logstate = basic_scope::require_thread_local_state();
            std::ostream & os = logstate2stream(logstate);

            logstate->preamble(this->style_, this->name1_, this->name2_);

            tosn(os, " ", std::forward<Tn>(args)...);

            if (log_config::location_enabled) {
                /* prints on next call to flush2sbuf */
                logstate->set_location(this->file_, this->line_);
                //tosn(os, " [", basename(this->file_), ":", this->line_, "]");
            }

            logstate->flush2sbuf(std::clog.rdbuf());

            ///* next call to scope::log() can reset to beginning of buffer space */
            //logstate->ss().seekp(0);

            logstate->incr_nesting();
        }
    } /*ctor*/

    template <typename CharT, typename Traits>
    basic_scope<CharT, Traits>::~basic_scope() {
        if(!this->finalized_)
            this->end_scope();
    } /*dtor*/

    template <typename CharT, typename Traits>
    thread_local std::unique_ptr<state_impl<CharT, Traits>>
    basic_scope<CharT, Traits>::s_threadlocal_state;

    template <typename CharT, typename Traits>
    std::uint32_t
    basic_scope<CharT, Traits>::nesting_level() const {
        return require_thread_local_state()->nesting_level();
    } /*nesting_level*/

    template <typename CharT, typename Traits>
    typename basic_scope<CharT, Traits>::state_impl_type *
    basic_scope<CharT, Traits>::require_indent_thread_local_state()
    {
        state_impl_type * local_state = require_thread_local_state();

        local_state->reset_stream();
        local_state->indent(' ' /*pad_char*/);

        return local_state;
    } /*require_thread_local_stream*/

    template <typename CharT, typename Traits>
    typename basic_scope<CharT, Traits>::state_impl_type *
    basic_scope<CharT, Traits>::require_thread_local_state()
    {
        if(!s_threadlocal_state) {
            s_threadlocal_state.reset(new state_impl_type());
        }

        return s_threadlocal_state.get();
    } /*require_thread_local_state*/

    template <typename CharT, typename Traits>
    std::ostream &
    basic_scope<CharT, Traits>::logstate2stream(state_impl_type * logstate)
    {
        return logstate->ss();
    } /*logstate2stream*/

    template <typename CharT, typename Traits>
    log_streambuf<CharT, Traits> &
    basic_scope<CharT, Traits>::logstate2streambuf(state_impl_type * logstate)
    {
        return logstate->sbuf();
    }

    template <typename CharT, typename Traits>
    void
    basic_scope<CharT, Traits>::flush2sbuf(state_impl_type * logstate)
    {
        logstate->flush2sbuf(this->dest_sbuf_);
    } /*flush2sbuf*/

    template <typename CharT, typename Traits>
    template <typename... Tn>
    void
    basic_scope<CharT, Traits>::end_scope(Tn&&... args)
    {
        if(!this->finalized_) {
            this->finalized_ = true;

            state_impl_type * logstate
                = basic_scope::require_thread_local_state();

            logstate->decr_nesting();

            logstate->postamble(this->style_, this->name1_, this->name2_);

            tosn(logstate2stream(logstate), " ", std::forward<Tn>(args)...);

            logstate->flush2sbuf(std::clog.rdbuf());
        }
    } /*end_scope*/


    using scope = basic_scope<char>;

} /*namespace xo*/

/* end scope.hpp */
