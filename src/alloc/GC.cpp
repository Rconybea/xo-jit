/* GC.cpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#include "GC.hpp"
#include "Object.hpp"
#include "xo/indentlog/scope.hpp"
#include <cassert>
#include <cstddef>

namespace xo {
    namespace gc {
        void
        PerGenerationStatistics::include_gc(std::size_t alloc_z,
                                            std::size_t before_z,
                                            std::size_t after_z,
                                            std::size_t promote_z)
        {
            this->update_snapshot(after_z);

            new_alloc_z_ += alloc_z;
            scanned_z_   += before_z;
            survive_z_   += after_z;
            promote_z_   += promote_z;
        }

        void
        PerGenerationStatistics::update_snapshot(std::size_t after_z)
        {
            used_z_ = after_z;
        }

        void
        PerGenerationStatistics::display(std::ostream & os) const
        {
            os << "<PerGenerationStatistics"
               << xtag("used", used_z_)
               << xtag("n_gc", n_gc_)
               << xtag("new_alloc_z", new_alloc_z_)
               << xtag("scanned_z", scanned_z_)
               << xtag("survive_z", survive_z_)
               << xtag("promote_z", promote_z_)
               << ">";
        }

        void
        GcStatistics::include_gc(generation upto,
                                 std::size_t alloc_z,
                                 std::size_t before_z,
                                 std::size_t after_z,
                                 std::size_t promote_z)
        {
            gen_v_[static_cast<std::size_t>(upto)].include_gc(alloc_z, before_z, after_z, promote_z);
        }

        void
        GcStatistics::update_snapshot(generation upto,
                                      std::size_t after_z)
        {
            gen_v_[static_cast<std::size_t>(upto)].update_snapshot(after_z);
        }

        void
        GcStatistics::display(std::ostream & os) const
        {
            os << "<GcStatistics"
               << xtag("gen_v", gen_v_)
               << xtag("total_allocated", total_allocated_)
                // << xtag("per_type_stats", per_type_stats_)
               << ">";
        }

        GC::GC(const Config & config)
            : config_{config}
        {
            enum { NurseryFrom, NurseryTo, TenuredFrom, TenuredTo };

            std::size_t nursery_size = config.initial_nursery_z_;
            std::size_t tenured_size = config.initial_tenured_z_;

            nursery_[role2int(role::from_space)]
                = ListAlloc::make("NA", nursery_size, 2 * nursery_size, config.debug_flag_);
            nursery_[role2int(role::to_space)  ]
                = ListAlloc::make("NB", nursery_size, 2 * nursery_size, config.debug_flag_);

            tenured_[role2int(role::from_space)]
                = ListAlloc::make("TA", tenured_size, 2 * tenured_size, config.debug_flag_);
            tenured_[role2int(role::to_space)  ]
                = ListAlloc::make("TB", tenured_size, 2 * tenured_size, config.debug_flag_);

            this->checkpoint();
        }

        up<GC>
        GC::make(const Config & config)
        {
            GC * gc = new GC(config);

            return up<GC>{gc};
        }

        std::size_t
        GC::size() const
        {
            return nursery_[role2int(role::to_space)]->size() + tenured_[role2int(role::to_space)]->size();
        }

        std::size_t
        GC::allocated() const
        {
            return (nursery_[role2int(role::to_space)]->allocated()
                    + tenured_[role2int(role::to_space)]->allocated());
        }

        std::size_t
        GC::available() const
        {
            return nursery_[role2int(role::to_space)]->available();
        }

        bool
        GC::fromspace_contains(const void * x) const
        {
            return (nursery_[role2int(role::from_space)]->contains(x)
                    || tenured_[role2int(role::from_space)]->contains(x));
        }

        bool
        GC::contains(const void * x) const
        {
            return (nursery_[role2int(role::to_space)]->contains(x)
                    || tenured_[role2int(role::to_space)]->contains(x));
        }

        bool
        GC::is_before_checkpoint(const void * x) const
        {
            return nursery_[role2int(role::to_space)]->is_before_checkpoint(x);
        }

        std::size_t
        GC::before_checkpoint() const
        {
            return nursery_[role2int(role::to_space)]->before_checkpoint();
        }

        std::size_t
        GC::after_checkpoint() const
        {
            return nursery_[role2int(role::to_space)]->after_checkpoint();
        }

        generation
        GC::fromspace_generation_of(const void * x) const
        {
            if (tenured_[role2int(role::from_space)]->contains(x))
                return generation::tenured;

            return generation::nursery;
        }

        generation
        GC::generation_of(const void * x) const
        {
            if (tenured_[role2int(role::to_space)]->contains(x))
                return generation::tenured;

            return generation::nursery;
        }

        std::byte *
        GC::free_ptr(generation gen)
        {
            switch(gen) {
            case generation::nursery:
                return nursery_[role2int(role::to_space)]->free_ptr();
            case generation::tenured:
                return tenured_[role2int(role::to_space)]->free_ptr();
            case generation::N:
                assert(false);
            }

            return nullptr;
        }

        void
        GC::clear()
        {
            nursery_[role2int(role::from_space)]->clear();
            nursery_[role2int(role::to_space)  ]->clear();

            tenured_[role2int(role::from_space)]->clear();
            tenured_[role2int(role::to_space)  ]->clear();
        }

        void
        GC::add_gc_root(Object ** addr)
        {
            gc_root_v_.push_back(addr);
        }

        void
        GC::checkpoint()
        {
            nursery_[role2int(role::to_space)  ]->checkpoint();
        }

        std::byte *
        GC::alloc(std::size_t z)
        {
            std::byte * x = nursery_[role2int(role::to_space)]->alloc(z);

            if (!x) {
                this->request_gc(generation::nursery);

                if (incr_gc_pending_ || full_gc_pending_)
                    nursery_[role2int(role::to_space)]->release_redline_memory();

                /* try (just once) more, maybe request fits in redline space */
                x = nursery_[role2int(role::to_space)]->alloc(z);

                assert(x);
            }

            return x;
        }

        std::byte *
        GC::alloc_gc_copy(std::size_t z, const void * src)
        {
            scope log(XO_DEBUG(config_.debug_flag_), xtag("z", z), xtag("+pad", IAlloc::alloc_padding(z)));

            generation g = this->fromspace_generation_of(src);

            std::byte * retval = nullptr;

            if (g == generation::tenured)
            {
                log && log("tenured");

                retval = tenured_[role2int(role::to_space)]->alloc(z);
            } else if (nursery_[role2int(role::from_space)]->is_before_checkpoint(src))
            {
                log && log("promote");

                /* nursery object has survived 2nd collection cycle
                 *  -> promote into tenured generation
                 */
                retval = tenured_[role2int(role::to_space)]->alloc(z);

                this->gc_statistics_.total_promoted_ += IAlloc::with_padding(z);
            } else {
                log && log("nursery");

                retval = nursery_[role2int(role::to_space)]->alloc(z);

                if (!retval) {
                    /* nursery space exhausted */

                    this->request_gc(generation::nursery);

                    nursery_[role2int(role::to_space)]->release_redline_memory();

                    retval = nursery_[role2int(role::to_space)]->alloc(z);
                }
            }

            assert(retval);

            return retval;
        }

        void
        GC::release_redline_memory()
        {
            // not supported feature for GC
        }

        void
        GC::swap_nursery()
        {
            up<ListAlloc> tmp = std::move(nursery_[role2int(role::to_space)]);
            nursery_[role2int(role::to_space)] = std::move(nursery_[role2int(role::from_space)]);
            nursery_[role2int(role::from_space)] = std::move(tmp);
        }

        void
        GC::swap_tenured()
        {
            up<ListAlloc> tmp = std::move(tenured_[role2int(role::to_space)]);
            tenured_[role2int(role::to_space)] = std::move(tenured_[role2int(role::from_space)]);
            tenured_[role2int(role::from_space)] = std::move(tmp);
        }

        void
        GC::swap_spaces(generation target)
        {
            // will be copying into storage currently labelled FromSpace

            /* gc will copy some to-be-determined amount in [0..promote_z]
               from nursery->tenured generation.
             */
            std::size_t promote_z = nursery_[role2int(role::to_space)]->before_checkpoint();
            if (target == generation::tenured) {
                /* gc on tenured generation may need this much space */
                std::size_t tenured_z = (tenured_[role2int(role::to_space)]->allocated()
                                         + promote_z
                                         + full_gc_threshold_);

                tenured_[role2int(role::from_space)]->reset(tenured_z);

                this->swap_tenured();
            } else {
                if (tenured_[role2int(role::to_space)]->available() < promote_z) {
                    tenured_[role2int(role::to_space)]->expand(promote_z);
                }
            }

            nursery_[role2int(role::from_space)]->reset(nursery_[role2int(role::to_space)]->allocated()
                                             - promote_z
                                             + incr_gc_threshold_);
            this->swap_nursery();
        } /*swap_spaces*/

        void
        GC::copy_object(Object ** pp_object, generation upto, ObjectStatistics * object_stats)
        {
            void * object_address = *pp_object;

            if (nursery_[role2int(role::to_space)]->contains(object_address)
                || ((upto == generation::tenured)
                    && tenured_[role2int(role::to_space)]->contains(object_address)))
            {
                /* global is already in to-space */
                ;
            } else if((upto == generation::nursery) && tenured_[role2int(role::to_space)]->contains(object_address))
            {
                /* skip tenured objects when incremental collection */
                ;
            } else {
                *pp_object = Object::_deep_move(*pp_object, this, object_stats);
            }
        }

        void
        GC::copy_globals(generation upto)
        {
            for (Object ** pp_root : gc_root_v_) {
                this->copy_object(pp_root, upto, &gc_statistics_.per_type_stats_);
            }
        }

        void
        GC::cleanup_phase(generation upto)
        {
            scope log(XO_DEBUG(config_.debug_flag_));

            std::size_t N_allocated = nursery_[role2int(role::from_space)]->after_checkpoint();
            std::size_t T_allocated = tenured_[role2int(role::from_space)]->after_checkpoint();

            std::size_t N_before_gc = nursery_[role2int(role::from_space)]->allocated();
            std::size_t T_before_gc = tenured_[role2int(role::from_space)]->allocated();

            std::size_t N_after_gc = nursery_[role2int(role::to_space)]->allocated();
            std::size_t T_after_gc = tenured_[role2int(role::to_space)]->allocated();
            //std::byte * N_free_ptr = nursery_[role2int(role::to_space)]->free_ptr();

            std::size_t promote_z = gc_statistics_.total_promoted_ - gc_statistics_.total_promoted_sab_;

            this->nursery_[role2int(role::from_space)]->reset(0);
            this->tenured_[role2int(role::from_space)]->reset(0);

            /* objects currenty in to-space nursery have survived one collection */
            this->nursery_[role2int(role::to_space)]->checkpoint();

            // nursery_[role2int(role::to_space)]->set_redline(nursery_[role2int(role::to_space)]->allocated() + incr_gc_threshold_)

            if (upto == generation::tenured)
                this->tenured_[role2int(role::to_space)]->checkpoint();

            if (log) {
                log(xtag("N_allocated", N_allocated));
                log(xtag("N_before_gc", N_before_gc));
                log(xtag("N_after_gc", N_after_gc));
                log(xtag("T_allocated", T_allocated));
                log(xtag("T_before_gc", T_before_gc));
                log(xtag("T_after_gc", T_after_gc));
            }

            this->incr_gc_pending_ = false;
            this->gc_statistics_.include_gc(generation::nursery, N_allocated, N_before_gc, N_after_gc, promote_z);

            if (upto == generation::tenured) {
                this->full_gc_pending_ = false;
                this->gc_statistics_.include_gc(generation::tenured, T_allocated, T_before_gc, T_after_gc, 0);
            } else {
                // still want to update tenured stats for current alloc size
                this->gc_statistics_.update_snapshot(generation::tenured, T_after_gc);
            }
        }

        void
        GC::execute_gc(generation target)
        {
            scope log(XO_DEBUG(config_.debug_flag_));

            bool full_move = (target == generation::tenured);

            // TODO: RAII version in case of exceptions
            this->runstate_ = GCRunstate(true /*in_progress*/, full_move);

            log && log("step 0: snapshot alloc stats");

            /* new allocation since last GC */
            std::size_t new_alloc = this->after_checkpoint();

            ++(gc_statistics_.gen_v_[static_cast<std::size_t>(target)].n_gc_);
            gc_statistics_.total_allocated_ += new_alloc;
            gc_statistics_.total_promoted_sab_ = gc_statistics_.total_promoted_;

            log && log(xtag("new_alloc", new_alloc));

            log && log("step 1:  swap to/from roles");

            this->swap_spaces(target);

            log && log("step 2a: copy globals");

            this->copy_globals(target);

            log && log("step 2b: TODO: copy pinned");

            log && log("step 3:  TODO: forward mutation log");

            log && log("step 4:  TODO: notify destructor log");

            log && log("step 5:  TODO: keep reachable weak pointers");

            log && log("step 6:  cleanup");

            this->cleanup_phase(target);

            this->runstate_ = GCRunstate();

            log && log("statistics:");
            log && log(gc_statistics_);
        }

        void
        GC::request_gc(generation target)
        {
            if (!runstate_.in_progress() && (gc_enabled_ == 0)) {
                if (!config_.allow_incremental_gc_)
                    target = generation::tenured;

                if ((target == generation::nursery)
                    && (tenured_[role2int(role::to_space)]->after_checkpoint() > full_gc_threshold_))
                {
                    /** full collection when >= @ref full_gc_threshold_ bytes added to tenured
                     *  generation, since last full collection
                     **/
                    target = generation::tenured;
                }

                this->execute_gc(target);
            } else {
                this->incr_gc_pending_ = true;
                if (target == generation::tenured)
                    this->full_gc_pending_ = true;
            }
        }

        void
        GC::disable_gc() {
            --gc_enabled_;
        }

        void
        GC::enable_gc() {
            ++gc_enabled_;

            if (gc_enabled_ == 0) {
                /* unblock gc */
                if (incr_gc_pending_)
                    this->request_gc(full_gc_pending_ ? generation::tenured : generation::nursery);
            }
        }
    } /*namespace gc*/
} /*namespace xo*/

/* end GC.cpp */
