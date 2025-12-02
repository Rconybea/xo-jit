/* @file GC.test.cpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#include "xo/alloc/GC.hpp"
#include "xo/allocutil/gc_allocator_traits.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::gc::IAlloc;
    using xo::gc::GC;
    using xo::gc::gc_allocator_traits;
    using xo::gc::generation;
    using xo::gc::Config;
    using xo::reflect::TaggedPtr;

    namespace ut {

        namespace {
            struct testcase_gc {
                testcase_gc(std::size_t nz, std::size_t tz, std::size_t n_gct, std::size_t t_gct)
                    : nursery_z_{nz}, tenured_z_{tz}, incr_gc_threshold_{n_gct}, full_gc_threshold_{t_gct} {}

                std::size_t nursery_z_;
                std::size_t tenured_z_;
                std::size_t incr_gc_threshold_;
                std::size_t full_gc_threshold_;
            };

            std::vector<testcase_gc>
            s_testcase_v = {
                // n_gct: nursery gc threshold
                // t_gct: tenured gc threshold
                //
                //            nz    tz  n_gct  t_gct
                testcase_gc(1024, 4096,  1024,  1024)
            };
        }

        TEST_CASE("gc", "[alloc][gc]")
        {
            for (std::size_t i_tc = 0, n_tc = s_testcase_v.size(); i_tc < n_tc; ++i_tc) {
                try {
                    const testcase_gc & tc = s_testcase_v[i_tc];

                    up<GC> gc = GC::make(
                        {.initial_nursery_z_ = tc.nursery_z_,
                         .initial_tenured_z_ = tc.tenured_z_,
                         .incr_gc_threshold_ = tc.incr_gc_threshold_,
                         .full_gc_threshold_ = tc.full_gc_threshold_,
                        });

                    REQUIRE(gc.get());
                    REQUIRE(gc->name() == "GC");
                    REQUIRE(gc->nursery_to_allocated() == 0);
                    REQUIRE(gc->nursery_to_committed() >= tc.nursery_z_);
                    REQUIRE(gc->nursery_to_reserved() >= tc.nursery_z_);
                    REQUIRE(gc->nursery_to_reserved() < tc.nursery_z_ + gc->hugepage_z());
                    REQUIRE(gc->size() >= tc.nursery_z_ + tc.tenured_z_);
                    REQUIRE(gc->size() < tc.nursery_z_ + gc->hugepage_z() + tc.tenured_z_ + gc->hugepage_z());
                    REQUIRE(gc->allocated() == 0);
                    REQUIRE(gc->available() == gc->nursery_to_reserved());
                    REQUIRE(gc->before_checkpoint() == 0);
                    // ListAlloc model is that nothing is before checkpoint
                    // until it's first established
                    REQUIRE(gc->after_checkpoint() == 0);

                    REQUIRE(gc->gc_in_progress() == false);
                    REQUIRE(gc->is_gc_enabled() == true);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::nursery)].n_gc_ == 0);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::tenured)].n_gc_ == 0);

                    /* gc with empty state */
                    gc->request_gc(generation::nursery);

                    REQUIRE(gc->gc_in_progress() == false);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::nursery)].n_gc_ == 1);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::tenured)].n_gc_ == 0);

                    /* still empty state */
                    gc->request_gc(generation::tenured);

                    REQUIRE(gc->gc_in_progress() == false);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::nursery)].n_gc_ == 1);
                    REQUIRE(gc->native_gc_statistics().gen_v_[gen2int(generation::tenured)].n_gc_ == 1);
                } catch (std::exception & ex) {
                    std::cerr << "caught exception: " << ex.what() << std::endl;
                    REQUIRE(false);
                }
            }
        }

        /** gc-enabled allocator **/
        namespace {
            /** Setup test with custom allocator
             *  
             **/
            template <typename Nested, typename GcObjectInterface>
            struct TestClass : public GcObjectInterface {
                TestClass() = default;
                explicit TestClass(const Nested & member1) : member1_{member1} {}
                
                // using allocator_type = Allocator;
                // using allocator_traits = xo::gc::gc_allocator_traits<Allocator>;

                /** stage1 - just allocates some memory using allocator **/
                template <typename Allocator>
                static TestClass * make_0(Allocator & alloc) {
                    TestClass * mem = alloc.allocate(sizeof(TestClass));

                    /* but ctor will not have run, so ub to visit object */

                    return mem;
                }

                /** stage2 - use allocator_traits construct **/
                template <typename Allocator>
                static TestClass * make_1(Allocator & alloc) {
                    using traits = gc_allocator_traits<Allocator>;

                    TestClass * mem = traits::allocate(alloc, 1);

                    /* ctor will not have run here either */
                    return mem;
                }

                /** stage3 - invoke construct **/
                template <typename Allocator>
                static TestClass * make_2(Allocator & alloc) {
                    using traits = gc_allocator_traits<Allocator>;

                    TestClass * obj = traits::allocate(alloc, 1);
                    try {
                        // placement new
                        traits::construct(alloc, obj);

                        return obj;
                    } catch(...) {
                        traits::deallocate(alloc, obj, 1);
                        throw;
                    }
                }

                /** stage4 - init nested type **/
                template <typename Allocator>
                static TestClass * make_3(Allocator & alloc) {
                    using traits = gc_allocator_traits<Allocator>;

                    TestClass * obj = traits::allocate(alloc, 1);
                    try {
                        Nested nested;

                        // placemenet new
                        traits::construct(alloc, obj);

                        return obj;
                    } catch(...) {
                        traits::deallocate(alloc, obj, 1);
                        throw;
                    }
                }

                // ----- inherited from Object -----

                virtual TaggedPtr self_tp() const final override {
                    assert(false); return TaggedPtr::universal_null();
                }
                virtual void display(std::ostream & os) const final override {
                    os << "<TestClass>";
                }
                virtual std::size_t _shallow_size() const final override {
                    assert(false); return sizeof(*this);
                }
                virtual IObject * _shallow_copy(IAlloc * gc) const final override {
                    assert(false); return nullptr;
                }
                virtual std::size_t _forward_children(IAlloc * gc) final override {
                    assert(false); return _shallow_size();
                }

                Nested member1_;
            };

            struct MemberType {
                MemberType() : ctor_ran_{true} {}
                explicit MemberType(const std::vector<gp<Object>> & mem2) : member2_{mem2} {}

                std::vector<gp<Object>> member2_;
                std::size_t ctor_ran_ = false;
            };
        }

        TEST_CASE("gc_allocator_traits", "[alloc][gc]")
        {
            for (std::size_t i_tc = 0, n_tc = s_testcase_v.size(); i_tc < n_tc; ++i_tc) {
                try {
                    const testcase_gc & tc = s_testcase_v[i_tc];

                    up<GC> gc = GC::make(
                        {.initial_nursery_z_ = tc.nursery_z_,
                         .initial_tenured_z_ = tc.tenured_z_,
                         .incr_gc_threshold_ = tc.incr_gc_threshold_,
                         .full_gc_threshold_ = tc.full_gc_threshold_,
                        });

                    REQUIRE(gc.get());
                    REQUIRE(gc->name() == "GC");

                    using ex_allocator = xo::gc::allocator<int>;
                    using MyObjectInterface = gc_allocator_traits<ex_allocator>::template object_interface<ex_allocator>;
                    using NestedElementAllocator = xo::gc::allocator<gp<Object>>;
                    using NestedType = MemberType;
                    using MyType = TestClass<NestedType, MyObjectInterface>;
                    using MyAllocator = xo::gc::allocator<MyType>;

                    MyAllocator alloc(gc.get());

                    {
                        /* verify that MyType is constructible */
                        MyType obj0;

                        REQUIRE(obj0.member1_.ctor_ran_ == true);
                    }

                    {
                        MyType * mem0 = MyType::make_0(alloc);

                        REQUIRE(mem0 != nullptr);
                        REQUIRE(mem0->member1_.ctor_ran_ == false);
                    }

                    {
                        MyType * mem1 = MyType::make_1(alloc);

                        REQUIRE(mem1 != nullptr);
                        REQUIRE(mem1->member1_.ctor_ran_ == false);
                    }

                    {
                        MyType * mem2 = MyType::make_2(alloc);

                        REQUIRE(mem2 != nullptr);
                        REQUIRE(mem2->member1_.ctor_ran_ == true);
                    }

                    {
                        MyType * mem3 = MyType::make_3(alloc);

                        REQUIRE(mem3 != nullptr);
                        REQUIRE(mem3->member1_.ctor_ran_ == true);
                    }

                    gp<MyType> ptr;
                    {
                        REQUIRE(ptr.is_null());
                        //ptr = MyType::make_0();
                    }
                } catch (std::exception & ex) {
                    std::cerr << "caught exception: " << ex.what() << std::endl;
                    REQUIRE(false);
                }
            }
            
        }

    } /*namespace ut*/
} /*namespace xo*/

/* GC.test.cpp */
