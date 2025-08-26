/* file ArenaAlloc.cpp
 *
 * author: Roland Conybeare
 */

#include "ArenaAlloc.hpp"
#include "Object.hpp"
#include "ObjectStatistics.hpp"
#include "xo/indentlog/scope.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <sys/mman.h>
#include <cassert>

namespace xo {
    namespace gc {
        ArenaAlloc::ArenaAlloc(const std::string & name,
                               std::size_t z, bool debug_flag)
        {
            scope log(XO_DEBUG(debug_flag), xtag("name", name));

            this->name_       = name;
            this->page_z_     = getpagesize();

            // reserve virtual memory

            void * base = mmap(nullptr, z, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

            // could use this as fallback..
            //base         = (new std::byte [z]);

            if (base == MAP_FAILED) {
                throw std::runtime_error(tostr("ArenaAlloc: uncommitted allocation failed",
                                               xtag("size", z)));
            }

            this->lo_          = reinterpret_cast<std::byte *>(base);
            this->committed_z_ = 0;
            this->checkpoint_  = lo_;
            this->free_ptr_    = lo_;
            this->limit_       = lo_ + z;
            this->hi_          = limit_;
            this->debug_flag_  = debug_flag;

            if (!lo_) {
                throw std::runtime_error(tostr("ArenaAlloc: allocation failed",
                                               xtag("size", z)));
            }

            log && log(xtag("lo", (void*)lo_), xtag("page_z", page_z_));
        }

        ArenaAlloc::~ArenaAlloc()
        {

            // hygiene..

            if (lo_) {
                munmap(lo_, hi_ - lo_);
            }
            // could use this as fallback if not using uncommitted technique
            //delete [] this->lo_;

            this->lo_ = nullptr;
            this->committed_z_ = 0;
            this->checkpoint_ = nullptr;
            this->free_ptr_ = nullptr;
            this->limit_ = nullptr;
            this->hi_ = nullptr;
            this->debug_flag_ = false;
        }

        up<ArenaAlloc>
        ArenaAlloc::make(const std::string & name,
                         std::size_t z, bool debug_flag)
        {
            return up<ArenaAlloc>(new ArenaAlloc(name,
                                                 z, debug_flag));
        }

        namespace {
            /* alignment better be a power of 2 */
            std::size_t
            align_lub(std::size_t x, std::size_t align)
            {
                /* e.g:
                 *   align = 4096, x%align = 100 -> dx = 3996
                 *   align = 4096, x%align = 0   -> dx = 0
                 */
                std::size_t dx = (align - (x % align)) % align;

                return x + dx;
            }
        }

        bool
        ArenaAlloc::expand(size_t offset_z) {
            scope log(XO_DEBUG(debug_flag_), xtag("offset_z", offset_z), xtag("committed_z", committed_z_));

            if (offset_z <= committed_z_)
                return true;

            std::size_t align_offset_z = align_lub(offset_z, page_z_);
            std::byte * commit_start = lo_ + committed_z_;
            std::size_t new_commit_z = align_offset_z - committed_z_;

            log && log(xtag("align_offset_z", align_offset_z),
                       xtag("new_commit_z", new_commit_z));

            if (mprotect(commit_start, new_commit_z, PROT_READ | PROT_WRITE) != 0) {
                throw std::runtime_error(tostr("ArenaAlloc::expand: commit failure",
                                               xtag("committed_z", committed_z_),
                                               xtag("new_commit_z", new_commit_z)));
            }

            this->committed_z_ = align_offset_z;
            this->limit_ = this->lo_ + committed_z_;

            return true;
        }

        void
        ArenaAlloc::set_free_ptr(std::byte * x)
        {
            assert(lo_ <= x);
            assert(x < limit_);

            if (lo_ <= x && x < limit_) {
                this->free_ptr_ = x;
                if (checkpoint_ > free_ptr_)
                    this->checkpoint_ = free_ptr_;
            } else {
                throw std::runtime_error(tostr("LinearAllog::set_free_ptr(x): expected lo <= x < limit",
                                               xtag("lo", lo_), xtag("x", x), xtag("limit", limit_)));
            }
        }

        std::pair<bool, std::size_t>
        ArenaAlloc::location_of(const void * x) const
        {
            if ((lo_ <= x) && (x < hi_)) {
                return std::make_pair(true, reinterpret_cast<const std::byte *>(x) - lo_);
            } else {
                return std::make_pair(false, 0);
            }
        }

        void
        ArenaAlloc::reset(std::size_t need_z) {
            this->clear();
            this->expand(need_z);
        }

        void
        ArenaAlloc::capture_object_statistics(capture_phase phase,
                                              ObjectStatistics * p_dest) const
        {
            scope log(XO_DEBUG(debug_flag_),
                      xtag("name", name_),
                      xtag("capacity", limit_ - lo_),
                      xtag("alloc", free_ptr_ - lo_),
                      xtag("lo", (void*)lo_),
                      xtag("free_ptr", (void*)free_ptr_));

            using xo::reflect::TaggedPtr;

            std::byte * p = lo_;

            while (p < free_ptr_) {
                Object *     obj = reinterpret_cast<Object *>(p);
                TaggedPtr     tp = obj->self_tp();
                std::size_t    z = obj->_shallow_size();
                std::uint32_t id = tp.td()->id().id();

                log && log(xtag("obj", (void*)obj),
                           xtag("z", z),
                           xtag("typeid", id));

                if (p_dest->per_type_stats_v_.size() < id + 1)
                    p_dest->per_type_stats_v_.resize(id + 1);

                PerObjectTypeStatistics & dest = p_dest->per_type_stats_v_.at(id);

                dest.td_ = tp.td();

                log && log(xtag("td", tp.td()->short_name()));

                switch (phase) {
                case capture_phase::sab:
                    ++dest.scanned_n_;
                    dest.scanned_z_ += z;
                    break;
                case capture_phase::sae:
                    ++dest.survive_n_;
                    dest.survive_z_ += z;
                    break;
                }

                p += z;
            }

            assert(p == free_ptr_);
        }

        const std::string &
        ArenaAlloc::name() const {
            return name_;
        }

        std::size_t
        ArenaAlloc::size() const {
            return limit_ - lo_;
        }

        std::size_t
        ArenaAlloc::committed() const {
            return committed_z_;
        }

        std::size_t
        ArenaAlloc::available() const {
            return limit_ - free_ptr_;
        }

        std::size_t
        ArenaAlloc::allocated() const {
            return free_ptr_ - lo_;
        }

        bool
        ArenaAlloc::contains(const void * x) const {
            return (lo_ <= x) && (x < hi_);
        }

        bool
        ArenaAlloc::is_before_checkpoint(const void * x) const {
            return (lo_ <= x) && (x < checkpoint_);
        }

        std::size_t
        ArenaAlloc::before_checkpoint() const
        {
            return checkpoint_ - lo_;
        }

        std::size_t
        ArenaAlloc::after_checkpoint() const
        {
            return free_ptr_ - checkpoint_;
        }

        bool
        ArenaAlloc::debug_flag() const
        {
            return debug_flag_;
        }

        void
        ArenaAlloc::clear()
        {
            this->set_free_ptr(lo_);
            this->limit_ = hi_;
        }

        void
        ArenaAlloc::checkpoint()
        {
            this->checkpoint_ = this->free_ptr_;
        }

        std::byte *
        ArenaAlloc::alloc(std::size_t z0)
        {
            scope log(XO_DEBUG(debug_flag_));

            /* word size for alignment */
            constexpr uint32_t c_bpw = sizeof(std::uintptr_t);

            std::uintptr_t free_u64 = reinterpret_cast<std::uintptr_t>(free_ptr_);

            assert(free_u64 % c_bpw == 0ul);

            std::uint32_t dz = alloc_padding(z0);

            std::size_t z1 = z0 + dz;

            assert(z1 % c_bpw == 0ul);

            this->expand(this->allocated() + z1);

            std::byte * retval = this->free_ptr_;

            log && log(xtag("self", name_),
                       xtag("z0", z0),
                       xtag("+pad", dz),
                       xtag("z1", z1),
                       xtag("size", this->size()),
                       xtag("avail", this->available()));

            this->free_ptr_ += z1;

            return retval;
        }

    } /*namespace gc*/
} /*namespace xo*/


/* end ArenaAlloc.cpp */
