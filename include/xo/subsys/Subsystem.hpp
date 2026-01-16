/* file Subsystem.hpp
 *
 * author: Roland Conybeare, Aug 2022
 */

#pragma once

#include "xo/indentlog/scope.hpp"
#include <iostream>
#include <functional>
#include <list>
#include <string_view>
#include <cstddef>

/* e.g. XO_SUBSYSTEM_TAG(simulator) => xo::S_simulator_tag */
#define XO_SUBSYSTEM_TAG(subsys_name) xo::S_ ## subsys_name ## _tag

/* e.g. XO_SUBSYSTEM_REQUIRE(simulator) =>
 *        xo::InitSubsys<xo::S_simulator_tag>::require()
 */
#define XO_SUBSYSTEM_REQUIRE(subsys_name) xo::InitSubsys<XO_SUBSYSTEM_TAG(subsys_name)>::require();

/* e.g. XO_SUBSYSTEM_PROVIDE(simulator, &init) =>
 *        xo::Subsystem::provide<xo::S_simulator_tag>("simulator", &init)
 */
#define XO_SUBSYSTEM_PROVIDE(subsys_name, init_addr) xo::Subsystem::provide<XO_SUBSYSTEM_TAG(subsys_name)>(STRINGIFY(subsys_name), init_addr)

//#define VERIFY_SUBSYSTEM(tag) Subsystem::verify_present<tag>(STRINGIFY(tag))

namespace xo {
    using xo::tostr;

    /* evidence that one or more subsystems have been initialized.
     * Used to prevent static linker stripping must-run initialization code
     */
    class InitEvidence {
    public:
        InitEvidence() = default;
        InitEvidence(std::uint64_t x) : evidence_{x} {}

        std::uint64_t evidence() const { return evidence_; }

        InitEvidence operator^=(InitEvidence x) {
            this->evidence_ ^= x.evidence_;

            return *this;
        } /*operator^=*/

        InitEvidence operator^(InitEvidence x) {
            return InitEvidence(this->evidence_ ^ x.evidence_);
        }

    private:
        /* we don't care about the specific value computed here,
         * purpose is to be sufficiently impenentrable to compiler such
         * that static linker can't optimize it away
         */
        std::uint64_t evidence_ = 0;
    }; /*InitEvidence*/

    inline std::ostream &
    operator<<(std::ostream & os, InitEvidence x) {
        os << "<init-evidence " << x.evidence() << ">";
        return os;
    } /*operator<<*/

    /* Goals:
     * 1. provide for code that must run once (and only once)
     *    to initialize subsystems
     * 2. in executable,  want to run such code after main() starts
     *    instead of relying on static initializers;
     *    that way init behavior can be parameterized based on
     *    program arguments
     *
     * Use
     *   // subsystem foo
     *
     *   enum Foo_tag {};
     *
     *   // guarantees that if anything gets initialized,  then
     *   // foo_init() is included
     *   //
     *   template<>
     *   struct InitSubsys<Foo_tag> {
     *     static void foo_init() { ... }
     *
     *     static InitEvidence require() {
     *       return Subsystem::require<Foo_tag>("foo", &foo_init);
     *     }
     *   };
     *
     *   .. register other subsystems ..
     *
     *   Subsystem::initialize_all();  // foo_init() has been called once
     *
     * If subsystem bar depends on supporting subsystem {foo, quux},  then write:
     *
     *   enum Bar_tag {};
     *
     *   template<>
     *   struct InitSubsys<Bar_tag> {
     *     static void bar_init() { ... }
     *
     *     static InitEvidence require() {
     *       InitEvidence retval;
     *
     *       retval ^= InitSubsys<Foo_tag>::require();
     *       retval ^= InitSubsys<Quux_tag>::require();
     *
     *       retval ^= Subsystem::require<Bar_tag>("bar", &bar_init);
     *     }
     *   };
     *
     * If using subsystems from a shared library (so no access to cmdline args etc):
     * e.g. in pyfoo.cpp:
     *
     *   InitEvidence s_pyfoo_init = InitSubsys<Foo_tag>::require();
     * or
     *   InitEvidence s_pyfoo_init = (InitSubsys<Foo_tag>::require()
     *                                 ^ InitSubsys<Bar_tag>::require());
     *
     * Note: Tag argument here no relation of BuildTag in SubsystemImpl<BuildTag> below
     */
    template<typename Tag>
    struct InitSubsys {};

    /* BuildTag: placeholder; insisting on header-only library */
    template <typename BuildTag>
    class SubsystemImpl {
    public:
        SubsystemImpl() = default;
        SubsystemImpl(bool require_flag,
                      std::string_view subsys_name,
                      std::function<void ()> init_fn)
            : require_flag_{require_flag},
              subsys_name_{subsys_name},
              init_fn_{init_fn} {}

        /* establish an empty Subsystem record for subsys_name.
         * record is _not_ linked into s_subsys_l!
         * idempotent.
         */
        template<typename SubsystemTag>
        static SubsystemImpl * establish() {
            static SubsystemImpl s_subsys;

            return &s_subsys;
        } /*establish*/

        template<typename SubsystemTag>
        static bool verify_present(std::string subsys_tag) {
            SubsystemImpl * subsys = establish<SubsystemTag>();

            if (!subsys->require_flag()) {
                throw std::runtime_error(tostr("subsystem not present."
                                               "(missing InitSubsys<", subsys_tag, ">::require() ?)"));
                return false;
            }

            return true;
        } /*verify_present*/

        /* provide (once only) initialization code for a subsystem with tag SubsystemTag.
         * ideally this would be called just once for a particular tag;
         * if called multiple times,   calls after the first are no-ops.
         */
        template<typename SubsystemTag>
        static InitEvidence provide(std::string_view subsys_name,
                                    std::function<void ()> init_fn) {
            SubsystemImpl * subsys = establish<SubsystemTag>();

            provide_aux(subsys_name, init_fn, subsys);

            return InitEvidence(reinterpret_cast<std::uint64_t>(subsys));
        } /*provide*/

        /* throw exception if there's anything left for .initialize_all() to do,
         * or subsystems have been added since last call to .initialize_all()
         * Can use this to remind application author to call SubsystemImpl::initialize_all()
         */
        static bool verify_all_initialized();

        /* 1. initialize all subsystems: promise that for every preceding call
         *    to .require<tag>(),  the corresponding initialization function has been
         *    run exactly once.
         * 2. harmless to call this multiple times -- will not call any init_fn more than once
         * 3. can interleave .initialize_all() with .require<tag>() as desired
         */
        static InitEvidence initialize_all();

        bool require_flag() const { return require_flag_; }
        bool init_flag() const { return init_flag_; }
        std::string_view subsys_name() const { return subsys_name_; }

        InitEvidence initialize();

    private:
        /* helper for .provide<SubsystemTag>() */
        static void provide_aux(std::string_view subsys_name,
                                std::function<void ()> init_fn,
                                SubsystemImpl * p_subsys);

    private:
        /* set to true iff .s_subsys_l has been extended since last call to .initialize_all() */
        static bool s_dirty_flag;
        /* one member for each unique call to .require() */
        static std::list<SubsystemImpl *> s_subsys_l;

    private:
        /* set to true on 1st call to .require() */
        bool require_flag_ = false;
        /* set to true when .init_fn() invoked */
        bool init_flag_ = false;
        /* unique subsystem name */
        std::string_view subsys_name_;
        /* call this function once (at most) to initialize this subsystem */
        std::function<void ()> init_fn_;
    }; /*SubsystemImpl*/

    template <typename BuildTag>
    bool
    SubsystemImpl<BuildTag>::s_dirty_flag = false;

    template <typename BuildTag>
    std::list<SubsystemImpl<BuildTag> *>
    SubsystemImpl<BuildTag>::s_subsys_l;

    template <typename BuildTag>
    void
    SubsystemImpl<BuildTag>::provide_aux(std::string_view subsys_name,
                                         std::function<void ()> init_fn,
                                         SubsystemImpl<BuildTag> * p_subsys)
    {
        if (!p_subsys->require_flag()) {
            /* 1st call to .provide() for this SubsystemTag */

            using xo::scope;
            using xo::xtag;

            scope log(XO_ENTER0(chatty),
                      xtag("subsys", subsys_name),
                      xtag("address", p_subsys));

            *p_subsys = SubsystemImpl<BuildTag>(true /*require_flag*/,
                                                subsys_name,
                                                init_fn);

            s_dirty_flag = true;
            s_subsys_l.push_back(p_subsys);
        }
    } /*provide_aux*/

    template <typename BuildTag>
    bool
    SubsystemImpl<BuildTag>::verify_all_initialized()
    {
        if (s_dirty_flag) {
            scope log(XO_ENTER0(error), "required subsystems NOT initialized!?");

            for (SubsystemImpl<BuildTag> * subsys : s_subsys_l) {
                if (!subsys->init_flag()) {
                    log && log("missing InitSubsys<S_", subsys->subsys_name(), "_tag>::require()");
                }
            }

            throw std::runtime_error("Subsystem::verify_initialized:"
                                     " Subsystem::initialize_all() never called, or out-of-date");
            return false;
        }

        return true;
    } /*verify_all_initialized*/

    template <typename BuildTag>
    InitEvidence
    SubsystemImpl<BuildTag>::initialize_all() {
        scope log(XO_ENTER0(chatty));

        InitEvidence retval;

        if (s_dirty_flag) {
            for (SubsystemImpl<BuildTag> * subsys : s_subsys_l) {
                log && log("init", xtag("subsys", subsys->subsys_name()));

                retval ^= subsys->initialize();
            }
        }

        s_dirty_flag = false;

        return retval;
    } /*initialize_all*/

    template <typename BuildTag>
    InitEvidence
    SubsystemImpl<BuildTag>::initialize()
    {
        if (!init_flag_) {
            init_flag_ = true;

            init_fn_();
        }

        return InitEvidence(reinterpret_cast<std::uint64_t>(this));
    } /*initialize*/

    using Subsystem = SubsystemImpl<class Subsystem_tag>;
} /*namespace xo*/

/* end Subsystem.hpp */
