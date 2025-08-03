/* AllocPolicy.hpp
 *
 * author: Roland Conybeare, Jul 2025
 */

#include <cstdint>

namespace xo {
    /** Tag class, drives overload of operator new.
     *  See also: xoglobal, xocopy
     **/
    struct xolib {
        xolib() = default;
    };

    /** @brief opt-in allocator for XO libraries.
     *
     *  By default delegates to vanilla operator new/delete,
     *  but can set alloc/free functions at runtime to
     *  adopt a different implementation.
     *
     *  Intending this to op-in to garbage-collector?
     *  Not sure if we actually need this
     *
     *  Use:
     *    struct Foo { .. };
     *    auto p = new (xo) Foo(..);
     **/
    class XoAllocPolicy {
    public:
        using AllocFn = void* (*)(std::size_t);
        using FreeFn  = void (*)(void *);

    public:
        XoAllocPolicy() = default;

        static void * global_alloc(std::size_t z) { return ::operator new(z); }
        static void global_free(void * x) { ::operator delete(x); }

        void * alloc(std::size_t z) { return (*alloc_)(z); }
        void free(void * x) { (*free_)(x); }

    private:
        AllocFn alloc_ = global_alloc;
        FreeFn free_ = global_free;
    };

    /** singleton xolib instance **/
    static XoAllocPolicy xo;
}

inline void * operator new(std::size_t z, xo::xolib) {
    return xo::xo.alloc(z);
}

void operator delete(void * ptr) noexcept;

/* end AllocPolicy.hpp */
