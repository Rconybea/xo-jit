/* file ListAlloc.cpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#include "ListAlloc.hpp"
#include "ArenaAlloc.hpp"
#include <cassert>
#include <cstddef>

namespace xo {
    namespace gc {
        ListAlloc::ListAlloc(std::unique_ptr<ArenaAlloc> hd,
                             ArenaAlloc * marked,
                             std::size_t cz, std::size_t nz, std::size_t tz,
                             bool use_redline,
                             bool debug_flag)
            : start_z_{cz},
              hd_{std::move(hd)},
              marked_{marked},
              full_l_{},
              current_z_{cz},
              next_z_{nz},
              total_z_{tz},
              use_redline_{use_redline},
              debug_flag_{debug_flag}
        {}

        ListAlloc::~ListAlloc()
        {
            this->clear();
        }

        up<ListAlloc>
        ListAlloc::make(const std::string & name, std::size_t cz, std::size_t nz, bool debug_flag)
        {
            std::unique_ptr<ArenaAlloc> hd{ArenaAlloc::make(name, 0, cz, debug_flag)};

            if (!hd)
                return nullptr;

            ArenaAlloc * marked = nullptr;

            up<ListAlloc> retval{new ListAlloc(std::move(hd),
                                               marked,
                                               cz, nz, cz,
                                               false /*!use_redline*/,
                                               debug_flag)};

            return retval;
        }

        std::size_t
        ListAlloc::size() const {
            return total_z_;
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

            if (hd_)
                total += hd_->allocated();

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
                return false;

            if ((marked_ == hd_.get()) && hd_->contains(x))
                return hd_->is_before_checkpoint(x);

            /*
             * 1. allocs in full_l_ appear in youngest-to-oldest order
             * 2. allocators that appear before marked_ in full_l_ count as 'after checkpoint'
             * 3. allocators that appear after  marked_ in full_l_ count as 'before checkpoint'
             */

            bool younger_than_marked = true;

            for (const auto & alloc : full_l_) {
                if (younger_than_marked) {
                    if (alloc.get() == marked_) {
                        /* nothing else to test on this iteration,
                         * already checked .marked_ specifically
                         */
                        younger_than_marked = false;
                    } else {
                        /* after checkpoint */
                        if (alloc->contains(x))
                            return false;
                    }
                } else {
                    if (alloc->contains(x))
                        return true;
                }
            }

            return false;
        }

        std::size_t
        ListAlloc::before_checkpoint() const
        {
            if (marked_) {
                if (full_l_.empty()) {
                    assert(marked_ == hd_.get());

                    return marked_->before_checkpoint();
                }
            } else {
                /* count everything allocated */
                return this->allocated();
            }

            std::size_t z = 0;

            /* control here: .marked & .full_l non-empty. */
            if (hd_.get() == marked_) {
                z += hd_->before_checkpoint();

                /* anything in .full_l older than marked .hd */
                for (const auto & alloc : full_l_) {
                    z += alloc->allocated();
                }

                return z;
            } else {
                /* messiest case: .marked is true,
                 * and not the youngest arena
                 */
                bool younger_than_marked = true;

                for (const auto & alloc : full_l_) {
                    if (younger_than_marked) {
                        if (alloc.get() == marked_) {
                            younger_than_marked = false;
                            z += marked_->before_checkpoint();
                        } else {
                            ;
                        }
                    } else {
                        z += alloc->allocated();
                    }
                }
            }

            return z;
        }

        std::size_t
        ListAlloc::after_checkpoint() const
        {
            if (!marked_)
                return 0;

            if (full_l_.empty()) {
                assert(marked_ == hd_.get());

                return marked_->after_checkpoint();
            }

            bool younger_than_marked = true;

            std::size_t z = 0;

            for (const auto & alloc : full_l_) {
                if (younger_than_marked) {
                    if (alloc.get() == marked_) {
                        younger_than_marked = false;
                        z += marked_->after_checkpoint();
                        break;
                    } else {
                        z += alloc->allocated();
                    }
                }
            }

            return z;
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
            use_redline_ = false;
        }

        bool
        ListAlloc::reset(std::size_t z)
        {
            // warning: hd_->size() does not include redline memory
            hd_->release_redline_memory();

            bool recycle_head_bucket = hd_ && (z <= hd_->size());

            this->full_l_.clear();
            this->marked_ = nullptr;
            this->redlined_flag_ = false;

            if (recycle_head_bucket) {
                this->hd_->clear();
                this->total_z_ = hd_->size();

                return true;
            } else {
                this->hd_.reset(nullptr);
                this->total_z_ = 0;

                return this->expand(z);
            }
        }

        bool
        ListAlloc::expand(std::size_t z)
        {
            std::size_t cz = current_z_;
            std::size_t nz = next_z_;
            std::size_t tz;

            do {
                tz = cz + nz;
                cz = nz;
                nz = tz;
            } while (cz < z);

            std::string name = hd_->name() + "+exp";

            std::unique_ptr<ArenaAlloc> new_alloc = ArenaAlloc::make(name, 0, cz, debug_flag_);

            if (!new_alloc)
                return false;

            this->current_z_ = cz;
            this->next_z_ = nz;
            this->total_z_ += cz;

            this->hd_ = std::move(new_alloc);

            return true;
        }

        void
        ListAlloc::checkpoint() {
            hd_->checkpoint();

            this->marked_ = hd_.get();
        }

        std::byte *
        ListAlloc::alloc(std::size_t z) {
            std::byte * retval = hd_->alloc(z);

            if (retval)
                return retval;

            if (this->expand(z))
                return hd_->alloc(z);

            return nullptr;
        }

        void
        ListAlloc::release_redline_memory()
        {
            if (use_redline_)
                redlined_flag_ = true;

            this->hd_->release_redline_memory();
        }
    } /*namespace gc*/
} /*namespace xo*/

/* end ListAlloc.cpp */
