/* file ArenaAlloc.cpp
 *
 * author: Roland Conybeare
 */

#include "ArenaAlloc.hpp"
#include "xo/indentlog/scope.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <cassert>

namespace xo {
    namespace gc {
        ArenaAlloc::ArenaAlloc(const std::string & name, std::size_t rz, std::size_t z, bool debug_flag)
        {
            this->name_       = name;
            this->lo_         = (new std::byte [rz + z]);
            this->checkpoint_ = lo_;
            this->free_ptr_   = lo_;
            this->limit_      = lo_ + z;
            this->redline_z_  = rz;
            this->hi_         = limit_ + rz;
            this->debug_flag_ = debug_flag;

            if (!lo_) {
                throw std::runtime_error(tostr("ArenaAlloc: allocation failed",
                                               xtag("size", rz + z)));
            }
        }

        ArenaAlloc::~ArenaAlloc()
        {
            delete [] this->lo_;

            // hygiene..

            this->lo_ = nullptr;
            this->checkpoint_ = nullptr;
            this->free_ptr_ = nullptr;
            this->limit_ = nullptr;
            this->redline_z_ = 0;
            this->hi_ = nullptr;
            this->debug_flag_ = false;
        }

        up<ArenaAlloc>
        ArenaAlloc::make(const std::string & name, std::size_t rz, std::size_t z, bool debug_flag)
        {
            return up<ArenaAlloc>(new ArenaAlloc(name, rz, z, debug_flag));
        }

        void
        ArenaAlloc::set_free_ptr(std::byte * x)
        {
            assert(lo_ <= x);
            assert(x < limit_);

            if (lo_ <= x && x < limit_) {
                this->free_ptr_ = x;
                if (this->checkpoint_ > free_ptr_)
                    this->checkpoint_ = free_ptr_;
            } else {
                throw std::runtime_error(tostr("LinearAllog::set_free_ptr(x): expected lo <= x < limit",
                                               xtag("lo", lo_), xtag("x", x), xtag("limit", limit_)));
            }
        }

        std::size_t
        ArenaAlloc::size() const {
            return limit_ - lo_;
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

        void
        ArenaAlloc::clear()
        {
            this->set_free_ptr(lo_);
            this->limit_ = hi_ - redline_z_;
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

            std::byte * retval = this->free_ptr_;

            log && log(xtag("self", name_), xtag("z0", z0), xtag("+pad", dz), xtag("z1", z1));

            if (free_ptr_ + z1 > limit_) {
                return nullptr;
            }

            this->free_ptr_ += z1;

            return retval;
        }

        void
        ArenaAlloc::release_redline_memory() {
            this->limit_ = this->hi_;
        }

    } /*namespace gc*/
} /*namespace xo*/


/* end ArenaAlloc.cpp */
