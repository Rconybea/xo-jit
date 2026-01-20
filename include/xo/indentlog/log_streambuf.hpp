/* @file log_streambuf.hpp */

#pragma once

#include "print/quoted_char.hpp"
#include <iostream>
#include <string_view>
#include <vector>
#include <cstring>   // e.g. for std::memcpy()
#include <cstdint>
#include <cassert>

namespace xo {
    /** recycling buffer for logging and pretty-printing
     *  - write to self-extending storage array
     *  - track position relative to start of line
     **/
    template <typename CharT, typename Traits = std::char_traits<CharT>>
    class log_streambuf : public std::streambuf {
    public:
        struct rewind_state {
            explicit rewind_state(std::size_t solpos, std::size_t color_esc, std::uint32_t p)
            : solpos{solpos}, color_escape_chars{color_esc}, pos{p} {}

            std::size_t solpos = 0;
            std::size_t color_escape_chars = 0;
            std::uint32_t pos = 0;
        };

    public:
        log_streambuf(std::uint32_t buf_z, bool debug_flag = false) : debug_flag_{debug_flag} {
            this->buf_v_.resize(buf_z);
            this->reset_stream();
        } /*ctor*/

        std::streamsize capacity() const { return this->buf_v_.size(); }
        const char * lo() const { return this->pbase(); }
        const char * hi() const { return this->lo() + this->capacity(); }
        std::uint32_t pos() const { return this->pptr() - this->pbase(); }

        /** output position (relative to pbase) when local state last computed. Exposed here for unit tests **/
        const char * _local_ppos() const { return local_ppos_; }
        /** position (relative to pbase) one character after last \n or \r. For unit tests **/
        std::uint32_t _solpos() const { return solpos_; }
        /** start of incomplete color-escape sequence **/
        const char * _color_escape_start() const { return color_escape_start_; }
        /** number of non-printing chars after @ref solpos_ from completed color-escape sequences **/
        std::uint32_t _color_escape_chars() const { return color_escape_chars_; }

        /** number of visible characters since start of line (last \n or \r) **/
        std::uint32_t lpos() const {
            if (debug_flag_) {
                std::cerr << "log_streambuf::lpos: enter" << std::endl;
            }

            // logically-const. lazy implementation
            log_streambuf * self = const_cast<log_streambuf *>(this);

            self->_check_update_local_state();

            return pos() - solpos_ - color_escape_chars_;
        }

        rewind_state checkpoint() const {
            // logically-const. lazy implementation
            log_streambuf * self = const_cast<log_streambuf *>(this);

            self->_check_update_local_state();

            return rewind_state(solpos_, color_escape_chars_, pos());
        }

        bool debug_flag() const { return debug_flag_; }

        operator std::basic_string_view<CharT> () const { return std::basic_string_view<CharT>(this->pbase(), this->pptr()); }

        void reset_stream() {
            char * p_lo = &(this->buf_v_[0]);
            char * p_hi = p_lo + this->capacity();

            /* tells parent our buffer extent */
            this->setp(p_lo, p_hi);

            this->local_ppos_ = 0;
            this->solpos_ = 0;
            this->color_escape_chars_ = 0;
            this->color_escape_start_ = nullptr;
        } /*reset_stream*/

        void rewind_to(rewind_state s) {
            if (debug_flag_) {
                std::cout << "rewind_to: pos " << pos() << "->" << s.pos
                          << " solpos " << solpos_ << "->" << s.solpos
                          << " color_esc " << color_escape_chars_ << "->" << s.color_escape_chars
                          << std::endl;
            }

            /* .setp(): using just for side effect: sets .pptr to .pbase */
            this->setp(this->pbase(), this->epptr());
            /* advance pptr to saved position */
            this->pbump(s.pos);

            this->local_ppos_ = this->pptr() - this->pbase();
            this->solpos_ = s.solpos;
            this->color_escape_chars_ = s.color_escape_chars;
            /* assuming we never try to capture rewind state with incomplete color escape */
            this->color_escape_start_ = nullptr;
        }

    protected:
        /** expand buffer storage (by 2x), preserve current contents **/
        void
        expand_to(std::size_t new_z) {
            char * old_pptr = pptr();
            std::streamsize old_n = old_pptr - pbase();

            assert(old_n <= static_cast<std::streamsize>(buf_v_.size()));
            assert(new_z > buf_v_.capacity());

            /* note: local_ppos_ invariant across expand_to() */

            this->buf_v_.resize(new_z);

            char * p_base = &(this->buf_v_[0]);
            char * p_hi = p_base + this->buf_v_.capacity();

            this->setp(p_base, p_hi);
            this->pbump(old_n);
        }

        virtual std::streamsize
        xsputn(char const * s, std::streamsize n) override {
            /* s must be an address in [this->lo() .. this->lo() + capacity()] */

            assert(hi() >= pptr());

            if (pptr() + n > hi()) {
                this->expand_to(std::max(2 * this->buf_v_.capacity(), std::size_t(this->pos() + n + 1)));
            }

            if (debug_flag_) {
                std::cout << "xsputn: pbase=" << (void *)(this->pbase())
                          << ", pptr=" << (void*)(this->pptr())
                          << "(+" << (this->pptr() - this->lo()) << ")"
                          << ", n=" << n << " -> (+" << (this->pptr() + n - this->lo()) << ")"
                          << ", buf_v.size=" << this->buf_v_.size()
                          << std::endl;
            }

            std::streamsize ncopied = 0;

            if (this->pptr() + n > this->hi()) {
                ncopied = this->hi() - this->pptr();
            } else {
                ncopied = n;
            }

            if (false /*debug_flag_*/) {
                std::cout << "xsputn: copying ncopied=" << ncopied << " (/n=" << n << ") bytes into range [lo,hi)"
                          << ", lo=" << (void*)this->pptr()
                          << ", hi=" << (void*)(this->pptr() + n)
                          << std::endl;
            }

            std::memcpy(this->pptr(), s, ncopied);

            this->pbump(ncopied);

            /* now {pbase, pptr} consistent with new input */

            this->_check_update_local_state();

            return ncopied;
        } /*xsputn*/

        virtual int_type
        overflow(int_type new_ch) override {
            char * old_base = this->pbase();
            char * old_pptr = this->pptr();
            /* #of chars buffered */
            std::streamsize old_n = old_pptr - old_base;

            assert(old_n <= static_cast<std::streamsize>(this->buf_v_.size()));

            if (debug_flag_) {
                std::cout << "overflow: new_ch=" << quoted_char(new_ch) << std::endl;
            }

            /* increase buffer size */
            this->expand_to(2 * buf_v_.size());

            this->buf_v_[old_n] = new_ch;
            this->pbump(1);

            if ((new_ch == static_cast<int_type>('\n')) || (new_ch == static_cast<int_type>('\r'))) {
                this->solpos_ = this->pos();

                // what if new_ch starts color escape ?
            }

            if (new_ch == Traits::eof()) {
                /* reminder: returning eof sets badbit on ostream */
                return Traits::not_eof(new_ch);
            } else {
                return new_ch;
            }
        } /*overflow*/

        /* off.   offset, relative to starting point dir.
         * dir.
         * which. in|out|both
         *
         * Note that off=0,dir=cur,which=out reads offset
         */
        virtual pos_type seekoff(off_type off,
                                 std::ios_base::seekdir dir,
                                 std::ios_base::openmode which) override {
            //std::cout << "seekoff: off=" << off << ", dir=" << dir << ", which=" << which << std::endl;
            if (debug_flag_) {
                std::cout << "seekoff(off,dir,which)" << std::endl;
            }

            // Only output stream is supported
            if (which != std::ios_base::out)
                throw std::runtime_error("log_streambuf: only output mode supported");

            if (dir == std::ios_base::cur) {
                this->pbump(off);
            } else if (dir == std::ios_base::end) {
                /* .setp(): using for side effect: sets .pptr to .pbase */
                this->setp(this->pbase(), this->epptr());
                this->pbump(off);
            } else if (dir == std::ios_base::beg) {
                /* .setp(): using for side effect: sets .pptr to .pbase */
                this->setp(this->pbase(), this->epptr());
                this->pbump(this->capacity() + off);
            }

            return this->pptr() - this->pbase();
        } /*seekoff*/

    private:
        void _update_local_state_char(const char * p_lo, const char * p)
        {
            if ((*p == '\n') || (*p == '\r')) {
                this->solpos_ = (p+1 - this->pbase());
                /* reset, since these chars relevant as correction to solpos */
                this->color_escape_chars_ = 0;
                /* -> incomplete color escape, broken by newline */
                this->color_escape_start_ = nullptr;
            } else if (*p == '\033') {
                if (debug_flag_) [[unlikely]] {
                    std::cout << "xsputn: \\033 at p-p_lo=" << (p - p_lo) << std::endl;
                }
                this->color_escape_start_ = p;
            } else if (this->color_escape_start_ != nullptr) {
                if (*p == 'm') {
                    /* escape seq non-printing including both endpoints */
                    std::int64_t esc_chars = (p+1 - color_escape_start_);

                    this->color_escape_chars_ += esc_chars;

                    if (debug_flag_) [[unlikely]] {
                        std::cout << "xsputn: m at p-p_lo" << (p - p_lo) << " +" << esc_chars
                        << " -> color_escape_chars=" << color_escape_chars_ << std::endl;
                    }
                    this->color_escape_start_ = nullptr;
                } else if (!isdigit(*p) && (*p != '[') && (*p != ';')) {
                    /* not color escape after all */
                    this->color_escape_start_ = nullptr;
                }
            }
        }

        /** recognize stale local state vars:
         *  @ref solpos_, @ref color_escape_chars_, @ref color_escape_start_.
         *
         *  Require:
         *  - {pbase, pptr} in consistent state
         *  Promise:
         *  - @c local_ppos_ + @c pbase = @c pptr
         *  - @c solpos_, @c color_escape_chars_, @c color_escape_start_ all up-to-date
         **/
        void _check_update_local_state() {
            const char * p0 = this->pbase();
            const char * pn = this->pptr();

            if (debug_flag_) {
                std::cerr << "_check_update_local_state:" << std::endl;
                std::cerr << "  buf: (p0=" << (void*)p0 << ", pn=" << (void*)pn << ")" << std::endl;
                std::cerr << "  solpos_=" << solpos_ << ", color_escape_chars_=" << color_escape_chars_ << std::endl;
            }

            if (p0 + local_ppos_ == pn) [[likely]] {
                // solpos_, color_escape_chars_, color_escape_start_ all up-to-date
            } else {
                // [pnew, pn): input that hasn't been incorporated into
                //   {solpos_, color_escape_chars_, color_escape_start_)

                const char * pnew = this->pbase() + this->local_ppos_;

                if (debug_flag_) {
                    std::cerr << "_check_update_local_state: range: (pnew=" << (void*)pnew << ", pn=" << (void*)pn << ")" << std::endl;
                }

                for(const char * p = pnew; p < pn; ++p) {
                    this->_update_local_state_char(p0, p);
                }
            }

            // solpos_, color_escape_chars_, color_escape_start_ all up-to-date
            // for current buffered contents

            this->local_ppos_ = pn - p0;

            if (debug_flag_) {
                std::cerr << "_check_update_local_state: pos=" << pos();
                std::cerr << ", solpos=" << solpos_;
                std::cerr << ", color_escape_chars=" << color_escape_chars_ << std::endl;
            }

            assert(pos() >= solpos_ + color_escape_chars_);
        }

    private:
        /*
         *   pbase: start of buffered text.  Thils will be address of buf_v_[0]
         *
         *
         *   pbase                                          pptr             epptr
         *   v                  >e1<  >e2<                     v                 v
         *   |xx\xxEEExxx\xxxxxxxEExxxxEExxxxxxxEExxx\xEExxxxxx..................|
         *                ^                 ^<------new------->
         *           solpos        local_ppos
         *
         *   solpos           : first character after newline (stale)
         *   color_escape_pos : e1+e2+.. (stale)
         *   new              : new characters not reflected
         *                      in local_ppos_, color_escape_chars_ etc.
         *
         *   Legend:
         *    [\] newline
         *    [x] visible character
         *    [E] color escape chars
         *
         *
         *   after _check_update_local_state():
         *
         *
         *   pbase                                          pptr             epptr
         *   v                                        >e1<     v                 v
         *   |xx\xxEEExxx\xxxxxxxEExxxxEExxxxxxxEExxx\xEExxxxxx..................|
         *                                            ^        ^
         *                                       solpos        local_ppos
         *
         */

        /** @defgroup logstreambuf-instance-vars **/
        ///@{

        /** value of pptr (relative to pbase) when _check_update_local_state() last ran **/
        std::size_t local_ppos_ = 0;
        /** position (relative to pbase) one character after last \n or \r.
         *  Use to drive @ref lpos.  This _has_ to be lazy, since
         *  xsputn() isn'g guaranteed to be called when there's room in
         *  in buffer.
         **/
        std::size_t solpos_ = 0;
        /** number of non-printing chars after @ref solpos_, from
         *  completed color escape sequences.
         *  (ansi color escapes = text between '\033' and 'm')
         **/
        std::size_t color_escape_chars_ = 0;
        /** non-null: start of incomplete color escape sequence **/
        const char * color_escape_start_ = nullptr;

        /** buffered output stored here **/
        std::vector<char> buf_v_;
        /** true to debug log_streambuf itself **/
        bool debug_flag_ = false;

        ///@}
    }; /*log_streambuf*/

} /*namespace xo*/

/* end log_streambuf.hpp */
