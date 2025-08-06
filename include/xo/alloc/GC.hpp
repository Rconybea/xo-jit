/* GC.hpp
 *
 * author: Roland Conybeare, jul 2025
 */

#pragma once

#include "ListAlloc.hpp"
#include "xo/indentlog/print/array.hpp"
#include <vector>
#include <array>

namespace xo {
    /** types that can participate in GC inherit from this base class.  See Object.hpp in this directory **/
    class Object;

    namespace gc {
        enum class generation {
            nursery,
            tenured,
            N
        };

        constexpr std::size_t gen2int(generation x) { return static_cast<std::size_t>(x); }

        enum class generation_result {
            nursery,
            tenured,
            not_found
        };

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
             *  Will allocate more space as needed
             **/
            std::size_t initial_nursery_z_ = 0;
            /** initial size in bytes for oldest (Tenured) generation.
             *  GC allocates two tenured spaces of this size
             *  Will allocate more space as needed
             **/
            std::size_t initial_tenured_z_ = 0;
            /** true to permit incremental garbage collection **/
            bool allow_incremental_gc_ = true;
            /** true to enable debug logging **/
            bool debug_flag_ = false;
        };

        /** @class ObjectStatistics
         *  @brief placeholder for type-driven allocation statistics
         *
         *  Passed to @ref Object::deep_move for example
         **/
        class ObjectStatistics {
        };

        /** @class PerGenerationStatistics
         *  @brief garbage collection statistics for particular GC generation
         **/
        class PerGenerationStatistics {
        public:
            /** update statistics after a GC cycle
             *  @param alloc_z.    new allocations (since preceding GC)
             *  @param before_z.   generation size (bytes allocated) before collection
             *  @param after_z.    generation size after collection
             *  @param promote_z.  bytes promoted to next generation
             **/
            void include_gc(std::size_t alloc_z, std::size_t before_z, std::size_t after_z,
                            std::size_t promote_z);
            /** update with current state (use at end of gc cycle) **/
            void update_snapshot(std::size_t after_z);

            /** @param os.  write stats on this output stream **/
            void display(std::ostream & os) const;

            /** number of bytes currently in use **/
            std::size_t used_z_ = 0;

            /** number of collection cycles completed **/
            std::size_t n_gc_ = 0;
            /** sum of new alloc bytes, sampled at start of each collection cycle **/
            std::size_t new_alloc_z_ = 0;
            /** sum of allocated bytes sampled at beginning of each collection cycle **/
            std::size_t scanned_z_ = 0;
            /** sum of bytes remaining after collection cycle **/
            std::size_t survive_z_ = 0;
            /** sum of bytes promoted to next generation **/
            std::size_t promote_z_ = 0;
        };

        inline std::ostream & operator<< (std::ostream & os, const PerGenerationStatistics & x) {
            x.display(os);
            return os;
        }

        /** @class GcStatistics
         *  @brief garbage collection statistics
         **/
        class GcStatistics {
        public:
            /** update statistics after a GC cycle
             *  @param upto.      nursery -> incremental collection; tenured -> full collection
             *  @param alloc_z.   new allocations (since preceding GC)
             *  @param before_z.  generation size (bytes allocated) before collection
             *  @param after_z.   generation size after collection
             *  @param promote_z. bytes promoted to next generation
             **/
            void include_gc(generation upto, std::size_t alloc_z,
                            std::size_t before_z, std::size_t after_z, std::size_t promote_z);
            /** update snapshot for current state.
             *  Use with tenured stats after incremental gc
             **/
            void update_snapshot(generation upto, std::size_t after_z);

            /** @param os. write stats on this output stream **/
            void display(std::ostream & os) const;

            /** statistics gathered across {incr, full} GCs respectively **/
            std::array<PerGenerationStatistics, static_cast<std::size_t>(generation::N)> gen_v_;
            /** total bytes allocated since inception **/
            std::size_t total_allocated_ = 0;
            /** snapshot of total bytes promoted asof beginning of last gc cycle **/
            std::size_t total_promoted_sab_ = 0;
            /** total bytes promoted from nursery->tenured since inception **/
            std::size_t total_promoted_ = 0;

            /** total number of mutations to already-allocated objects,
             *  whether or not GC needs to log them.
             **/
            std::size_t n_mutation_ = 0;
            /** total number of mutation eligible for logging **/
            std::size_t n_logged_mutation_ = 0;
            /** total number of cross-generation mutations (tenured->nursery when reported) **/
            std::size_t n_xgen_mutation_ = 0;
            /** total number of cross-checkpoint mutations (N0 -> N1 when reported) **/
            std::size_t n_xckp_mutation_ = 0;

            /** per-type statistics (placeholder) **/
            ObjectStatistics per_type_stats_;
        };

        inline std::ostream & operator<< (std::ostream & os, const GcStatistics & x) {
            x.display(os);
            return os;
        }

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
            MutationLogEntry(Object * parent, Object ** lhs) : parent_{parent}, lhs_{lhs} {}

            Object * parent() const { return parent_; }
            Object ** lhs() const { return lhs_; }

            Object * child() const { return *lhs_; }

            bool is_child_forwarded() const;
            bool is_parent_forwarded() const;

            Object * parent_destination() const;

            /** Flag obsolete mutation.
             *  Future proofing, never happens for regular objects
             **/
            bool is_dead() const { return false; }

            MutationLogEntry update_parent_moved(Object * parent_to) const;
            void fixup_parent_child_moved(Object * child_to) { *lhs_ = child_to; }

        private:
            Object * parent_;
            Object ** lhs_;
        };

        using MutationLog = std::vector<MutationLogEntry>;

        /** @class GC
         *  @brief generational garbage collector
         *
         *  Works with objects of type @ref xo::Object
         **/
        class GC : public IAlloc {
        public:
            /** create new GC instance with configuration @p config **/
            explicit GC(const Config & config);
            virtual ~GC();

            /** create GC allocator.
             *
             *  Initial memory consumption:
             *  approximately 2x @ref Config::nursery_size_ + 2x @ref Config::tenured_size_
             **/
            static up<GC> make(const Config & config);

            const GCRunstate & runstate() const { return runstate_; }
            const GcStatistics & gc_statistics() const { return gc_statistics_; }

            /** true iff GC permitted in current state **/
            bool is_gc_enabled() const { return gc_enabled_ == 0; }
            /** true during (and only during) a GC cycle **/
            bool gc_in_progress() const { return runstate_.in_progress(); }
            /** @return generation to which object at @p x belongs **/
            generation_result tospace_generation_of(const void * x) const;
            /** @return generation that contains @p x, given it's in from-space **/
            generation_result fromspace_generation_of(const void * x) const;
            /** true iff from-space contains @p x **/
            bool fromspace_contains(const void * x) const;
            /** @return free pointer for generation @p gen, i.e. nursery or tenured space **/
            std::byte * free_ptr(generation gen);
            /** @return current size of (number of entries in) mutation log **/
            std::size_t mlog_size() const;

            /** add gc root at address @p addr .  Gc will keep alive anything reachable
             *  from @c *addr
             **/
            void add_gc_root(Object ** addr);
            /** request garbage collection. **/
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
             **/
            void enable_gc();

            // inherited from IAlloc..

            virtual const std::string & name() const final override;
            /** capacity in bytes (counting both free+allocated) for object storage.
             *  only counts one of {to-space, from-space},
             *  since one role is always held empty between collections.
             **/
            virtual std::size_t size() const final override;

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
            virtual void assign_member(Object * parent, Object ** lhs, Object* rhs) final override;

            virtual std::byte * alloc(std::size_t z) final override;
            virtual std::byte * alloc_gc_copy(std::size_t z, const void * src) final override;

            virtual void release_redline_memory() final override;

        private:
            /** begin GC now **/
            void execute_gc(generation g);
            /** cleanup phase.  aux function for @ref execute_gc **/
            void cleanup_phase(generation g);
            /** swap roles of From/To spaces for nursery generation **/
            void swap_nursery();
            /** swap roles of From/To spaces for tenured generation **/
            void swap_tenured();
            /** swap roles of From/To spaces for mutation log **/
            void swap_mutation_log();
            /** swap roles of FromSpace/ToSpace **/
            void swap_spaces(generation g);
            /** copy object **/
            void copy_object(Object ** addr, generation upto, ObjectStatistics * object_stats);
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

        private:
            /** garbage collector configuration **/
            Config config_;

            /** contains allocated objects, along with unreachable garbage to be collected.
             *  roles reverse after each incremental, or full, collection.
             **/
            std::array<up<ListAlloc>, role2int(role::N)> nursery_;
            /** empty space, destination for objects that survive collection.
             *  roles reverse after each full collection.
             **/
            std::array<up<ListAlloc>, role2int(role::N)> tenured_;

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
            std::vector<Object**> gc_root_v_;

            /** log cross-generational and cross-checkpoint mutations.
             *  These need to be adjusted on next incremental collection
             **/
            std::array<up<MutationLog>, role2int(role::N)> mutation_log_;
            /** temporary mutation log (for deferred entries) **/
            up<MutationLog> defer_mutation_log_;

            /** allocation/collection counters **/
            GcStatistics gc_statistics_;

            /** trigger full GC whenever this much data arrives in tenured generation **/
            std::size_t full_gc_threshold_ = 0;
            /** trigger incr GC whenever this much data arrives in nuresery generation **/
            std::size_t incr_gc_threshold_ = 0;

            /** true when GC requested,
             *  remains true until GC.. completes? begins?
             **/
            bool incr_gc_pending_ = false;
            bool full_gc_pending_ = false;

            /** enabled when 0.  disabled when <0 **/
            int gc_enabled_ = 0;
        };
    } /*namespace gc*/

} /*namespace xo*/

/* end GC.hpp */
