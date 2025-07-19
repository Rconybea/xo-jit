/* @file log_streambuf.hpp */

#pragma once

#include "print/quoted_char.hpp"
#include <iostream>
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
        char const * lo() const { return this->pbase(); }
        char const * hi() const { return this->lo() + this->capacity(); }
        std::uint32_t pos() const { return this->pptr() - this->pbase(); }

        /** number of visible characters since start of line (last \n or \r) **/
        std::uint32_t lpos() const {
            assert(pos() >= solpos_ + color_escape_chars_);
            return pos() - solpos_ - color_escape_chars_;
        }

        rewind_state checkpoint() const {
            return rewind_state(solpos_, color_escape_chars_, pos());
        }

        bool debug_flag() const { return debug_flag_; }

        void reset_stream() {
            char * p_lo = &(this->buf_v_[0]);
            char * p_hi = p_lo + this->capacity();

            /* tells parent our buffer extent */
            this->setp(p_lo, p_hi);

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

            /* .setp(): using for side effect: sets .pptr to .pbase */
            this->setp(this->pbase(), this->epptr());
            this->pbump(s.pos);

            this->solpos_ = s.solpos;
            this->color_escape_chars_ = s.color_escape_chars;
        }

    protected:
        /** expand buffer storage (by 2x), preserve current contents **/
        void
        expand_to(std::size_t new_z) {
            char * old_pptr = pptr();
            std::streamsize old_n = old_pptr - pbase();

            assert(old_n <= static_cast<std::streamsize>(buf_v_.size()));
            assert(new_z > buf_v_.capacity());

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

            /* scan range [pptr, pptr+n] for:
             * 1. completed color escape sequences \033..m
             *    - account for chars in these sequences, since non-printing
             * 2. newlines (and carriage returns)
             *    - remember position of last {newline or carriage return)
             */
            for (char const * p_lo = this->pptr(), * p_hi = p_lo + n, * p = p_lo; p < p_hi; ++p) {
                if (*p == '\n' || *p == '\r') {
                    this->solpos_ = (p+1 - this->pbase());
                    /* reset, since these chars relevant as correction to solpos */
                    this->color_escape_chars_ = 0;
                    /* -> incomplete color escape, broken by newline */
                    this->color_escape_start_ = nullptr;
                } else if (*p == '\033') {
                    if (debug_flag_) {
                        std::cout << "xsputn: \\033 at p-p_lo=" << (p - p_lo) << std::endl;
                    }
                    this->color_escape_start_ = p;
                } else if (this->color_escape_start_ != nullptr) {
                    if (*p == 'm') {
                        /* escape seq non-printing including both endpoints */
                        std::int64_t esc_chars = (p+1 - color_escape_start_);
                        this->color_escape_chars_ += esc_chars;

                        if (debug_flag_) {
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

            this->pbump(ncopied);

            return ncopied;
        } /*xsputn*/

        virtual int_type
        overflow(int_type new_ch) override {
            char * old_pptr = this->pptr();
            std::streamsize old_n = old_pptr - this->pbase();

            assert(old_n <= static_cast<std::streamsize>(this->buf_v_.size()));

            if (debug_flag_) {
                std::cout << "overflow: new_ch=" << quoted_char(new_ch) << std::endl;
            }

            this->expand_to(2 * buf_v_.size());

            this->buf_v_[old_n] = new_ch;
            this->pbump(1);

            if ((new_ch == static_cast<int_type>('\n')) || (new_ch == static_cast<int_type>('\r'))) {
                this->solpos_ = this->pos();
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
        /** position (relative to pbase) one character after last \n or \r.
         *  Use to drive @ref lpos
         **/
        std::size_t solpos_ = 0;
        /** number of non-printing chars after @ref solpos_, from
         *  completed color escape sequences.
         *  (ansi color escapes = text between '\033' and 'm')
         **/
        std::size_t color_escape_chars_ = 0;
        /** non-null: start of incomplete color escape sequence **/
        char const * color_escape_start_ = nullptr;
        /** buffered output stored here **/
        std::vector<char> buf_v_;
        /** true to debug log_streambuf itself **/
        bool debug_flag_ = false;
    }; /*log_streambuf*/

} /*namespace xo*/

/* end log_streambuf.hpp */
