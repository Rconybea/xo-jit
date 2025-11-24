/* Forwarding1.test.cpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "Forwarding1.hpp"
#include "ArenaAlloc.hpp"
#include "xo/reflect/Reflect.hpp"
#include <catch2/catch.hpp>
#include <cstring>

namespace xo {
    using xo::reflect::Reflect;
    using xo::obj::Forwarding1;

    namespace gc {
        namespace {
            class DummyObject : public Object {
            public:
                explicit DummyObject(const char * data) {
                    ::strncpy(data_.data(), data, 128);
                }

                gp<Object> member() const { return member_; }
                void assign_member(Object * x) {
                    Object::mm->assign_member(this, member_.ptr_address(), x);
                }

                TaggedPtr self_tp() const final override {
                    return Reflect::make_tp(const_cast<DummyObject*>(this));
                }

                void display(std::ostream & os) const final override { os << data_; }

                virtual std::size_t _shallow_size() const final override { return sizeof(*this); }
                virtual Object * _shallow_copy(gc::IAlloc * mm) const final override { return new (Cpof(mm, this)) DummyObject(*this); }
                virtual std::size_t _forward_children(gc::IAlloc * gc) final override { return _shallow_size(); }

            private:
                std::array<char, 128> data_;
                gp<Object> member_;
            };
        }

        TEST_CASE("Forwarding1", "[gc][alloc]")
        {
            bool saved = tag_config::tag_color_enabled;
            tag_config::tag_color_enabled = false;

            gp<Object> obj = new DummyObject("Well, I wasn't expecting that!");
            gp<Forwarding1> fwd = new Forwarding1(obj);

            REQUIRE(fwd->_destination() == obj.ptr());
            REQUIRE(fwd->_offset_destination(fwd.ptr()) == obj.ptr());

            REQUIRE(fwd->self_tp().td()->short_name() == "Forwarding1");

            std::stringstream ss;
            ss << fwd;

            REQUIRE(ss.str() == "<fwd :dest-td DummyObject>");

            tag_config::tag_color_enabled = saved;
        }

        TEST_CASE("IAlloc.assign_member", "[gc][alloc]")
        {
            /* not giving this nit it's own translation unit.
             */

            gp<DummyObject> obj = new DummyObject("This also a surprise..");

            up<ArenaAlloc> arena = ArenaAlloc::make("test", 1024, false);

            Object::mm = arena.get();

            obj->assign_member(obj.ptr());

            REQUIRE(obj->member().ptr() == obj.ptr());
        }

    } /*namespace gc*/
} /*namespace xo*/

/* end Forwarding1.test.cpp */
