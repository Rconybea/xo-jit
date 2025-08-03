/* Forwarding.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#pragma once

#include "Object.hpp"

namespace xo {
    namespace gc {
        class Forwarding : public Object {
        public:
            Forwarding() = default;

            // inherited from Object..
#ifdef NOT_USING
            virtual bool _is_forwarded() const override final { return true; }
#endif
            virtual Object * _destination() override final { return destination_.ptr(); }

        private:
            gp<Object> destination_;
        };
    } /*namespace gc*/
} /*namespace xo*/

/* end Forwarding.hpp */
