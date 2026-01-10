/** @file TkInputState.cpp
 *
 *  @author Roland Conybeare, Jun 2025
 **/

#include "TkInputState.hpp"

namespace xo {
    namespace scm {
        using CharT = char;

        bool
        TkInputState::is_newline(CharT ch) {
            return (ch == '\n');
        }

        bool
        TkInputState::is_whitespace(CharT ch) {
            switch(ch) {
            case ' ': return true;
            case '\t': return true;
            case '\n': return true;
            case '\r': return true;
            }

            return false;
        }

        TkInputState
        TkInputState::rewind(std::size_t n) const
        {
            return TkInputState(this->current_line_,
                                (n <= current_pos_) ? current_pos_ - n : 0,
                                0 /*whitespace*/);
        }

        void
        TkInputState::advance(size_t z)
        {
            scope log(XO_DEBUG(debug_flag_));

            this->current_pos_ += z;

            log && log(xtag("z", z), xtag("current_pos", current_pos_));
        }

        void
        TkInputState::advance_until(const CharT * pos)
        {
            scope log(XO_DEBUG(debug_flag_));

            assert(current_line_.lo() <= pos && pos <= current_line_.hi());

            this->current_pos_ = pos - current_line_.lo();

            log && log(xtag("current_pos", current_pos_));
        }

        auto
        TkInputState::consume_current_line() -> span_type
        {
            span_type retval = current_line_;

            this->discard_current_line();

            return retval;
        }

        void
        TkInputState::discard_current_line()
        {
            this->current_line_ = span_type::make_null();
            this->current_pos_ = 0;
            this->whitespace_ = 0;
        }

        auto
        TkInputState::capture_current_line(const span_type & input,
                                           bool eof_flag)
            -> std::pair<input_error, span_type>
        {
            // see also discard_current_line()
            // note: must capture entirety of first line,
            //       for example including leading whitespace.
            //       See discussion in tokenizer scan() method

            scope log(XO_DEBUG(debug_flag_));

            /* look ahead to {end of line, end of input}, whichever comes first */
            const CharT * sol = input.lo();
            const CharT * eol = sol;

            if (sol == current_line_.lo()) {
                log && log("short-circuit - current line already stashed");

                /* nothing to do here */
                return std::make_pair(input_error::ok, current_line_);
            }

            while ((eol < input.hi()) && (*eol != '\n'))
                ++eol;

            if (*eol == '\n') {
                /* include \n at end-of-line */
                ++eol;
            } else {
                if (!eof_flag) {
                    /* caller expected to provide complete line of input. complain and ignore */
                    return std::make_pair(input_error::incomplete,
                                          input.prefix(0ul));
                }
            }

            this->current_line_ = span_type(sol, eol);
            this->current_pos_ = 0;
            this->whitespace_ = 0;

            log && log(xtag("current_line", print::printspan(current_line_)),
                       xtag("current_pos", current_pos_));

            return std::make_pair(input_error::ok,
                                  span_type(sol, eol));
        }

        const CharT *
        TkInputState::skip_leading_whitespace()
        {
            scope log(XO_DEBUG(debug_flag_));

            const CharT * ix = current_line_.lo() + current_pos_;

            this->whitespace_ = 0;

            /* skip whitespace + remember beginning of most recent line */
            while (is_whitespace(*ix) && (ix != current_line_.hi())) {
                ++ix;

                ++(this->whitespace_);
            }

            this->tk_start_ = ix - current_line_.lo();
            this->current_pos_ = ix - current_line_.lo();

            return ix;
        }


    } /*namespace scm*/
} /*namespace xo*/

/* end TkInputState.cpp */
