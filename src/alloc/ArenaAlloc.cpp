/* file ArenaAlloc.cpp
 *
 * author: Roland Conybeare
 */

#include "ArenaAlloc.hpp"
#include "Object.hpp"
#include "ObjectStatistics.hpp"
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

        void
        ArenaAlloc::capture_object_statistics(capture_phase phase,
                                              ObjectStatistics * p_dest) const
        {
            using xo::reflect::TaggedPtr;

            std::byte * p = lo_;

            while (p < free_ptr_) {
                Object * obj = reinterpret_cast<Object *>(p);
                TaggedPtr tp = obj->self_tp();
                std::size_t z = obj->_shallow_size();
                std::uint32_t id = tp.td()->id().id();

                if (p_dest->per_type_stats_v_.size() < id + 1)
                    p_dest->per_type_stats_v_.resize(id + 1);

                PerObjectTypeStatistics & dest = p_dest->per_type_stats_v_.at(id);

                dest.td_ = tp.td();
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
