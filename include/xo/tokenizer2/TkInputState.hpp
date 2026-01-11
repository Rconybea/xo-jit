/* @file TkInputState.hpp
 *
 * author: Roland Conybeare, Jun 2025
 */

#pragma once

#include "span.hpp"

namespace xo {
    namespace scm {
        /** enum to report outcome of @ref capture_current_line **/
        enum class input_error {
            /** normal return, input line successfully identified and captured **/
            ok = 0,
            /** incomplete input; should not have been submitted
             *  to @ref capture_current_line.
             *  note: submit last line of input with eof_flag=true
             **/
            incomplete,
            N
        };

        /** @class input_state
         *  @brief Track detailed input position for use in error messages
         *
         *  input characters fall into two categories:
         *  - consumed: memory can be reclaimed/recycled
         *  - buffered: memory will be retained unaltered until consumed
         *
         *  remarks:
         *  - always in one of two states:
         *    - empty
         *    - contains exactly one line of input
         *  - also record current input position.
         *    Use this for example to identify where tokenizer rejected input.
         *  - .current_pos advances by one token
         *
         *  - buffered characters always form a single contiguous range.
         *  - input_state does not own any storage; storage is owned elsewhere
         *
         *  @text
         *
         *    <------------------.current_line------------------>
         *                                   >  <-- .whitespace
         *    cccccccccccccccccccccccccccccccc__TTTTTTTTxxxxxxxxx
         *    ^                                 ^                ^
         *    .current_line.lo                  |                .current_line.hi
         *                           .current_pos
         *
         *    <----prev_line----> <----current_line---->
         *                                   >  <--whitespace
         *    ppppppppppppppppppp cccccccccccc__TTTTTTTT
         *    ^
         *
         *  @endtext
         **/
        class TkInputState {
        public:
            /** @defgroup input-state-type-traits input-state type straits **/
            ///@{

            using CharT = char;

            /** type representing a contiguous span of tokenizer input characters **/
            using span_type = xo::mm::span<const CharT>;

            ///@}

        public:
            /** @defgroup input-state-ctors input_state constructors **/
            ///@{

            TkInputState() = default;
            explicit TkInputState(bool debug_flag) : debug_flag_{debug_flag} {}
            /** Create instance with supplied @p current_line, @p current_pos, @p whitespace.
             *  Introduced for unit tests, not used in tokenizer.
             **/
            explicit TkInputState(const span_type & current_line,
                                  size_t current_pos,
                                  size_t whitespace) : current_line_{current_line},
                                                       current_pos_{current_pos},
                                                       whitespace_{whitespace} {}

            ///@}

            /** @defgroup input-state-static-methods input_state static methods **/
            ///@{

            /** recognize the newline character '\n' **/
            static bool is_newline(CharT ch);
            /** identifies whitespace chars.
             *  These are chars that do not belong to any token.
             *  They are not permitted to appear within
             *  a symbol or string token.
             *  Appearance of a whitespace char forces completioon of
             *  preceding token.
             **/
            static bool is_whitespace(CharT ch);

            ///@}

            /** @defgroup input-state-access-methods **/
            ///@{

#pragma GCC diagnostic push
#ifndef __APPLE__
#pragma GCC diagnostic ignored "-Wchanges-meaning"
#endif
            const span_type & current_line() const { return current_line_; }
#pragma GCC diagnostic pop
            size_t tk_start() const { return tk_start_; }
            size_t current_pos() const { return current_pos_; }
            size_t whitespace() const { return whitespace_; }
            bool debug_flag() const { return debug_flag_; }

            ///@}

            /** @defgroup input-state-general-methods **/
            ///@{

            /** Input state less @p n chars.
             *  Use to recover input state before a complete but error-triggering token
             **/
            TkInputState rewind(std::size_t n) const;

            /** Capture prefix of @p input up to first newline.
             *  Set read position to start of line.
             *
             *  Alters:
             *    .current_line
             *    .current_pos
             *
             * Return pair comprising error code and input span representing first line
             * (including trailing newline) from @p input.
             **/
            std::pair<input_error, span_type> capture_current_line(const span_type & input,
                                                                   bool eof_flag);

            /** atomically return current line while discarding it from input state
             *
             *  Alters
             *    .current_line
             *    .current_pos
             *    .whitespace
             **/
            span_type consume_current_line();

            /** Reset input state for start of next line.
             *  Expression parser may use this to discard remainder of input line
             *  after a parsing error.
             *
             * Alters:
             *   .current_line
             *   .current_pos
             *   .whitespace
             **/
            void discard_current_line();

            /** Advance input position by @p z
             *
             *  Alters:
             *   .current_pos
             **/
            void advance(size_t z);

            /** Advance .current_pos to pos.
             *  Require: pos in @ref current_line_
             **/
            void advance_until(const CharT * pos);

            /** Skip prefix of input, starting at current read position,
             *  comprising only whitespace.
             *
             *  Presume input position is at end of token;
             *  on return @ref whitespace_ counts number of whitespace characters
             *  skipped.
             *
             *  Return pointer to first non-whitespace character after @ref current_pos_
             *  or @ref current_line_.hi if reached end of buffered line.
             *
             *  Alters:
             *    .whitespace
             **/
            const CharT * skip_leading_whitespace();

            ///@}

        private:
            /** @defgroup input-state-instance-vars input_state instance variables **/
            ///@{

            /** remember current input line.  Used only to report errors **/
            span_type current_line_ = span_type();
            /** start of last token within @ref current_line_ **/
            size_t tk_start_ = 0;
            /** input position within @ref current_line_ **/
            size_t current_pos_ = 0;
            /** number of whitespace chars since end of preceding token,
             *  or last newline, whichever is less
             **/
            size_t whitespace_ = 0;

            /** true to log input activity */
            bool debug_flag_ = false;

            ///@}
        }; /*TkInputState*/

        inline std::ostream &
        operator<<(std::ostream & os,
                   const TkInputState & x)
        {
            using xo::print::unq;

            os << "<input_state"
               << xtag("tk", x.tk_start())
               << xtag("pos", x.current_pos())
               << xtag("line",
                       unq(std::string_view(x.current_line().lo(),
                                            x.current_line().hi())))
               << xtag("whitespace", x.whitespace())
            << ">";

            return os;
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end TkInputState.hpp */
