/* file Forwarding1.hpp
 *
 * author: Roland Conybeare, Aug 2025
 */

#include "Object.hpp"

namespace xo {
    namespace obj {
        class Forwarding1 : public Object {
        public:
            explicit Forwarding1(gp<Object> dest);

            // inherited from Object..
            virtual TaggedPtr   self_tp() const final override;
            virtual bool        _is_forwarded() const final override { return true; }
            virtual Object *    _offset_destination(Object * src) const final override;
            virtual Object *    _destination() final override;
            /** never called on Forwarding1 **/
            virtual std::size_t _shallow_size() const final override;
            /** never called on Forwarding1 **/
            virtual Object *    _shallow_copy() const final override;
            /** never called on Forwarding1 **/
            virtual std::size_t _forward_children() final override;

        private:
            /** the object that used to be located at this address (i.e. @c this)
             *  has been moved to @ref destination_ ,
             *  with original location overwritten by a forwarding pointer
             *
             *  Require:
             *  - can only use Forwarding with types that have a single vtable.
             *    To forward a multiply-inheriting class with two vtables, use Forwarding2.
             *  - if you try to use Forwarding for an object with multiple vtables,
             *    one of the vtable pointers will be replaced by @ref destination_.
             *    UB revealed when GC traverses a pointer that relies on the 2nd
             *    vtable to index virtual methods.
             **/
            gp<Object> dest_;
        };

    } /*namespace obj*/
} /*namespace xo*/

/* end Forwarding1.hpp */
