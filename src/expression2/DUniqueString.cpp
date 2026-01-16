/** @file DUniqueString.cpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "DUniqueString.hpp"
#include <xo/arena/padding.hpp>
#include <xo/indentlog/scope.hpp>

namespace xo {
    using xo::mm::padding;
    using xo::facet::typeseq;

    namespace scm {
        const DString *
        DUniqueString::_text() const noexcept
        {
            // location of paired DString is chosen
            // by allocator (DArena, probably).
            //
            // In general allocator alignment more conservative
            // than C++ alignment
            //
            // Remmebr also: although DUniqueString has zero members,
            // C++ requires it to behave asif size at least 1 byte
            // for iterator consistency
            // (e.g. because c++ would support iterating over
            //  std::vector<EmptyStruct>)
            //
            size_t offset = padding::with_padding(sizeof(*this));
            assert(offset > 0);

            return (const DString *)(((std::byte *)this) + offset);
        }

        bool
        DUniqueString::pretty(const ppindentinfo & ppii) const
        {
            return _text()->pretty(ppii);
        }

        DUniqueString *
        DUniqueString::from_view(obj<AAllocator> mm,
                                 std::string_view sv)
        {
            scope log(XO_DEBUG(false));

            /** fine point: choosing to allocate DUniqueString ahead of DString,
             *  so it comes first in bump allocator
             **/

            void * mem = mm.super_alloc(typeseq::id<DUniqueString>(),
                                        sizeof(DUniqueString));
            DUniqueString * result = new (mem) DUniqueString();

            /** allocated in memory immediate following @p result.
             *  This optimization saves us one pointer (8 bytes) in DUniqueString
             *  itself, plus one allocation header (8 bytes) for 16 bytes total
             **/
            DString * text = DString::from_view_suballoc(mm, sv);

            log && log(xtag("result", result), xtag("result.text", result->_text()), xtag("text", text));

            assert(text);
            assert(text == result->_text());

            /** must finish super-allocation before next alloc **/
            mm.sub_alloc(0, true);

            return result;
        }

        size_t
        DUniqueString::shallow_size() const noexcept
        {
            return sizeof(DUniqueString);
        }

        DUniqueString *
        DUniqueString::shallow_copy(obj<AAllocator> mm) const noexcept
        {
            // well-posed, but not expected to be used.
            assert(false);

            DUniqueString * copy = (DUniqueString *)mm.alloc_copy((std::byte *)this);

            if (copy)
                *copy = *this;

            return copy;
        }

        size_t
        DUniqueString::forward_children(obj<ACollector>) noexcept
        {
            return shallow_size();
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end DUniqueString.cpp */
