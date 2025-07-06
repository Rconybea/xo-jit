/* @file log_state.hpp */

#pragma once

#include "log_config.hpp"
#include "log_streambuf.hpp"
#include "print/pad.hpp"
#include "print/filename.hpp"
#include "print/code_location.hpp"
#include "print/time.hpp"
#include <ostream>
#include <sstream>
#include <memory>   // for std::unique_ptr

namespace xo {
    enum EntryExit {
        EE_Entry,
        EE_Exit
    };

    // track per-thread state associated with nesting logger
    //
    template <typename CharT, typename Traits>
    class state_impl {
    public:
        using log_streambuf_type = log_streambuf<char, std::char_traits<char>>;
        using utc_nanos = xo::time::utc_nanos;

    public:
        state_impl();

        std::uint32_t nesting_level() const { return nesting_level_; }

        void incr_nesting() { ++nesting_level_; }
        void decr_nesting() { --nesting_level_; }

        std::ostream & ss() { return ss_; }

        void check_print_time(utc_nanos now_tm) {
            using xo::time::timeutil;
            using xo::time::utc_nanos;
            using xo::time::hms_msec;
            using xo::time::hms_usec;

            if (log_config::time_enabled) {
                if (log_config::time_local_flag) {
                    if (log_config::time_usec_flag)
                        this->ss_ << hms_usec::local(now_tm) << " ";
                    else
                        this->ss_ << hms_msec::local(now_tm) << " ";
                } else {
                    if (log_config::time_usec_flag)
                        this->ss_ << hms_usec::utc(now_tm) << " ";
                    else
                        this->ss_ << hms_msec::utc(now_tm) << " ";
                }
            }
        } /*check_print_time*/

        /* space budget for time-of-day */
        std::size_t calc_time_indent() const {
            if (log_config::time_enabled) {
                if (log_config::time_usec_flag) {
                    /*strlen("14:38:19.123456 ")*/
                    return 16;
                } else {
                    /*strlen("14:38:19.974 ")*/
                    return 13;
                }
            } else {
                return 0;
            }
        } /*calc_time_indent*/

        void time_indent() {
            if (log_config::time_enabled)
                this->ss_ << pad(this->calc_time_indent(), ' ');
        } /*time_indent*/

        /* call on entry to new scope */
        void preamble(function_style style, std::string_view name1, std::string_view name2);
        /* call before each new log entry */
        void indent(char pad_char);
        /* call on exit from scope */
        void postamble(function_style style, std::string_view name1, std::string_view name2);

        /* write collected output to *p_sbuf */
        void flush2sbuf(std::streambuf * p_sbuf);

        /* discard output, reset write pointer to beginning of buffer */
        void reset_stream() {
            p_sbuf_phase1_->reset_stream();
            p_sbuf_phase2_->reset_stream();
        }

        void set_location(std::string_view file, std::uint32_t line) {
            this->location_flag_ = true;
            this->file_ = std::move(file);
            this->line_ = line;
        } /*set_location*/

    private:
        /* common implementation for .preamble(), .postamble() */
        void entryexit_aux(function_style style,
                           std::string_view name1,
                           std::string_view name2,
                           EntryExit entryexit);

    private:
        /* current nesting level for this thread */
        std::uint32_t nesting_level_ = 0;

        /* buffer space for logging
         * (before pretty-printing for scope::log() calls that span multiple lines)
         * reused across tos() and scope::log() calls
         */
        std::unique_ptr<log_streambuf_type> p_sbuf_phase1_;

        /* #of characters found in .p_sbuf_phase1 since last \n.
         * this value is established+updated in .flush2sbuf().
         * (in particular ignored by stream .ss())
         */
        std::size_t lpos_ = 0;

        /* whenever .set_location() is called:
         * - capture (file, line)
         * - print them near right margin with next output line
         * - ..and reset .location_flag
         */
        bool location_flag_ = false;
        std::string_view file_;
        std::uint32_t line_ = 0;

        /* buffer space for handling scope::log() calls that span multiple lines;
         * inserts extra characters in effort to indent gracefully
         */
        std::unique_ptr<log_streambuf_type> p_sbuf_phase2_;

        /* output stream -- always attached to .p_sbuf_phase1
         * stream inserters for application datatypes will target this stream
         */
        std::ostream ss_;
    }; /*state_impl*/

    constexpr uint32_t c_default_buf_size = 1024*1024;

    template <typename CharT, typename Traits>
    state_impl<CharT, Traits>::state_impl()
        : p_sbuf_phase1_(new log_streambuf_type(c_default_buf_size)),
          p_sbuf_phase2_(new log_streambuf_type(c_default_buf_size)),
          ss_(p_sbuf_phase1_.get())
    {
        assert(p_sbuf_phase1_.get() == ss_.rdbuf());
    } /*ctor*/

    template <typename CharT, typename Traits>
    void
    state_impl<CharT, Traits>::indent(char pad_char)
    {
        //log_streambuf * sbuf = this->p_sbuf_phase1_.get();

#ifdef NOT_IN_USE
        {
            char buf[80];
            ::snprintf(buf, sizeof(buf), "[%02d] ", this->nesting_level_);

            this->ss_ << buf;
            //this->p_sbuf_->sputn(buf, strlen(buf));
        }
#endif

        /* indent to nesting level.
         *
         * note: see also flush2sbuf(),  need special indent handling for continuation lines
         *       (when application sends explicit newlines to this logger)
         */
        this->ss_ << pad(std::min(this->nesting_level_ * log_config::indent_width,
                                  log_config::max_indent_width),
                         pad_char);
    } /*indent*/

    template <typename CharT, typename Traits>
    void
    state_impl<CharT, Traits>::entryexit_aux(function_style style,
                                             std::string_view name1,
                                             std::string_view name2,
                                             EntryExit entryexit)
    {
        log_streambuf_type * sbuf = this->p_sbuf_phase1_.get();

        sbuf->reset_stream();

        this->check_print_time(xo::time::timeutil::now());
        this->indent(' ');

        char ee_label = '\0';
        color_spec_type fn_color;

        /* mnemonic for scope entry/exit */
        switch(entryexit) {
        case EE_Entry:
            ee_label = '+';
            fn_color = log_config::function_entry_color;
            break;
        case EE_Exit:
            ee_label = '-';
            fn_color = log_config::function_exit_color;
            break;
        }

        this->ss_ << ee_label;

        if (log_config::nesting_level_enabled) {
            /* e.g.
             *   (^[[38;5;195m7^[[0m)
             *    <-----a---->b<-c->
             *
             *   a = color on
             *   b = level - displayed in color
             *   c = color off
             */
            this->ss_
                << "("
                << with_color(log_config::nesting_level_color,
                              this->nesting_level_)
                << ")";
        }

        if (log_config::indent_width > 0)
            this->ss_ << ' ';

        /* scope name - note no trailing newline;  expect .preamble()/.postamble() caller to supply */
        this->ss_ << function_name(style, fn_color, name1) << name2;
    } /*entryexit_aux*/

    template <typename CharT, typename Traits>
    void
    state_impl<CharT, Traits>::preamble(function_style style,
                                        std::string_view name1,
                                        std::string_view name2)
    {
        this->entryexit_aux(style, name1, name2, EE_Entry);
    } /*preamble*/

    template <typename CharT, typename Traits>
    void
    state_impl<CharT, Traits>::postamble(function_style style,
                                         std::string_view name1,
                                         std::string_view name2)
    {
        this->entryexit_aux(style, name1, name2, EE_Exit);
    }  /*postamble*/

    template <typename CharT, typename Traits>
    void
    state_impl<CharT, Traits>::flush2sbuf(std::streambuf * p_sbuf)
    {
        log_streambuf_type * sbuf1 = this->p_sbuf_phase1_.get();
        log_streambuf_type * sbuf2 = this->p_sbuf_phase2_.get();

        /* generally expecting sbuf to contain one line of output.
         * if it contains multiple newlines,  need to indent
         * after each one.
         *
         * will scan output in *sbuf1,  post-process to *sbuf2,
         * then write *sbuf2 to output stream
         *
         * note: we inherit .lpos from prec call to .flush2sbuf(),
         *       in the unlikely event that it's non-zero
         */
        char const * s = sbuf1->lo();
        char const * e = s + sbuf1->pos();

        char const * p = s;

        /* point to first space following a non-space character.
         * will indent to just after this space
         */
        char const * space_after_nonspace = nullptr;

        /* true on VT100 color escape (\033);   in which case false on terminating char (m)
         * don't advance lpos during escape
         */
        bool in_color_escape = false;

        while(true) {
            bool have_nonspace = false;

            /* invariant: s<=p<=e */

            /* for indenting,  looking for first 'space following non-space, on first line', if any */

            std::size_t lpos_on_newline = 0;

            while(p < e) {
                if(space_after_nonspace) {
                    ;
                } else {
                    if(*p != ' ')
                        have_nonspace = true;

                    if(have_nonspace && (*p == ' ')) {
                        space_after_nonspace = p;
                    }
                }

                if (in_color_escape && (*p != '\n')) {
                    /* in color escape -> don't advance .lpos */
                    if (*p == 'm')
                        in_color_escape = false;
                    ++p;
                } else if (*p == '\033') {
                    /* begin color escape sequence */
                    in_color_escape = true;
                    ++p;
                } else if (*p == '\n') {
                    /* reset .pos on newline;  also drop any (incomplete + ill-formed) color escape */

                    in_color_escape = false;

                    lpos_on_newline = this->lpos_;
                    this->lpos_ = 0;

                    ++p;
                    break;
                } else {
                    /* increment .lpos on non-newline */
                    ++(this->lpos_);
                    ++p;
                }
            }

            /* p=e or *p=\n */

            /* charseq [s,p) does not contain any newlines,  print it */
            if (lpos_on_newline > 0) {
                /* charseq [s,p) does not contain any newlines,  print it */
                sbuf2->sputn(s, p - s - 1);

                if (this->location_flag_) {
                    /* 'tab' to position lpos for [file:line] */
                    sbuf2->sputc(' ');
                    for (std::uint32_t i = lpos_on_newline + 1; i < log_config::location_tab; ++i)
                       sbuf2->sputc(' ');

                    std::stringstream ss;
                    ss << code_location(this->file_, this->line_,
                                        log_config::code_location_color);

                    std::string ss_str = ss.str(); /*hoping for copy elision here*/
                    sbuf2->sputn(ss_str.c_str(), ss_str.size());

                    this->location_flag_ = false;
                    this->file_ = "";
                    this->line_ = 0;
                }

                sbuf2->sputc('\n');
            } else {
                /* control here if .flush2sbuf() called without trailing newline in .p_sbuf_phase1 */
                sbuf2->sputn(s, p - s);
            }

            if (p == e)
                break;

            // {
            //   char buf[80];
            //   snprintf(buf, sizeof(buf), "*** indent=[%d] next=[%c]", this->nesting_level_, *(p+1));
            //
            //   std::clog.rdbuf()->sputn(buf, strlen(buf));
            //}

            /* control here only for continuation lines (application logging code embedding its own newlines)
             * - minimum indent = nesting level;
             * - however if space_after_nonspace defined, also indent for that
             */
            std::uint32_t n_indent = 0;

            n_indent += this->calc_time_indent();

            n_indent += std::min(this->nesting_level_ * log_config::indent_width,
                                 log_config::max_indent_width);

            /* this is just to indent for per-line entry/exit label */
            if(space_after_nonspace)
                n_indent += (space_after_nonspace - s);

            for(std::uint32_t i = 0; i < n_indent; ++i)
                sbuf2->sputc(' ');

            s = p;
        }

        /* now write entire contents of *sbuf2 to clog */
        p_sbuf->sputn(sbuf2->lo(), sbuf2->pos());

        /* reset streams for next message */
        this->reset_stream();
    } /*flush2sbuf*/
} /*namespace xo*/

/* end log_state.hpp */
