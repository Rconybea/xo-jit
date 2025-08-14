/* file ListAlloc.cpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#include "ListAlloc.hpp"
#include "ArenaAlloc.hpp"
#include "xo/indentlog/scope.hpp"
#include <cassert>
#include <cstddef>

namespace xo {
    namespace gc {
        ListAlloc::ListAlloc(std::unique_ptr<ArenaAlloc> hd,
                             ArenaAlloc * marked,
                             std::size_t cz, std::size_t nz, std::size_t tz,
                             bool debug_flag)
            : start_z_{cz},
              hd_{std::move(hd)},
              marked_{marked},
              full_l_{},
              current_z_{cz},
              next_z_{nz},
              total_z_{tz},
              debug_flag_{debug_flag}
        {}

        ListAlloc::~ListAlloc()
        {
            this->clear();
        }

        up<ListAlloc>
        ListAlloc::make(const std::string & name, std::size_t cz, std::size_t nz, bool debug_flag)
        {
            std::unique_ptr<ArenaAlloc> hd{ArenaAlloc::make(name,
                                                            cz, debug_flag)};

            if (!hd)
                return nullptr;

            ArenaAlloc * marked = nullptr;

            up<ListAlloc> retval{new ListAlloc(std::move(hd),
                                               marked,
                                               cz, nz, cz,
                                               debug_flag)};

            return retval;
        }

        void
        ListAlloc::capture_object_statistics(capture_phase phase,
                                             ObjectStatistics * p_dest) const
        {
            hd_->capture_object_statistics(phase, p_dest);

            for (const auto & arena : full_l_)
                arena->capture_object_statistics(phase, p_dest);

        }

        const std::string &
        ListAlloc::name() const {
            if (hd_) {
                return hd_->name();
            }

            static std::string s_default_name = "ListAlloc";
            return s_default_name;
        }

        std::size_t
        ListAlloc::page_size() const {
            return hd_->page_size();
        }

        std::size_t
        ListAlloc::size() const {
            return total_z_;
        }

        std::size_t
        ListAlloc::committed() const {
            std::size_t z = 0;
            if (hd_)
                z += hd_->committed();
            for (const auto & a : full_l_)
                z += a->committed();

            return z;
        }

        std::byte *
        ListAlloc::free_ptr() const {
            return hd_->free_ptr();
        }

        std::size_t
        ListAlloc::available() const {
            if (hd_) {
                /* can only allocate from @ref hd_,
                 * so even if there were available space in @ref full_l_,
                 * it's not accessible to ListAlloc.
                 */

                return hd_->available();
            }

            return 0;
        }

        std::size_t
        ListAlloc::allocated() const {
            std::size_t total = 0;

            if (hd_) {
                total += hd_->allocated();
            }

            for (const auto & alloc : full_l_)
                total += alloc->allocated();

            return total;
        }

        bool
        ListAlloc::contains(const void * x) const {
            if (hd_ && hd_->contains(x))
                return true;

            for (const auto & alloc : full_l_) {
                if (alloc->contains(x))
                    return true;
            }

            return false;
        }

        bool
        ListAlloc::is_before_checkpoint(const void * x) const {
            if (!marked_)
                return true;

            if (marked_ && marked_->contains(x))
                return marked_->is_before_checkpoint(x);

            /*
             * 1. allocs in full_l_ appear in oldest-to-youngest order
             * 2. allocators that appear before marked_ in full_l_ count as 'before checkpoint'
             * 3. allocators that appear after  marked_ in full_l_ count as 'after checkpoint'
             */

            bool older_than_marked = true;

            for (const auto & alloc : full_l_) {
                if (older_than_marked) {
                    if (alloc.get() == marked_) {
                        /* nothing else to test on this iteration,
                         * already checked .marked_ specifically
                         */
                        break;
                    } else {
                        /* before checkpoint */
                        if (alloc->contains(x))
                            return true;
                    }
                }
            }

            return false;
        }

        std::size_t
        ListAlloc::before_checkpoint() const
        {
            scope log(XO_DEBUG(false && debug_flag_), xtag("marked", marked_ ? marked_->name() : ""));

            if (marked_) {
                if (full_l_.empty()) {
                    assert(marked_ == hd_.get());

                    return marked_->before_checkpoint();
                } else {
                    std::size_t z = 0;

                    /* control here: .marked & .full_l non-empty. */
                    if (hd_.get() == marked_) {
                        z += hd_->before_checkpoint();

                        /* anything in .full_l is older than marked .hd */
                        for (const auto & alloc : full_l_) {
                            z += alloc->allocated();
                        }

                        return z;
                    } else {
                        /* messiest case: .marked is true,
                         * and not the youngest arena
                         */

                        /* full_l always in increasing time order: oldest-to-youngest order */
                        size_t i_alloc = 0;
                        for (const auto & alloc : full_l_) {
                            log && log(xtag("i_alloc", i_alloc),
                                       xtag("alloc", alloc->name()),
                                       xtag("z", z));

                            if (alloc.get() == marked_) {
                                log && log("marked", xtag("+z", marked_->before_checkpoint()));
                                z += marked_->before_checkpoint();
                                break;
                            } else {
                                log && log("older than marked", xtag("+z", alloc->allocated()));
                                z += alloc->allocated();
                            }
                            ++i_alloc;
                        }
                    }

                    return z;
                }
            } else {
                /* count *everything* allocated */
                return this->allocated();
            }
        }

        std::size_t
        ListAlloc::after_checkpoint() const
        {
            scope log(XO_DEBUG(false && debug_flag_), xtag("marked", marked_ ? marked_->name() : ""));

            if (!marked_)
                return 0;

            if (full_l_.empty()) {
                assert(marked_ == hd_.get());

                return marked_->after_checkpoint();
            }

            bool older_than_marked = true;

            std::size_t z = 0;

            std::size_t i_alloc = 0;
            for (const auto & alloc : full_l_) {
                log && log(xtag("i_alloc", i_alloc),
                           xtag("alloc", alloc->name()),
                           xtag("z", z));

                if (older_than_marked) {
                    if (alloc.get() == marked_) {
                        log && log("marked", xtag("+z", marked_->after_checkpoint()));
                        older_than_marked = false;
                        z += marked_->after_checkpoint();
                    }
                } else {
                    /* younger than marked */
                    log && log("younger", xtag("+z", alloc->allocated()));
                    z += alloc->allocated();
                }

                ++i_alloc;
            }

            /** head must be included, since it's always the youngest bucket **/
            z += hd_->after_checkpoint();

            log && log("z", z);

            return z;
        }

        bool
        ListAlloc::debug_flag() const {
            return debug_flag_;
        }

        void
        ListAlloc::clear() {
            // general hygiene
            start_z_ = 0;
            hd_.reset();
            marked_ = nullptr;
            full_l_.clear();
            current_z_ = 0;
            next_z_ = 0;
            total_z_ = 0;
        }

        bool
        ListAlloc::reset(std::size_t z)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("z", z));

            bool recycle_head_bucket = hd_ && (z <= hd_->size());

            this->full_l_.clear();
            this->marked_ = nullptr;

            if (recycle_head_bucket) {
                this->hd_->clear();
                this->total_z_ = hd_->size();

                return true;
            } else {
                std::string old_name = this->hd_->name();

                this->hd_.reset(nullptr);
                this->total_z_ = 0;

                return this->expand(z, old_name + "+");
            }
        }

        bool
        ListAlloc::expand(std::size_t z, const std::string & name)
        {
            scope log(XO_DEBUG(debug_flag_), xtag("name", name));

            //log && log("before", xtag("before_ckp", this->before_checkpoint()));

            std::size_t cz = current_z_;
            std::size_t nz = next_z_;
            std::size_t tz;

            do {
                tz = cz + nz;
                cz = nz;
                nz = tz;
            } while (cz < z);

            log && log("expand to", xtag("cz", cz));

            std::unique_ptr<ArenaAlloc> new_alloc = ArenaAlloc::make(name,
                                                                     cz, debug_flag_);

            if (!new_alloc)
                return false;

            this->current_z_ = cz;
            this->next_z_ = nz;
            this->total_z_ += cz;

            if (hd_)
                this->full_l_.push_back(std::move(hd_));

            this->hd_ = std::move(new_alloc);

            //log && log("after", xtag("before_ckp", this->before_checkpoint()));

            return true;
        }

        void
        ListAlloc::checkpoint() {
            scope log(XO_DEBUG(debug_flag_));

            hd_->checkpoint();

            this->marked_ = hd_.get();

            log && log(xtag("hd", (void*)hd_.get()), xtag("marked", (void*)marked_));
        }

        std::byte *
        ListAlloc::alloc(std::size_t z) {
            scope log(XO_DEBUG(debug_flag_));

            /* ArenaAlloc::alloc() may modify its own size */

            std::size_t z_pre = hd_->size();
            std::byte * retval = hd_->alloc(z);

            if (retval) {
                std::size_t z_post = hd_->size();
                this->total_z_ += (z_post - z_pre);

                return retval;
            }

            log && log("space exhausted -> expand");

            if (this->expand(z, hd_->name() + "+"))
                return hd_->alloc(z);

            return nullptr;
        }
    } /*namespace gc*/
} /*namespace xo*/

/* end ListAlloc.cpp */
