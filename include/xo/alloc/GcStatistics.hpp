/* GcStatistics.hpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#pragma once

#include "generation.hpp"
#include "CircularBuffer.hpp"
#include "xo/reflect/TypeDescr.hpp"
#include "xo/unit/quantity.hpp"
#include "xo/unit/quantity_iostream.hpp"
#include "xo/indentlog/print/pretty.hpp"
#include <ostream>
#include <array>

namespace xo {
    namespace gc {
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
            GcStatistics() = default;

            /** update statistics at beginning of a GC cycle
             *  @param upto.      nursery -> incremental collection; tenured -> full collection
             *  @param alloc_z.   new allocations (since preceding GC)
             **/
            void begin_gc(generation upto,
                          std::size_t alloc_z);

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

            /** number of collection cycles, whether full or incremental **/
            std::size_t n_gc() const { return gen_v_[gen2int(generation::nursery)].n_gc_; }

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
            /** total number of mutation eligible for logging (cumulative across GCs) **/
            std::size_t n_logged_mutation_ = 0;
            /** total number of cross-generation mutations
             *  (tenured->nursery when reported; cumulative across GCs) **/
            std::size_t n_xgen_mutation_ = 0;
            /** total number of cross-checkpoint mutations
             *  (N0 -> N1 when reported; cumulative across GCs)
             **/
            std::size_t n_xckp_mutation_ = 0;
        };

        inline std::ostream & operator<< (std::ostream & os, const GcStatistics & x) {
            x.display(os);
            return os;
        }

        /** @class GcStatisticsExt
         *  @brief extend GcStatistics for application convenience
         **/
        class GcStatisticsExt : public GcStatistics {
        public:
            GcStatisticsExt() = default;
            explicit GcStatisticsExt(const GcStatistics & x) : GcStatistics{x} {}

            /** @param os. write stats on this output stream **/
            void display(std::ostream & os) const;

            /** current capacity of nursery generation **/
            std::size_t nursery_z_ = 0;
            /** current nursery survivor size **/
            std::size_t nursery_before_checkpoint_z_ = 0;
            /** current nursery new alloc size **/
            std::size_t nursery_after_checkpoint_z_ = 0;
            /** current capacity of tenured generation **/
            std::size_t tenured_z_ = 0;
        };

        inline std::ostream & operator<< (std::ostream & os, const GcStatisticsExt & x) {
            x.display(os);
            return os;
        }

        /** @class GcStatisticsHistoryItem
         *  @brief info we want to record over time (won't have cumulative things in it)
         **/
        class GcStatisticsHistoryItem {
        public:
            using nanos = xo::qty::type::nanoseconds<std::int64_t>;

        public:
            GcStatisticsHistoryItem() = default;
            constexpr GcStatisticsHistoryItem(std::size_t gc_seq,
                                              generation upto,
                                              std::size_t new_alloc_z,
                                              std::size_t survive_z,
                                              std::size_t promote_z,
                                              std::size_t persist_z,
                                              std::size_t effort_z,
                                              std::size_t garbage0_z,
                                              std::size_t garbage1_z,
                                              std::size_t garbageN_z,
                                              nanos dt,
                                              std::size_t sum_effort_z,
                                              std::size_t sum_garbage_z)
            : gc_seq_{gc_seq},
              upto_{upto},
              new_alloc_z_{new_alloc_z},
              survive_z_{survive_z},
              promote_z_{promote_z},
              persist_z_{persist_z},
              effort_z_{effort_z},
              garbage0_z_{garbage0_z},
              garbage1_z_{garbage1_z},
              garbageN_z_{garbageN_z},
              dt_{dt},
              sum_effort_z_{sum_effort_z},
              sum_garbage_z_{sum_garbage_z}
                {}
            constexpr GcStatisticsHistoryItem(const GcStatisticsHistoryItem &) = default;

            std::size_t garbage_z() const { return garbage0_z_ + garbage1_z_ + garbageN_z_; }

            float efficiency() const {
                std::size_t gz = this->garbage_z();

                return gz / static_cast<float>(effort_z_ + gz);
            }

            /** lifetime byte-weighted average collection efficiency.  Always in [0.0, 1.0] **/
            float average_efficiency() const {
                return sum_garbage_z_ / static_cast<float>(sum_effort_z_ + sum_garbage_z_);
            }

            /** collection rate, in bytes/sec **/
            float collection_rate() const;

            GcStatisticsHistoryItem & operator=(const GcStatisticsHistoryItem & x) {
                gc_seq_ = x.gc_seq_;
                upto_ = x.upto_;
                new_alloc_z_ = x.new_alloc_z_;
                survive_z_ = x.survive_z_;
                promote_z_ = x.promote_z_;
                persist_z_ = x.persist_z_;
                effort_z_ = x.effort_z_;
                garbage0_z_ = x.garbage0_z_;
                garbage1_z_ = x.garbage1_z_;
                garbageN_z_ = x.garbageN_z_;
                this->dt_.scale_ = x.dt_.scale_;

                sum_effort_z_ = x.sum_effort_z_;
                sum_garbage_z_ = x.sum_garbage_z_;

                return *this;
            }

            /** @param os. write stats on this output stream **/
            void display(std::ostream & os) const;

            /** sequence number for collection being reported **/
            std::size_t gc_seq_ = 0;
            /** type of GC that generated this record **/
            generation upto_;
            /** #of bytes new allocation **/
            std::size_t new_alloc_z_ = 0;
            /** #of bytes surviving their first collection (i.e. N0->N1) **/
            std::size_t survive_z_ = 0;
            /** #of bytes promoted to tenured.
             *  Comprises all objects surviving their 2nd collection (i.e. N1->T)
             **/
            std::size_t promote_z_ = 0;
            /** #of bytes surviving 3rd of later collection **/
            std::size_t persist_z_ = 0;
            /** #of bytes copied **/
            std::size_t effort_z_ = 0;
            /** #of bytes garbage from N0 (i.e. survived 0 GCs) **/
            std::size_t garbage0_z_ = 0;
            /** #of bytes garbage from N1 (i.e. survived 1 GCs) **/
            std::size_t garbage1_z_ = 0;
            /** #of bytes garbage from T (i.e. survived 2+ GCs) **/
            std::size_t garbageN_z_ = 0;
            /** elapsed time for this GC (see @ref GC::execute_gc) **/
            nanos dt_;

            // ----- cumulative statistics -----

            /** sum (in bytes) copied by collections since inception **/
            std::size_t sum_effort_z_ = 0;
            /** sum (in bytes) of garbage collected since inception **/
            std::size_t sum_garbage_z_ = 0;
        };

        inline std::ostream & operator<< (std::ostream & os, const GcStatisticsHistoryItem & x) {
            x.display(os);
            return os;
        }

        using GcStatisticsHistory = CircularBuffer<GcStatisticsHistoryItem>;
    } /*namespace gc*/

    namespace print {
        template <>
        struct ppdetail<xo::gc::PerGenerationStatistics> {
            static bool print_pretty(const ppindentinfo &, const xo::gc::PerGenerationStatistics &);
        };

        template<>
        struct ppdetail<xo::gc::GcStatistics> {
            static bool print_pretty(const ppindentinfo &, const xo::gc::GcStatistics &);
        };

        template<>
        struct ppdetail<xo::gc::GcStatisticsExt> {
            static bool print_pretty(const ppindentinfo &, const xo::gc::GcStatisticsExt &);
        };

        template<>
        struct ppdetail<xo::gc::GcStatisticsHistoryItem> {
            static bool print_pretty(const ppindentinfo &, const xo::gc::GcStatisticsHistoryItem &);
        };
    } /*namespace print*/
} /*namespace xo*/

/* end GcStatistics.hpp */
