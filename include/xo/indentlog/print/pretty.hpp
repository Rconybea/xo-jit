/* file pretty.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "xo/indentlog/print/ppconfig.hpp"
#include "xo/indentlog/log_streambuf.hpp"
#include "ppdetail_atomic.hpp"
#include "pad.hpp"
#include <sstream>
#include <vector>
#include <utility>
#include <cstdint>

namespace xo {
    namespace print {

        /** @class ppstate
         *  @brief hold pretty-printer state
         *
         *  Use:
         *
         *    ppconfig ppc;
         *    ppstate pps(&cout, 0, &ppc);
         *
         *    pps.pretty("first");
         *    pps.pretty("second");
         **/
        struct ppstate {
            using streambuf_type = log_streambuf<char, std::char_traits<char>>;

            explicit ppstate(std::uint32_t ci, const ppconfig * config, std::ostream * scratch_ss, streambuf_type * scratch_sbuf)
                : scratch_sbuf_{scratch_sbuf}, scratch_ss_{scratch_ss},
                  current_indent_{ci}, config_{config}
                {}

            uint32_t indent_width() const { return config_->indent_width_; }

            std::uint32_t pos() const { return scratch_sbuf_->pos(); }
            std::uint32_t lpos() const { return scratch_sbuf_->lpos(); }

            /** space available from current position until @c ppconfig.right_margin_
             *  0 if current position is already beyond right margin
             **/
            std::uint32_t avail_margin() const {
                std::uint32_t p = lpos();
                std::uint32_t m = config_->right_margin_;

                return (p < m) ? (m - p) : 0;
            }

            bool has_margin() const { return avail_margin() > 0; }

            /** true if at least @p budget chars of space available
             *  before reaching right margin @c ppconfig.right_margin_
             **/
            bool has_budget(std::uint32_t budget) {
                return avail_margin() >= budget;
            }

            /** true if no newlines in range [@p lo, pos),
             *  where pos is current position
             **/
            bool scan_no_newline(std::uint32_t start) const {
                char const * p = scratch_sbuf_->lo() + start;
                char const * e = scratch_sbuf_->lo() + pos();

                for (; p < e; ++p) {
                    if (*p == '\n')
                        return false;
                }

                return true;
            }

            void indent(std::uint32_t tab) {
                (*scratch_ss_) << spaces(tab);
            }

            void newline_indent(std::uint32_t tab) {
                (*scratch_ss_) << "\n";
                this->indent(tab);
            }

            template <typename T>
            void write(T && x) {
                (*scratch_ss_) << x;
            }

            uint32_t incr_nesting_level() { return ++nesting_level_; }
            uint32_t decr_nesting_level() { return --nesting_level_; }

            virtual void commit() {}

            template <typename T>
            bool print_upto(T && x);

            template <typename T>
            ppstate& pretty(T && x);

            /** like pretty(x), but follow with trailing \n **/
            template <typename T>
            ppstate& prettyn(T && x);

        private:
            /** scratch space for pretty-printed output.
             *  Shared across nesting pretty calls.
             *  Caller responsible for saving/restoring state.
             **/
            streambuf_type * scratch_sbuf_ = nullptr;

            /** ostream pointing to @ref scratch_sbuf_ **/
            std::ostream * scratch_ss_ = nullptr;

            /** level of reentrant pretty calls.
             *  Collect formatted text in @ref scratch_sbuf_
             *  until end of toplevel @ref pretty call
             **/
            std::uint32_t nesting_level_ = 0;

            /** current desired indent from left margin **/
            std::uint32_t current_indent_ = 0;

            /** readonly pretty-printer config **/
            const ppconfig * config_ = nullptr;
        };

        /** @class ppstate_standalone
         *  @brief like ppstate, but also holds streambuf
         */
        struct ppstate_standalone : public ppstate {
            explicit ppstate_standalone(std::ostream * os, std::uint32_t ci, const ppconfig * config)
                : ppstate(ci, config, &scratch_ss_, &scratch_sbuf_), output_{os}, scratch_sbuf_{1024*1024}
                {}

            virtual void commit() override {
                output_->write(scratch_sbuf_.lo(), scratch_sbuf_.pos());
                reset_scratch_sbuf();
            }

            void reset_scratch_sbuf() {
                /* reset scratch_sbuf for next time */
                scratch_sbuf_.pubseekoff(0, std::ios_base::beg, std::ios_base::out);
            }

            /** send final output to this stream **/
            std::ostream * output_ = nullptr;

            /** streambuf (with newline tracking) **/
            streambuf_type scratch_sbuf_;

            /** always attached to @ref scratch_sbuf_ **/
            std::ostream scratch_ss_{&scratch_sbuf_};
        };

        struct ppcommitter {
            ppcommitter(ppstate * pps) : pps_{pps} {
                pps_->incr_nesting_level();
            }
            ~ppcommitter() {
                check_commit();
            }

            void check_commit() {
                if (pps_) {
                    ppstate * pps = pps_;
                    pps_ = nullptr;
                    if (pps->decr_nesting_level() == 0)
                        pps->commit();
                }
            }

            ppstate * pps_ = nullptr;
        };

        template <typename T>
        bool
        ppdetail_atomic<T>::print_upto(ppstate * pps, const T & x)
        {
            /* position before we write */
            if (!pps->has_margin())
                return false;

            std::uint32_t start = pps->pos();

            /* if x is non-atomic may optimize by checking budget more often */
            pps->write(x);

            if (!pps->has_margin())
                return false;

            /* scan characters [lo, hi) in line buffer.
             * Newline -> caller should use print_pretty()
             */
            return (pps->scan_no_newline(start));
        }

        template <typename T>
        void
        ppdetail_atomic<T>::print_pretty(ppstate * pps, const T & x)
        {
            /* In default implementation we don't know where to introduce newlines.
             * Still need to calculate lpos though
             */
            pps->write(x);
        }

        template <typename T>
        bool
        ppstate::print_upto(T && value) {
            return ppdetail<std::decay_t<T>>::print_upto(this, value);
        }

        template <typename T>
        ppstate &
        ppstate::pretty(T && value) {
            /* just because value fits doesn't mean we should print it.
             * Decision depends on ancestor context
             */

            std::uint32_t saved = pos();

            ppcommitter ppc(this);

            /* print_upto() modifies @ref scratch_sbuf_ (and @ref scratch_ss_) */
            bool fits = ppdetail<std::decay_t<T>>::print_upto(this, static_cast<const T &>(value));

            if (fits) {
                /* fits on 1 line -> pretty-printing maybe not required */
                return *this;
            }

            /* here: didn't fit -> split over multiple lines */

            this->scratch_sbuf_->rewind_to(saved);
            ppdetail<std::decay_t<T>>::print_pretty(this, value);
            ppc.check_commit();

            return *this;
        }

        template <typename T>
        ppstate &
        ppstate::prettyn(T && value) {
            /* just because value fits doesn't mean we should print it.
             * Decision depends on ancestor context
             */

            std::uint32_t saved = pos();

            ppcommitter ppc(this);

            /* print_upto() modifies @ref scratch_sbuf_ (and @ref scratch_ss_) */
            bool fits = ppdetail<std::decay_t<T>>::print_upto(this, value);

            if (fits) {
                this->newline_indent(0);
                /* fits on 1 line -> pretty-printing maybe not required */
                return *this;
            }

            /* here: didn't fit -> split over multiple lines */

            this->scratch_sbuf_->rewind_to(saved);
            ppdetail<std::remove_reference_t<T>>::print_pretty(this, value);
            this->newline_indent(0);
            ppc.check_commit();

            return *this;
        }

    } /*namespace print*/
} /*namespace xo*/
