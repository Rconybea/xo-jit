/* GC.hpp
 *
 * author: Roland Conybeare, jul 2025
 */

#pragma once

#include "ArenaAlloc.hpp"
#include "GcStatistics.hpp"
#include "Object.hpp"
#include "xo/callback/UpCallbackSet.hpp"
#include "xo/indentlog/print/array.hpp"
#include <vector>
#include <array>

namespace xo {
    /** types that can participate in GC inherit from this base class.  See Object.hpp in this directory **/
    class Object;

    namespace gc {
        enum class role {
            /** nursery: generation for new objects **/
            from_space,
            /** tenured: generation for objects that have survived two collections **/
            to_space,
            N,
        };

        constexpr std::size_t role2int(role x) { return static_cast<std::size_t>(x); }

        /** @class Config
         *  @brief garbage collector configuration
         **/
        struct Config {
            /** initial size in bytes for youngest (Nursery) generation.
             *  GC allocates two nursery spaces of this size.
             *  This number represents reserved address space.
             *  pages are committed on demand.
             *  Initial committment will be up to @ref incr_gc_threshold_
             **/
            std::size_t initial_nursery_z_ = 64*1024*1024;
            /** initial size in bytes for oldest (Tenured) generation.
             *  GC allocates two tenured spaces of this size.
             *  This number represents reserved address space.
             *  pages are committed on demand.
             *  Initial committment will be up to @ref full_gc_threshold_
             **/
            std::size_t initial_tenured_z_ = 128*1024*1024;
            /** trigger incremental GC after this many bytes allocated in nursery **/
            std::size_t incr_gc_threshold_ = 64*1024;
            /** trigger full GC after this many bytes promoted to tenured **/
            std::size_t full_gc_threshold_ = 512*1024;

            /** true to permit incremental garbage collection **/
            bool allow_incremental_gc_ = true;
            /** true to report statistics **/
            bool stats_flag_ = false;
            /** true to capture per-type object statistics **/
            bool object_stats_flag_ = false;
            /** remember basic gc statistics for this many GC's; separately for incremental + full GCs **/
            std::size_t stats_history_z_ = 256;
            /** true to enable debug logging **/
            bool debug_flag_ = false;
        };

        /** @class GCRunstate
         *  @brief encapsulate state needed while GC is running
         *
         *  state pertaining to a single GC invocation.
         *  We stash an instance of this in @ref GC as context,
         *  so that per-Object-derived-type auxiliary functions can be slightly streamlined
         **/
        class GCRunstate {
        public:
            GCRunstate() = default;
            explicit GCRunstate(bool in_progress, bool full_move)
                : in_progress_{in_progress}, full_move_{full_move} {}

            bool in_progress() const { return in_progress_; }
            bool full_move() const { return full_move_; }

        private:
            /** true when GC begins; remains true until GC cycle complete **/
            bool in_progress_ = false;
            /** true for full GC; false for incremental GC **/
            bool full_move_ = false;
        };

        class MutationLogEntry {
        public:
            MutationLogEntry(IObject * parent, IObject ** lhs)
                : parent_{parent}, lhs_{lhs} {}

            IObject * parent() const { return parent_; }
            IObject ** lhs() const { return lhs_; }

            IObject * child() const { return *lhs_; }

            bool is_child_forwarded() const;
            bool is_parent_forwarded() const;

            IObject * parent_destination() const;

            /** Flag obsolete mutation.
             *  Future proofing, never happens for regular objects
             **/
            bool is_dead() const { return false; }

            MutationLogEntry update_parent_moved(IObject * parent_to) const;
            void fixup_parent_child_moved(IObject * child_to) { *lhs_ = child_to; }

        private:
            IObject * parent_ = nullptr;
            IObject ** lhs_ = nullptr;
        };

        using MutationLog = std::vector<MutationLogEntry>;

        /** @class GcCopyCallback
         *  @brief optional callback to observe individual copy operations during GC
         *
         *  For viz
         **/
        class GcCopyCallback {
        public:
            virtual ~GcCopyCallback() = default;

            virtual void notify_gc_copy(std::size_t z, const void * src_addr, const void * dest_addr,
                                        generation src_gen, generation dest_gen) = 0;
            /** invoked when added to callback set (i.e. @ref GC::GcCopyCallbackSet) **/
            void notify_add_callback() {}
            /** invoked when removed from callback set **/
            void notify_remove_callback() {}
        };

        /** @class GC
         *  @brief generational garbage collector
         *
         *  Works with objects of type @ref xo::Object
         **/
        class GC : public IAlloc {
        public:
            using CallbackId = xo::fn::CallbackId;
            using GcCopyCallbackSet = xo::fn::UpCallbackSet<GcCopyCallback>;
            using nanos = decltype(xo::qty::qty::nanosecond);

            /** rebind is for typed allocators. since IAlloc is untyped,
             *  we want degenerate version
             **/
            template <typename U>
            struct rebind { using other = GC; };

        public:
            /** create new GC instance with configuration @p config **/
            explicit GC(const Config & config);
            /** noncopyable **/
            GC(const GC & other) = delete;
            virtual ~GC();

            /** create GC allocator.
             *
             *  Initial memory consumption:
             *  approximately 2x @ref Config::nursery_size_ + 2x @ref Config::tenured_size_
             **/
            static up<GC> make(const Config & config);

            /** runtime downcast **/
            static GC * from(IAlloc * mm);

            const Config & config() const { return config_; }
            std::uint8_t nursery_polarity() const { return nursery_polarity_; }
            std::uint8_t tenured_polarity() const { return tenured_polarity_; }
            const GCRunstate & runstate() const { return runstate_; }
            const GcStatistics & native_gc_statistics() const { return gc_statistics_; }
            GcStatisticsExt get_gc_statistics() const;
            const GcStatisticsHistory & gc_history() const { return gc_history_; }

            /** true iff GC permitted in current state **/
            bool is_gc_enabled() const { return gc_enabled_ == 0; }
            /** true iff GC has been requested **/
            bool is_gc_pending() const { return incr_gc_pending_ || full_gc_pending_; }
            /** true iff full GC pending **/
            bool is_full_gc_pending() const { return full_gc_pending_; }
            /** true during (and only during) a GC cycle **/
            bool gc_in_progress() const { return runstate_.in_progress(); }

            /** @return pagesize (will be the same for {nursery, tenured} spaces) **/
            std::size_t pagesize() const;
            /** @return hugepage size (will be the same for {nursery, tenured} spaces) **/
            std::size_t hugepage_z() const;

            /** @return allocation portion of Nursery to-space **/
            std::size_t nursery_to_allocated() const;
            /** @return reserved size of Nursery to-space **/
            std::size_t nursery_to_reserved() const;
            /** @return committed size of Nursery to-space **/
            std::size_t nursery_to_committed() const;
            /** @return nursery bytes used before checkpoint **/
            std::size_t nursery_before_checkpoint() const;
            /** @return nursery bytes used after checkpoint **/
            std::size_t nursery_after_checkpoint() const;
            /** @return allocated memory range for nursery **/
            std::pair<const std::byte *, const std::byte *> nursery_span(role role) const;
            /** @return nursery bytes used in from-space
             *  (only interesting during GC copy phase, e.g. during scope of a GcCopyCallback call)
             **/
            std::size_t nursery_from_allocated() const;
            /** @return reserved size of Tenured to-space **/
            std::size_t tenured_to_reserved() const;
            /** @return committed size of Tenured to-space **/
            std::size_t tenured_to_committed() const;
            /** @return tenured bytes used before checkpoint **/
            std::size_t tenured_before_checkpoint() const;
            /** @return tenured bytes used after checkpoint = promoted since last GC **/
            std::size_t tenured_after_checkpoint() const;

            /** @return generation to which object at @p x belongs **/
            generation_result tospace_generation_of(const void * x) const;
            /** @return generation to which object at @p x belongs,
             *          location relative to base address for that generation,
             *          and allocated size of that generation
             *          @p role chooses between to-space and from-space
             **/
            std::tuple<generation_result, std::size_t, std::size_t, std::size_t> location_of(role role, const void * x) const;
            /** @return generation to which object at @p x belongs,
             *          location relative to base address for @p x,
             *          and allocated size of generation
             **/
            std::tuple<generation_result, std::size_t, std::size_t, std::size_t> tospace_location_of(const void * x) const;
            /** @return generation that contains @p x, given it's in from-space **/
            generation_result fromspace_generation_of(const void * x) const;
            /** @return generation to which object at @p x belongs,
             *          location relative to base address for @p x,
             *          and allocated size of generation
             **/
            std::tuple<generation_result, std::size_t, std::size_t, std::size_t> fromspace_location_of(const void * x) const;
            /** true iff from-space contains @p x **/
            bool fromspace_contains(const void * x) const;
            /** @return free pointer for generation @p gen, i.e. nursery or tenured space **/
            std::byte * free_ptr(generation gen);
            /** @return current size of (number of entries in) mutation log **/
            std::size_t mlog_size() const;

            /** add gc root at address @p addr .  Gc will keep alive anything reachable
             *  from @c *addr
             **/
            void add_gc_root(IObject ** addr);
            /** reverse the effect of previous call to @ref add_gc_root **/
            void remove_gc_root(IObject ** addr);

            /** convenience wrapper **/

            template <typename T>
            void add_gc_root_dwim(gp<T> * p) {
                static_assert(std::is_convertible_v<T*, IObject*>);
                this->add_gc_root(reinterpret_cast<IObject**>(p->ptr_address()));
            }

            template <typename T>
            void remove_gc_root_dwim(gp<T> * p) {
                static_assert(std::is_convertible_v<T*, IObject*>);
                this->remove_gc_root(reinterpret_cast<IObject**>(p->ptr_address()));
            }

            /** may optionally use this to observe GC copy phase.
             *  Will be invoked once _per surviving object_, so not cheap.
             *  Intended for GC visualization.
             **/
            CallbackId add_gc_copy_callback(up<GcCopyCallback> fn);
            /** request garbage collection.
             *  If GC currently disabled, collection will be deferred until the next time GC
             *  is in an enabled state.  See @ref disable_gc and @ref enable_gc
             **/
            void request_gc(generation g);
            /** disable garbage collection until matching call to @ref enable_gc.
             *
             *  GC is disabled when number of calls to @ref disable_gc exceeds number of
             *  calls to @ref enable_gc.
             **/
            void disable_gc();
            /** enable garbage collection
             *
             *  GC is enabled when number of calls to @ref enable_gc is at least as large
             *  as number of calls to @ref disable_gc.
             *
             *  @return true iff GC performed
             **/
            bool enable_gc();
            /** same as @c this->enable_gc() followed by @c this->disable_gc()
             *  @return true iff GC performed
             **/
            bool enable_gc_once();

            // inherited from IAlloc..

            virtual const std::string & name() const final override;
            /** capacity in bytes (counting both free+allocated) for object storage.
             *  only counts one of {to-space, from-space},
             *  since one role is always held empty between collections.
             **/
            virtual std::size_t size() const final override;
            /** for committed count both to-space and from-space **/
            virtual std::size_t committed() const final override;
            virtual std::size_t allocated() const final override;
            virtual std::size_t available() const final override;
            /** only tests to-space **/
            virtual bool contains(const void * x) const final override;
            virtual bool is_before_checkpoint(const void * x) const final override;
            virtual std::size_t before_checkpoint() const final override;
            virtual std::size_t after_checkpoint() const final override;
            virtual bool debug_flag() const final override;

            virtual void clear() final override;
            virtual void checkpoint() final override;

            /** GC bookkeeping for an assignment that modifes an Object reference.
             *  Whenever an @ref Object instance P contains a member variable that can refer
             *  to another @ref Object, then we need to involve GC to perform the assignment.
             *  In particular a side-effect that changes the target of such reference to Q after P
             *  has been promoted, may lead to a tenured->nursery cross-generational pointer.
             *  GC needs to know about such pointers to it can update them as part of subsequent
             *  incremental collections.
             *
             *  @param parent.  object with member variable being modified
             *  @param lhs.     address of a member variable within the allocation of @p parent.
             *  @param rhs.     new target for @p *lhs
             **/
            virtual void assign_member(IObject * parent, IObject ** lhs, IObject* rhs) final override;
            /** evacuate @p *lhs and replace with forwarding pointer **/
            virtual void forward_inplace(IObject ** lhs) final override;
            /** during GC check for source objects owned by GC.
             *  See Object::_shallow_move.
             **/
            virtual bool check_owned(IObject * src) const final override;
            /** queries during GC to determine if object at address @p src should move:
             *  - full GC -> always
             *  - incr GC -> if not tenured
             **/
            virtual bool check_move(IObject * src) const final override;
            virtual std::byte * alloc(std::size_t z) final override;
            virtual std::byte * alloc_gc_copy(std::size_t z, const void * src) final override;

        private:
            ArenaAlloc * nursery_to() const { return nursery(role::to_space); }
            ArenaAlloc * nursery_from() const { return nursery(role::from_space); }

            ArenaAlloc * tenured_to() const { return tenured(role::to_space); }
            ArenaAlloc * tenured_from() const { return tenured(role::from_space); }

            ArenaAlloc * nursery(role r) const { return nursery_[role2int(r)].get(); }
            ArenaAlloc * tenured(role r) const { return tenured_[role2int(r)].get(); }

            MutationLog * mutation_log(role r) const { return mutation_log_[role2int(r)].get(); }

            /** begin GC now **/
            void execute_gc(generation g);
            /** cleanup phase.  aux function for @ref execute_gc **/
            void cleanup_phase(generation g, nanos dt);
            /** swap roles of From/To spaces for nursery generation **/
            void swap_nursery();
            /** swap roles of From/To spaces for tenured generation **/
            void swap_tenured();
            /** swap roles of From/To spaces for mutation log **/
            void swap_mutation_log();
            /** swap roles of FromSpace/ToSpace **/
            void swap_spaces(generation g);
            /** scan to-space for object statistics before GC */
            void capture_object_statistics(generation upto, capture_phase phase);
            /** copy object **/
            void copy_object(IObject ** addr, generation upto, ObjectStatistics * object_stats);
            /** copy everything reachable from global gc roots **/
            void copy_globals(generation g);
            /** review mutation log; may discover+rescue reachable objects.
             **/
            void forward_mutation_log(generation upto);
            /** Aux function for @ref execute_gc. Updates bookkeeping for cross-generational
             *  (T->N, aka xgen) and (N1->N0, aka xckp) pointers
             **/
            void incremental_gc_forward_mlog(ObjectStatistics * per_type_stats);
            /**
             *  Aux function for @ref incremental_gc_forward_mlog.  Calls this function until
             *  fixpoint.
             *
             *  @param from_mlog incoming mutation log. Contains {xgen,xckp} pointers before GC.
             *  Contents of this log is consumed (+discarded) before method returns.
             *  @param to_mlog    outgoing mutation log. Will contain {xgen,xckp} pointers after GC.
             *  @param defer_mlog contains log entries associated with possible garbage.
             **/
            void incremental_gc_forward_mlog_phase(MutationLog * from_mlog,
                                                   MutationLog * to_mlog,
                                                   MutationLog * defer_mlog,
                                                   ObjectStatistics * per_type_stats);
            /** Aux function for @ref execute_gc.  Updates bookkeeping for cross-generational
             *  (T->N, aka xgen) and (N1->N0, aka xckcp) pointers on full gc
             **/
            void full_gc_forward_mlog(ObjectStatistics * per_type_stats);
            /**
             *  Aux function for @ref full_gc_forward_mlog.  Calls this function until fixpoint.
             *
             *  @param from_mlog incoming mutation log. Contains {xgen,xckp} pointers before GC.
             *  Contents of this log is consumed (+discarded) before method returns.
             *  @param to_mlog    outgoing mutation log. Will contain {xgen,xckp} pointers after GC.
             *  @param defer_mlog contains log entries associated with possible garbage.
             *
             **/
            void full_gc_forward_mlog_phase(MutationLog * from_mlog,
                                            MutationLog * to_mlog,
                                            MutationLog * defer_mlog,
                                            ObjectStatistics * per_type_stats);

        private:
            /** garbage collector configuration **/
            Config config_;

            /** keep track of the identity of from-space and to-space.
             *  assist for animation (see xo-imgui/example/ex2).
             *  polarity alternates between 0 and 1 on each GC
             **/
            std::uint8_t nursery_polarity_ = 0;
            std::uint8_t tenured_polarity_ = 0;

            /** contains allocated objects, along with unreachable garbage to be collected.
             *  roles reverse after each incremental, or full, collection.
             **/
            std::array<up<ArenaAlloc>, role2int(role::N)> nursery_;
            /** empty space, destination for objects that survive collection.
             *  roles reverse after each full collection.
             **/
            std::array<up<ArenaAlloc>, role2int(role::N)> tenured_;

            /** current state of GC activity.
             *  @text
             *    in_progress full_move    descr
             *    -----------------------------------------
             *          false         *    gc not running
             *           true     false    incremental gc
             *           true      true    full gc
             *    -----------------------------------------
             *  @endtext
             **/
            GCRunstate runstate_;

            /** root object handles: targets of handles in this vector are always preserved by GC.
             *  Application can introduce new root object pointers at any time provided GC not running,
             *  but cannot withdraw them.
             **/
            std::vector<IObject**> gc_root_v_;

            /** log cross-generational and cross-checkpoint mutations.
             *  These need to be adjusted on next incremental collection
             **/
            std::array<up<MutationLog>, role2int(role::N)> mutation_log_;
            /** temporary mutation log (for deferred entries) **/
            up<MutationLog> defer_mutation_log_;

            /** allocation/collection counters **/
            GcStatistics gc_statistics_;
            /** optional per-object-type counters. snapshot at beginning of collection cycle **/
            std::array<ObjectStatistics, gen2int(generation::N)> object_statistics_sab_;
            /** optional per-object-type counters. snapshot at end of collection cycle **/
            std::array<ObjectStatistics, gen2int(generation::N)> object_statistics_sae_;

            /** true when GC requested,
             *  remains true until GC.. completes? begins?
             **/
            bool incr_gc_pending_ = false;
            bool full_gc_pending_ = false;

            /** enabled when 0.  disabled when <0 **/
            int gc_enabled_ = 0;

            /** rotating per-gc statistics history **/
            GcStatisticsHistory gc_history_;

            /** for (optional) viz: invoke when copying individual objects **/
            GcCopyCallbackSet gc_copy_cbset_;
        };
    } /*namespace gc*/

} /*namespace xo*/

/* end GC.hpp */
