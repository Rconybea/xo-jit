/* file pretty.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "xo/indentlog/print/ppconfig.hpp"
#include "xo/indentlog/log_streambuf.hpp"
#include "ppdetail_atomic.hpp"
#include "tag.hpp"
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
         *    ppconfig ppc;
         *    ppstate pps(&cout, 0, &ppc);
         *
         *    pps.pretty("first");
         *    pps.pretty("second");
         **/
        struct ppstate {
            using streambuf_type = log_streambuf<char, std::char_traits<char>>;

            explicit ppstate(std::uint32_t ci, const ppconfig * config,
                             std::ostream * scratch_ss, streambuf_type * scratch_sbuf)
                : scratch_sbuf_{scratch_sbuf}, scratch_ss_{scratch_ss},
                  current_indent_{ci}, config_{config}
                {}

            uint32_t indent_width() const { return config_->indent_width_; }
            ppindentinfo indent_info(bool upto) { return ppindentinfo(this, lpos(), indent_width(), upto); }

            std::uint32_t current_indent_unused() const { return current_indent_; }

            std::uint32_t pos() const { return scratch_sbuf_->pos(); }
            /** current position relative to start of line (last \n or \r),
             *  excluding color escape sequences
             **/
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

            /** true if no newlines in range [s, pos),
             *  where s is @p start and pos is current position
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
                assert(tab < config_->assert_indent_threshold);
                (*scratch_ss_) << spaces(tab);
            }

            /** write (to scratch stream) newline followed by @p tab spaces **/
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

            // pretty-printing helpers

            /** pretty-print empty struct **/
            template <typename StructName, typename... Members>
            std::uint32_t pretty_struct(const ppindentinfo & ppii, StructName && structname, Members&&...);

            /** auxiliary function supporting @ref pretty_stuct .
             *  pretty-print a single member name on behalf of a struct/class.
             *  @return true iff printed content fits on remainder of line before right margin
             **/
            template <typename Member, typename... Rest>
            std::uint32_t print_upto_struct_members(const ppindentinfo & ppii,
                                                    Member && member,
                                                    Rest&&... rest);

            /** base-case overload **/
            std::uint32_t print_upto_struct_members(const ppindentinfo &) { return true; }

            /** pretty-print newline-and-indent separated member values.
             *  For struct members, use refrtag(name,value)
             **/
            template <typename Member, typename... Rest>
            void pretty_struct_members(const ppindentinfo & ppii,
                                       Member && member,
                                       Rest&&... rest);

            /** base-case overload **/
            void pretty_struct_members(const ppindentinfo &) {}

            template <typename T>
            bool print_upto(T && x);

            template <typename Name, typename Value>
            bool print_upto_tag(Name && name, Value && value);

            template <typename T>
            ppstate& pretty(T && x);

            /** write (to internal scratch stream), in order:
             *  newline, indent to @p ci, tag @p name, space, @p value
             **/
            template <typename Name, typename Value>
            ppstate& newline_pretty_tag(std::uint32_t ci, Name && name, Value && value);

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
        }; /*ppstate*/

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
                    /* nuke state to prevent duplicate commit */
                    ppstate * pps = pps_;
                    pps_ = nullptr;
                    if (pps->decr_nesting_level() == 0)
                        pps->commit();
                }
            }

            ppstate * pps_ = nullptr;
        };

        template <typename StructName, typename... Members>
        std::uint32_t
        ppstate::pretty_struct(const ppindentinfo & ppii, StructName && structname, Members&&... members)
        {
            if (ppii.upto()) {
                return (this->print_upto("<")
                        && this->print_upto(structname)
                        && this->print_upto_struct_members(ppii, members...)
                        && this->print_upto(">"));
            } else {
                this->write("<");
                this->write(structname);
                this->pretty_struct_members(ppii, members...);
                this->write(">");
                return false;
            }
        }

        template <typename Member, typename... Rest>
        std::uint32_t
        ppstate::print_upto_struct_members(const ppindentinfo & ppii,
                                           Member && member,
                                           Rest&&... rest)
        {
            if (this->print_upto(" ") && this->print_upto(member))
                return this->print_upto_struct_members(ppii, rest...);

            return false;
        }

        template <typename Member, typename... Rest>
        void
        ppstate::pretty_struct_members(const ppindentinfo & ppii,
                                       Member && member,
                                       Rest&&... rest)
        {
            newline_indent(ppii.ci1());
            this->pretty(member);
            this->pretty_struct_members(ppii, rest...);
        }

        template <typename T>
        bool
        ppstate::print_upto(T && value) {
            std::uint32_t saved = pos();

            if (!has_margin())
                return false;

            if (!ppdetail<std::decay_t<T>>::print_pretty(this->indent_info(true /*upto*/), value))
                return false;

            return scan_no_newline(saved);
        }

        template <typename T>
        ppstate &
        ppstate::pretty(T && value) {
            /* just because value fits doesn't mean we should print it.
             * Decision depends on ancestor context
             */

            auto saved = scratch_sbuf_->checkpoint();

            ppcommitter ppc(this);

            /* print_pretty() modifies @ref scratch_sbuf_ (and @ref scratch_ss_) */
            bool fits = ppdetail<std::decay_t<T>>::print_pretty(this->indent_info(true /*upto*/),
                                                                static_cast<const T &>(value));

            if (fits) {
                /* fits on 1 line -> pretty-printing maybe not required */
                return *this;
            }

            /* here: didn't fit -> split over multiple lines */

            this->scratch_sbuf_->rewind_to(saved);
            ppdetail<std::decay_t<T>>::print_pretty(this->indent_info(false), value);
            ppc.check_commit();

            return *this;
        }

        template <typename T>
        ppstate &
        ppstate::prettyn(T && value) {
            /* just because value fits doesn't mean we should print it.
             * Decision depends on ancestor context
             */

            auto saved = scratch_sbuf_->checkpoint();

            ppcommitter ppc(this);

            /* print_pretty() modifies @ref scratch_sbuf_ (and @ref scratch_ss_) */
            bool fits = ppdetail<std::decay_t<T>>::print_pretty(this->indent_info(true), value);

            if (fits) {
                this->newline_indent(0);
                /* fits on 1 line -> pretty-printing maybe not required */
                return *this;
            }

            /* here: didn't fit -> split over multiple lines */

            this->scratch_sbuf_->rewind_to(saved);
            ppdetail<std::remove_reference_t<T>>::print_pretty(this->indent_info(false), value);
            this->newline_indent(0);
            ppc.check_commit();

            return *this;
        }

        template <typename T>
        bool
        ppdetail_atomic<T>::print_pretty(const ppindentinfo & ppii, const T & x)
        {
            if (ppii.upto()) {
                std::uint32_t start = ppii.pps()->pos();

                /* if x is non-atomic may optimize by checking budget more often */
                ppii.pps()->write(x);

                if (!ppii.pps()->has_margin())
                    return false;

                /* scan characters [lo, hi) in line buffer.
                 * Newline -> caller should use print_pretty()
                 */
                return (ppii.pps()->scan_no_newline(start));
            } else {
                /* In default implementation we don't know where to introduce newlines.
                 * Still need to calculate lpos though
                 */
                ppii.pps()->write(x);
                return false;
            }

            assert(false);
            return true;
        }

        namespace detail {
            template <typename Tag>
            struct ppdetail_tag {
                static bool print_pretty(const ppindentinfo & ppii,
                                         const Tag & tag)
                    {
                        if (ppii.upto()) {
                            if (tag.prefix_space())
                                ppii.pps()->write(" ");

                            /* must color here, because we may keep the output if it fits! */
                            if (!ppii.pps()->print_upto(with_color(color_spec_type::yellow(), // tag_config::tag_color,
                                                                   concat((char const *)":", tag.name()))))
                                return false;

                            ppii.pps()->write(" ");

                            if (!ppii.pps()->print_upto(tag.value()))
                                return false;

                            return true;
                        } else {
                            /* pretty-print like
                             *   :somename
                             *      pretty(value)
                             */

                            if (tag.prefix_space())
                                ppii.pps()->write(" ");
                            ppii.pps()->write(with_color(color_spec_type::yellow(),  //tag_config::tag_color,
                                                         concat((char const *)":", tag.name())));

                            ppii.pps()->newline_indent(ppii.ci1());
                            ppii.pps()->pretty(tag.value());

                            return false;
                        }
                    }
            };
        }

        template <bool PrefixSpace, tagstyle TagStyle, typename Name, typename Value>
        struct ppdetail<tag_impl<PrefixSpace, TagStyle, Name, Value>> {
            static bool print_pretty(const ppindentinfo & ppii,
                                     const tag_impl<PrefixSpace, TagStyle, Name, Value> & tag)
                {
                    using tag_type = tag_impl<PrefixSpace, TagStyle, Name, Value>;

                    return detail::ppdetail_tag<tag_type>::print_pretty(ppii, tag);
                }
        };

        template <bool PrefixSpace, tagstyle TagStyle, typename Name, typename Value>
        struct ppdetail<ref_tag_impl<PrefixSpace, TagStyle, Name, Value>> {
            static bool print_pretty(const ppindentinfo & ppii,
                                     const ref_tag_impl<PrefixSpace, TagStyle, Name, Value> & tag)
                {
                    using tag_type = ref_tag_impl<PrefixSpace, TagStyle, Name, Value>;

                    return detail::ppdetail_tag<tag_type>::print_pretty(ppii, tag);
                }
        };

        template <typename Name, typename Value>
        bool
        ppstate::print_upto_tag(Name && name, Value && value)
        {
            return this->print_upto(xrtag(name, value));
        }

        template <typename Name, typename Value>
        ppstate &
        ppstate::newline_pretty_tag(std::uint32_t ci, Name && name, Value && value)
        {
            this->newline_indent(ci);
            this->current_indent_ = ci;
            this->pretty(rtag(name, value));

            return *this;
        }

    } /*namespace print*/
} /*namespace xo*/
