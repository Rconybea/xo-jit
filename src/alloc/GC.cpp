/* GC.cpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#include "GcStatistics.hpp"
#include "GC.hpp"
#include "Object.hpp"
#include "xo/indentlog/scope.hpp"
#include <cassert>
#include <cstddef>

namespace xo {
    namespace gc {
        bool
        MutationLogEntry::is_child_forwarded() const
        {
            assert(!parent_->_is_forwarded());

            return (*lhs_)->_is_forwarded();
        }

        bool
        MutationLogEntry::is_parent_forwarded() const
        {
            return parent_->_is_forwarded();
        }

        Object *
        MutationLogEntry::parent_destination() const
        {
            //const bool c_debug_flag = true;
            //scope log(XO_DEBUG(c_debug_flag));

            if (parent_->_is_forwarded()) {
                //log && log("parent is forwarded", xtag("parent", (void*)parent_));

                return parent_->_destination();
            } else {
                //log && log("parent is ordinary", xtag("parent", (void*)parent_));

                return parent_;
            }
        }

        MutationLogEntry
        MutationLogEntry::update_parent_moved(Object * parent_to) const
        {
            std::byte * parent_from = reinterpret_cast<std::byte *>(parent_);
            std::byte * lhs_from = reinterpret_cast<std::byte *>(lhs_);

            std::ptrdiff_t offset = (lhs_from - parent_from);

            std::byte * lhs_to = reinterpret_cast<std::byte *>(parent_to) + offset;

            return MutationLogEntry(parent_to,
                                    reinterpret_cast<Object **>(lhs_to));
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

            mutation_log_[role2int(role::from_space)] = std::make_unique<MutationLog>();
            mutation_log_[role2int(role::to_space)] = std::make_unique<MutationLog>();
            defer_mutation_log_ = std::make_unique<MutationLog>();

            this->checkpoint();
        }

        GC::~GC() {
            /* hygiene */
            this->clear();

            nursery_[role2int(role::from_space)].reset();
            nursery_[role2int(role::to_space)  ].reset();

            tenured_[role2int(role::from_space)].reset();
            tenured_[role2int(role::to_space)  ].reset();

            mutation_log_[role2int(role::from_space)].reset();
            mutation_log_[role2int(role::to_space)  ].reset();
            defer_mutation_log_.reset();
        }

        up<GC>
        GC::make(const Config & config)
        {
            GC * gc = new GC(config);

            return up<GC>{gc};
        }

        const std::string &
        GC::name() const
        {
            static std::string s_default_name = "GC";
            return s_default_name;
        }

        std::size_t
        GC::size() const
        {
            return nursery_to()->size() + tenured_to()->size();
        }

        std::size_t
        GC::committed() const
        {
            return (nursery_to()->committed()
                    + nursery_from()->committed()
                    + tenured_to()->committed()
                    + tenured_from()->committed());
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
            return this->nursery_to()->before_checkpoint();
        }

        std::size_t
        GC::after_checkpoint() const
        {
            return this->nursery_to()->after_checkpoint();
        }

        bool
        GC::debug_flag() const
        {
            return config_.debug_flag_;
        }

        GcStatisticsExt
        GC::get_gc_statistics() const
        {
            GcStatisticsExt retval = GcStatisticsExt(this->native_gc_statistics());

            retval.nursery_z_ = nursery_[role2int(role::to_space)]->size();
            retval.nursery_before_checkpoint_z_ = this->nursery_to()->before_checkpoint();
            retval.nursery_after_checkpoint_z_ = this->nursery_to()->after_checkpoint();
            retval.tenured_z_ = tenured_[role2int(role::to_space)]->size();

            return retval;
        }

        std::size_t
        GC::nursery_to_committed() const
        {
            return nursery_to()->committed();
        }

        std::size_t
        GC::nursery_before_checkpoint() const
        {
            return nursery_to()->before_checkpoint();
        }

        std::size_t
        GC::nursery_after_checkpoint() const
        {
            return nursery_to()->after_checkpoint();
        }

        std::size_t
        GC::tenured_to_committed() const
        {
            return tenured_to()->committed();
        }

        std::size_t
        GC::tenured_before_checkpoint() const
        {
            return tenured_to()->before_checkpoint();
        }

        std::size_t
        GC::tenured_after_checkpoint() const
        {
            return tenured_to()->after_checkpoint();
        }

        generation_result
        GC::tospace_generation_of(const void * x) const
        {
            if (tenured_[role2int(role::to_space)]->contains(x))
                return generation_result::tenured;

            if (nursery_[role2int(role::to_space)]->contains(x))
                return generation_result::nursery;

            return generation_result::not_found;
        }

        generation_result
        GC::fromspace_generation_of(const void * x) const
        {
            if (tenured_[role2int(role::from_space)]->contains(x))
                return generation_result::tenured;

            if (nursery_[role2int(role::from_space)]->contains(x))
                return generation_result::nursery;

            return generation_result::not_found;
        }

        std::byte *
        GC::free_ptr(generation gen)
        {
            switch(gen) {
            case generation::nursery:
                return nursery_[role2int(role::to_space)]->free_ptr();
            case generation::tenured:
                return tenured_[role2int(role::to_space)]->free_ptr();
                // LCOV_EXCL_START
            case generation::N:
                assert(false);
                // LCOV_EXCL_STOP
            }

            return nullptr;
        }

        std::size_t
        GC::mlog_size() const {
            return mutation_log_[role2int(role::to_space)]->size();
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
            nursery_to()->checkpoint();
            /* checkpoint T generation so we can trigger GC based on new T objects rather than
             * overall T size
             */
            tenured_to()->checkpoint();
        }

        std::byte *
        GC::alloc(std::size_t z)
        {
            auto N_to = this->nursery_to();

            if (!incr_gc_pending_ && (N_to->after_checkpoint() > config_.incr_gc_threshold_)) {
                /* automatically ups to generation::tenured */
                this->request_gc(generation::nursery);
            }

            std::byte * x = N_to->alloc(z);

            /* ListAlloc won't fail unless we exhaust memory -- instead will increase heap size */
            assert(x);

            return x;
        }

        std::byte *
        GC::alloc_gc_copy(std::size_t z, const void * src)
        {
            scope log(XO_DEBUG(config_.debug_flag_), xtag("z", z), xtag("+pad", IAlloc::alloc_padding(z)));

            generation_result gr = this->fromspace_generation_of(src);

            std::byte * retval = nullptr;

            switch (gr) {
            case generation_result::tenured:
                {
                    log && log("tenured");

                    retval = this->tenured_to()->alloc(z);
                }
                break;
            case generation_result::nursery:
                {
                    if (this->nursery_from()->is_before_checkpoint(src))
                    {
                        /* nursery object has survived 2nd collection cycle
                         *  -> promote into tenured generation
                         */
                        retval = this->tenured_to()->alloc(z);

                        log && log("promote", xtag("addr", (void*)retval));

                        assert(this->tospace_generation_of(retval) == generation_result::tenured);

                        this->gc_statistics_.total_promoted_ += IAlloc::with_padding(z);
                    } else {
                        log && log("nursery");

                        retval = this->nursery_to()->alloc(z);
                    }
                }
                break;
            case generation_result::not_found:
                /* something wrong -- we only copy objects that are known to be in from-space
                 */

                assert(false);
                break;
            }

            assert(retval);

            return retval;
        }

        void
        GC::assign_member(Object * parent, Object ** lhs, Object * rhs)
        {
            ++gc_statistics_.n_mutation_;

            *lhs = rhs;

            if (runstate_.in_progress()) {
                /* don't log mutations (if any) during GC */
                return;
            }

            if (!config_.allow_incremental_gc_) {
                /* full GCs don't need mutation log, since no cross-generational pointers */
                return;
            }

            switch (tospace_generation_of(rhs))
            {
            case generation_result::tenured:
                /* only need to log mutations that create tenured->nursery pointers */
                return;

            case generation_result::nursery:
                switch (tospace_generation_of(parent)) {
                case generation_result::nursery:
                    if (is_before_checkpoint(parent)) {
                        // N1->N0, so must log
                        this->mutation_log_[role2int(role::to_space)]->push_back(MutationLogEntry(parent, lhs));
                        ++(this->gc_statistics_.n_logged_mutation_);
                        ++(this->gc_statistics_.n_xckp_mutation_);
                    } else {
                        // parent in N0, not an xckp mutation
                        return;
                    }
                    break;
                case generation_result::tenured:
                    // T->N, so must log
                    this->mutation_log_[role2int(role::to_space)]->push_back(MutationLogEntry(parent, lhs));
                    ++(this->gc_statistics_.n_logged_mutation_);
                    ++(this->gc_statistics_.n_xgen_mutation_);
                    break;
                case generation_result::not_found:
                    // parent is global
                    // This may be ok (provided lhs is a gc root)
                    break;
                }
                break;

            case generation_result::not_found:

                // child is global;
                // logging not required
                break;
            }
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
        GC::swap_mutation_log()
        {
            up<MutationLog> tmp = std::move(mutation_log_[role2int(role::to_space)]);
            mutation_log_[role2int(role::to_space)] = std::move(mutation_log_[role2int(role::from_space)]);
            mutation_log_[role2int(role::from_space)] = std::move(tmp);
        }

        void
        GC::swap_spaces(generation target)
        {
            scope log(XO_DEBUG(this->debug_flag()), xtag("upto", target));

            // will be copying into the memory regions currently labelled FromSpace

            /* gc will copy some to-be-determined amount in [0..promote_z]
               from nursery->tenured generation.
             */
            std::size_t max_promote_z = nursery_[role2int(role::to_space)]->before_checkpoint();

            log && log(xtag("max_promote_z", max_promote_z));

            if (target == generation::tenured) {
                /* gc on tenured generation may need this much space */
                std::size_t need_tenured_z = (tenured_[role2int(role::to_space)]->allocated()
                                              + max_promote_z
                                              + config_.full_gc_threshold_);

                log && log("need_tenured_z", need_tenured_z);

                tenured_from()->reset(need_tenured_z);

                this->swap_tenured();
            } else {
                std::size_t avail_tenured_z = tenured_[role2int(role::to_space)]->available();

                log && log(xtag("avail_tenured_z", avail_tenured_z));

                if (avail_tenured_z < max_promote_z) {
                    ListAlloc * tenured_to = this->tenured_to();

                    tenured_to->expand(max_promote_z, tenured_to->name() + "+");
                }
            }

            /* subtracting max_promote_z is correct here, since anything not promoted is garbage */
            std::size_t need_nursery_z = (nursery(role::to_space)->allocated()
                                          - max_promote_z
                                          + config_.incr_gc_threshold_);

            log && log(xtag("need_nursery_z", need_nursery_z));

            /* (from-space is about to become to-space, to receive surviving nursery objects) */
            nursery(role::from_space)->reset(need_nursery_z);

            this->swap_nursery();

            this->swap_mutation_log();

            ListAlloc * N_from = nursery(role::from_space);
            log && log(xtag("nursery.from", N_from->name()), xtag("size", N_from->size()));
            ListAlloc * N_to = nursery(role::to_space);
            log && log(xtag("nursery.to",   N_to->name()), xtag("size", N_to->size()));
            ListAlloc * T_from = tenured(role::from_space);
            log && log(xtag("tenured.from", T_from->name()), xtag("size", T_from->size()));
            ListAlloc * T_to = tenured(role::to_space);
            log && log(xtag("tenured.to", T_to->name()), xtag("size", T_to->size()));

        } /*swap_spaces*/

        void
        GC::capture_object_statistics(generation upto, capture_phase phase)
        {
            /* scan nursery */
            this->nursery_[role2int(role::to_space)]->capture_object_statistics
                (phase,
                 &object_statistics_sab_[gen2int(generation::nursery)]);

            if (upto == generation::tenured) {
                /* scan tenured */
                this->tenured_[role2int(role::to_space)]->capture_object_statistics
                    (phase,
                     &object_statistics_sab_[gen2int(generation::tenured)]);
            }
        }

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
            scope log(XO_DEBUG(config_.debug_flag_),
                      xtag("roots", gc_root_v_.size()));

            for (Object ** pp_root : gc_root_v_) {
                this->copy_object(pp_root, upto,
                                  &object_statistics_sae_[gen2int(upto)]);
            }
        }

        void
        GC::incremental_gc_forward_mlog_phase(MutationLog * from_mlog,
                                              MutationLog * to_mlog,
                                              MutationLog * defer_mlog,
                                              ObjectStatistics * per_type_stats)
        {
            scope log(XO_DEBUG(config_.debug_flag_), xtag("from_mlog.size", from_mlog->size()));

            /* categorize pointers based on combination of {source address, destination address},
             * only care about the generation associated with an address.
             *
             * N0 : nursery(from), before checkpoint
             * N0': nursery(to),   before checkpoint
             * N1 : nursery(from), after  checkpoint
             * N1': nursery(to),   after  checkpoint
             *  T : tenured(to)
             *
             * loc(P): parent region before GC
             * loc(C): child  region before GC
             *
             *     |               |   forwarded  | loc now post  | loc after     |
             *     |               |   already?   | root copy     | action        |
             *     | loc(P) loc(C) |     P      C |     P'     C' |     P'     C' | defer | action
             * ----|---------------+--------------+---------------+---------------+-------+---------------
             * (a) |      T     N0 |    no     no |     T     N0  |     T     N1' |       | C->N1', +mlog
             * (b) |               |          yes |           N1' |           N1' |       | +mlog
             * (c) |      T     N1 |    no     no |     T     N1  |     T      T  |       | C->T, -mlog
             * (d) |               |          yes |     T      T  |     T      T  |       | -mlog
             * (e) |     N1     N0 |    no     no |    N1     N0  |    N1     N0  | P ->C | defer
             * (f) |               |          yes |    N1     N1' |    N1     N1' | P ->C'| defer
             * (g) |               |   yes    yes |     T     N1' |     T     N1' |       | +mlog
             *
             * notes:
             * (a) C survives due to xgen ptr {T -> N0}; after collection have xgen ptr {T -> N1}.
             * (b) C already evac'd; after collection stil have xgen ptr {T -> N1}
             * (c) C survives due to xgen ptr (T -> N1): promote to T, so no longer xgen
             * (d) C already evac'd: after collection no longer xgen (T -> T)
             * (e) P,C maybe garbage. don't move either, but defer mlog incase P saved by a subsequent mutation.
             *     in that case C saved alto, + will still have an xgen ptr, so still need an mlog entry
             * (f) P maybe garbage, C survives. defer mlog incase P saved+promoted by a subsequent mutation;
             *     in that case will still have an xgen (T -> N) ptr, so still need an mlog entry.
             */

            std::size_t i_from = 0;
            // number of rescued subgraphs via mutation log entries
            std::size_t n_rescue = 0;

            for (MutationLogEntry & from_entry : *from_mlog)
            {
                if (log) {
                    if (i_from % 10000 == 0)
                        log(xtag("i_from", i_from));
                }

                void * parent = from_entry.parent();

                if (tospace_generation_of(parent) == generation_result::tenured)
                {
                    // cases (a)(b)(c)(d)
                    // loc(P) is T. T didn't move b/c incremental gc.

                    if (from_entry.is_dead()) {
                        // obsolete mutation -- no longer belongs to parent, discard
                    } else {
                        // note: child obtained (as it must be) by reading from parent's memory _now_.
                        Object * child_from = from_entry.child();

                        if (child_from) {
                            if (!child_from->_is_forwarded()) {
                                // P->C*.
                                // either:
                                // - C*=C  in from-space, so needs evac
                                // - C*=C' in to-space, P already updated b/c of another mutation
                                //
                                if (fromspace_generation_of(child_from) != generation_result::not_found) {
                                    // C*=C in from-space. needs evac, along with reachable descendants
                                    //
                                    // Includes cases:
                                    // (a) T->N0
                                    // (c) T->N1

                                    ++n_rescue;

                                    Object::_deep_move(child_from, this, per_type_stats);

                                    // C forwards to C', fall thru to parent fixup below
                                    // (a) T->N1'
                                    // (c) T->T
                                } else {
                                    // P updated via some other mutation
                                    // so don't need this mlog
                                    ;
                                }
                            }

                            // re-test, state may have changed above
                            if (from_entry.is_child_forwarded()) {
                                // P->C, C moved to C'
                                // Includes cases (a),(c) from above

                                Object * child_to = child_from->_destination();

                                from_entry.fixup_parent_child_moved(child_to);

                                // P->C', loc(C') in {N1', T'}

                                if (tospace_generation_of(child_to) == generation_result::nursery) {
                                    // (b) loc(P)=T, loc(C')=N1';  also case (a)

                                    // still have xgen pointer, so need mlog for it
                                    to_mlog->push_back(from_entry);
                                } else {
                                    // (d) loc(P)=T, loc(C')=T;  also case (c)
                                    // no longer xgen, so does not require mlog
                                }
                            }

                        } else {
                            // nullptr child, discard
                        }
                    }
                } else if (from_entry.is_parent_forwarded()) {
                    // Must have:
                    // loc(P) = N1, because:
                    //    loc(P)=N0 -> ineligible for mlog;
                    //    loc(P)=T  -> not moved on incr GC
                    //
                    // follows that loc(P') = T
                    // already have P'->C' when parent moved separately

                    Object * parent_to = from_entry.parent_destination();

                    log && log(xtag("parent_to", (void*)parent_to));

                    assert(tospace_generation_of(parent_to) == generation_result::tenured);

                    MutationLogEntry to_entry = from_entry.update_parent_moved(parent_to);

                    Object * child_to = to_entry.child(); // after moving

                    if (tospace_generation_of(child_to) == generation_result::nursery) {
                        if (to_entry.is_dead()) {
                            ;
                        } else {
                            // (g) loc(P)=N1, loc(C)=N0, loc(P')=T, loc(C')=N1
                            to_mlog->push_back(to_entry);
                        }

                    }
                } else {
                    // loc(P) = N1, loc(C) = N0, P may be garbage
                    // Includes cases:
                    // (e) P->C, C not moved
                    // (f) P->C, C moved to C'
                    //
                    // P may yet be rescued by another mlog entry, so defer

                    if (!from_entry.is_dead()) {
                        defer_mlog->push_back(from_entry);
                    }
                }

                ++i_from;
            }

            from_mlog->clear();

            if (n_rescue == 0) {
                // if we didn't rescue any objects
                // then we now confirm that otherwise-unreachable parents in defer_mlog
                // are garbage

                defer_mlog->clear();
            }
        }

        void
        GC::full_gc_forward_mlog_phase(MutationLog * from_mlog,
                                       MutationLog * to_mlog,
                                       MutationLog * defer_mlog,
                                       ObjectStatistics * /*per_type_stats*/)
        {
            scope log(XO_DEBUG(config_.debug_flag_), xtag("from_mlog.size", from_mlog->size()));

            /* categorize pointers based on combination of {source address, destination address},
             * only care about the generation associated with an address.
             *
             * N0 : nursery(from), before checkpoint
             * N0': nursery(to),   before checkpoint
             * N1 : nursery(from), after  checkpoint
             * N1': nursery(to),   after  checkpoint
             *  T : tenured(from)
             *  T': tenured(to)
             *
             * loc(P): parent region before GC
             * loc(C): child  region before GC
             *
             *     |               |   forwarded  | loc now post  | loc after     |
             *     |               |   already?   | root copy     | action        |
             *     | loc(P) loc(C) |     P      C |     P'     C' |     P'     C' | defer | action
             * ----|---------------+--------------+---------------+---------------+-------+---------------
             * (a) |      T     N0 |    no     no |     T     N0  |     T     N0  | P ->C | defer
             * (b) |               |          yes |           N1' |           N1' | P ->C'| defer
             *     |               |   yes     no | impossible
             * (b2)|               |          yes |     T'    N1' |     T'    N1' |       | +mlog
             * (c) |      T     N1 |    no     no |     T     N1  |     T      T  | P ->C | defer
             * (d) |               |          yes |     T      T' |     T      T' | P ->C'| defer
             *     |               |   yes     no | impossible
             * (d2)|               |          yes |     T'     T' |     T'     T' |       | -mlog
             * (e) |     N1     N0 |    no     no |    N1     N0  |    N1     N0  | P ->C | defer
             * (f) |               |          yes |    N1     N1' |    N1     N1' | P ->C'| defer
             *     |               |   yes     no | impossible
             * (g) |               |   yes    yes |     T'    N1' |     T'    N1' |       | +mlog
             *
             * notes:
             * (a)  P,C maybe garbage. don't move either, but defer mlog incase P saved by a subsequent mutation;
             *      in that case C saved also, + will still have an xgen ptr, and still need an mlog entry.
             * (b)  C already evac'd, but P maybe garbage. defer mlog incase P rescued by a subsequent mutation;
             *      in that case will still have an xgen (T -> N) ptre, and still need an mlog entry.
             * (b2) P,C already evac'd.  Must update+rembexember xgen ptr {T -> N1}
             * (c)  P,C maybe garbage. don't move either, but defer mlog in case P saved by a subsequent mutation;
             *      in that case C promoted, no longer xgen
             * (d)  P maybe garbage. defer in case P saved by a subsequent mutation.
             *      C now tenured, so will no longer have an xgen pointer.
             * (d2) P,C already evac'd.  After collection no longer have xgen pointer, so no mlog.
             * (e) P,C maybe garbage. don't move either, but defer mlog incase P saved by a subsequent mutation.
             *     in that case C saved alto, + will still have an xgen ptr, so still need an mlog entry
             * (f) P maybe garbage, C survives. defer mlog incase P saved+promoted by a subsequent mutation;
             *     in that case will still have an xgen (T -> N) ptr, so still need an mlog entry.
             * (g) P,C already evac'd. Still have xgen pointer, must mlog
             */

            std::size_t i_from = 0;
            // number of rescued subgraphs via mutation log entries
            std::size_t n_rescue = 0;

            for (MutationLogEntry & from_entry : *from_mlog)
            {
                log && (i_from % 10000 == 0) && log(xtag("i_from", i_from));

                if (from_entry.is_parent_forwarded()) {
                    Object * parent_to = from_entry.parent_destination();

                    log && log(xtag("parent_to", (void*)parent_to));

                    assert(tospace_generation_of(parent_to) == generation_result::tenured);

                    MutationLogEntry to_entry = from_entry.update_parent_moved(parent_to);

                    // note: child obtained (as it must be) by reading from prarent's memory _now_.
                    //       Since parent has moved, child has too
                    Object * child_to = to_entry.child(); // after moveing

                    if (tospace_generation_of(parent_to) == generation_result::tenured)
                    {
                        // cases (b2)(d2)(g), loc(P) is T'
                        // In all these cases parent has already been moved;
                        // therefore child has also been moved.
                        // Just need to decide whether to keep mlog entry

                        if (from_entry.is_dead()) {
                            // obsolete mutation -- no longer belongs to parent, discard
                        } else if (child_to) {
                            assert(!child_to->_is_forwarded());

                            if (tospace_generation_of(child_to) == generation_result::nursery) {
                                // case
                                //   (b2) loc(P')=T', loc(C')=N1' --> +mlog
                                //   (g)  loc(P')=T', loc(C')=N1' --> +mlog
                                //
                                to_mlog->push_back(to_entry);
                            } else {
                                // case
                                //   (d2) loc(P')=T', loc(C')=T' --> -mlog
                            }
                        }
                    } else {
                        // impossible - wouldn't have made mlog entry


                        assert(false);
                    }
                } else {
                    // case
                    //   (a) defer
                    //   (b) defer
                    //   (c) defer
                    //   (d) defer
                    //   (e) defer
                    //   (f) defer

                    defer_mlog->push_back(from_entry);
                }

                ++i_from;
            }

            from_mlog->clear();

            if (n_rescue == 0) {
                // if we didn't rescue any objects
                // then we now confirm that otherwise-unreachable parents in defer_mlog
                // are garbage

                defer_mlog->clear();
            }
        }


        void
        GC::incremental_gc_forward_mlog(ObjectStatistics * per_type_stats)
        {
            /* control here:
             * - incremental gc.
             * - gc roots have been copied, along with everything reachable from them.
             *
             * plan:
             * - forward mutations in *from_mutation_log, writing them to
             *   *to_mutationlog and/or *defer_mutation_log.
             *   Use defer when mutation P->C encountered, but P was not copied.
             *   P appears to be garbage, but may turn out to be live if encountered
             *   in another mutation.
             *
             */

            MutationLog *   to_mlog = mutation_log_[role2int(role::to_space)].get();

            for (;;) {
                MutationLog * from_mlog = mutation_log_[role2int(role::from_space)].get();
                MutationLog * defer_mlog = defer_mutation_log_.get();

                this->incremental_gc_forward_mlog_phase(from_mlog,
                                                        to_mlog,
                                                        defer_mlog,
                                                        per_type_stats);

                assert(from_mlog->empty());

                if (defer_mlog->empty()) {
                    /* fixpoint reached */
                    break;
                }

                /* control here:
                 * 1. at least one mlog triggered a rescue
                 * 2. at least one mlog was deferred (b/c otherwise-unreachable parent)
                 *
                 * it's conceivable deferred parent now reachable thanks to rescues;
                 * revisit entries in defer_mlog,
                 *
                 * using now-empty from_mlog as scratch for any remaining deferred entries
                 */

                std::swap(mutation_log_[role2int(role::from_space)], defer_mutation_log_);
            }
        }

        void
        GC::full_gc_forward_mlog(ObjectStatistics * per_type_stats)
        {
            /* control here:
             * - full gc.
             * - gc roots have been copied, along with everything reachable
             *   from them.
             *
             * plan:
             * - forward mutations in *from_mutation_log, writing them to
             *   *to_mutation_log and/or *defer_mutation_log.
             */

            MutationLog * to_mlog = this->mutation_log(role::to_space);

            for (;;) {
                MutationLog * from_mlog = this->mutation_log(role::from_space);
                MutationLog * defer_mlog = defer_mutation_log_.get();

                this->full_gc_forward_mlog_phase(from_mlog,
                                                 to_mlog,
                                                 defer_mlog,
                                                 per_type_stats);
                assert(from_mlog->empty());

                if (defer_mlog->empty())
                    break;

                /* control here:
                 * 1. at least one mlog triggered a rescue
                 * 2. at least one mlog was deferred (had otherwise-unreachable parent)
                 *
                 * possible that deferred parent is now reachable thanks to a rescue;
                 * to confirm/refute this need to revisit entries in defer_mlog.
                 */
                std::swap(mutation_log_[role2int(role::from_space)], defer_mutation_log_);
            }
        }

        void
        GC::forward_mutation_log(generation upto)
        {
            scope log(XO_DEBUG(config_.debug_flag_));

            if (upto == generation::tenured) {
                this->full_gc_forward_mlog(&object_statistics_sae_[gen2int(generation::tenured)]);
            } else {
                this->incremental_gc_forward_mlog(&object_statistics_sae_[gen2int(generation::nursery)]);
            }
        }

        void
        GC::cleanup_phase(generation upto)
        {
            scope log(XO_DEBUG(config_.debug_flag_));

            std::size_t N_allocated = nursery_from()->after_checkpoint();
            std::size_t T_allocated = tenured_from()->after_checkpoint();

            std::size_t N_before_gc = nursery_from()->allocated();
            std::size_t T_before_gc = tenured_from()->allocated();

            std::size_t N_after_gc = nursery_to()->allocated();
            std::size_t T_after_gc = tenured_to()->allocated();
            //std::byte * N_free_ptr = nursery_[role2int(role::to_space)]->free_ptr();

            std::size_t promote_z = (gc_statistics_.total_promoted_
                                     - gc_statistics_.total_promoted_sab_);

            /* Don't reset from-space here, it's unnecessary.
             * Would be permissible, but interferes with GC object modelling in
             * xo-object/utest/GC.test.cpp
             */
            //this->nursery_[role2int(role::from_space)]->reset(0);
            //this->tenured_[role2int(role::from_space)]->reset(0);

            /* objects currenty in to-space nursery have survived one collection */
            this->nursery_to()->checkpoint();
            if (upto == generation::tenured)
                this->tenured_to()->checkpoint();

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
        GC::execute_gc(generation upto)
        {
            scope log(XO_DEBUG(config_.stats_flag_));

            bool full_move = (upto == generation::tenured);

            // TODO: RAII version in case of exceptions
            this->runstate_ = GCRunstate(true /*in_progress*/, full_move);

            log && log("step 0: snapshot alloc stats");

            /* new allocation since last GC */
            std::size_t new_alloc = this->after_checkpoint();

            ++(gc_statistics_.gen_v_[static_cast<std::size_t>(upto)].n_gc_);
            gc_statistics_.total_allocated_ += new_alloc;
            gc_statistics_.total_promoted_sab_ = gc_statistics_.total_promoted_;

            log && log(xtag("new_alloc", new_alloc));

            log && log("step 0: (optional) scan for object statistics");

            this->capture_object_statistics(upto, capture_phase::sab);

            log && log("step 1 : swap to/from roles");

            this->swap_spaces(upto);

            log && log("step 2a: copy globals");

            this->copy_globals(upto);

            log && log("step 2b: TODO: copy pinned");

            log && log("step 3 : forward mutation log");

            this->forward_mutation_log(upto);

            log && log("step 4 : TODO: notify destructor log");

            log && log("step 5 : TODO: keep reachable weak pointers");

            log && log("step 6 : cleanup");

            this->cleanup_phase(upto);

            this->capture_object_statistics(upto, capture_phase::sae);

            log && log("object statistics [nursery]:");
            log && log(refrtag("stats", object_statistics_sab_[gen2int(generation::nursery)]));
            log && log("object statistics [tenured]:");
            log && log(refrtag("stats", object_statistics_sab_[gen2int(generation::tenured)]));

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
                    && (this->tenured_to()->after_checkpoint() > config_.full_gc_threshold_))
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
