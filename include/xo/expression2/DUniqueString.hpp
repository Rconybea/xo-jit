/** @file DUniqueString.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#pragma once

#include <xo/object2/DString.hpp>

namespace xo {
    namespace scm {
        /** @class DUniqueString
         *  @brief unique immutable string
         *
         *  A DUniqueString is an immutable string stored in a shared StringTable.
         *  Follows that DUniqueStrings at different memory locations
         *  have different contents.
         *
         *  DUniqueString instances will be created by StringTable (see also).
         *  Application code will not allocate them directly.
         *
         *  Needs to be gc-aware so that collector knows what to do when it encounters
         *  a obj<AGCObject> with a DUnqiueString data pointer; such instances
         *  will not be allocated from GC memory
         **/
        class DUniqueString {
        public:
            using AAllocator = xo::mm::AAllocator;
            using ACollector = xo::mm::ACollector;
            using size_type = DString::size_type;
            using ppindentinfo = xo::print::ppindentinfo;

            /* Memory model for a DUniqueString allocated via xo allocator
             *
             *    0               8              16      20      24           24+z
             *    v               v               v       v       v           v
             *    +---------------+-+-------------+-------+-------+-----------+
             *    |     header    |u|  padding    |  cap  |  size | text... \0|
             *    +---------------+-+-------------+-------+-------+-----------+
             *
             * Legend
             *   header   8 byte   allocation header
             *   u        1 byte   DUniqueString placeholder (c++ insists)
             *   padding  7 bytes  allocator-imposed padding to 8-byte alignment
             *   cap      4 bytes  DString.capacity
             *   size     4 bytes  DString.size
             *   text     z bytes  DString.size bytes of text (including null)
             *                     In practice followed by padding to 8 byte
             *                     alignment
             */

            /** @defgroup duniquestring-ctors constructors **/
            ///@{

            /** not copyable **/
            DUniqueString(const DUniqueString &) = delete;

            ///@}
            /** @defgroup duniquestring-methods methods **/
            ///@{

            /** Available storage for this instance.
             *  For completeness' sake since uniquestring not modifiable
             **/
            size_type capacity() const noexcept { return _text()->capacity(); }
            size_type size() const noexcept { return _text()->size(); }
            const char * chars() const noexcept { return _text()->chars(); }

            /** compare unique strings: return n with {n<0, n=0, n>0}
             *  when @p lhs lexicographically {before, at, after} @p rhs
             **/
            static int compare(const DUniqueString & lhs, const DUniqueString & rhs) {
                if (&lhs == &rhs)
                    return 0;

                return DString::compare(*(lhs._text()), *(rhs._text()));
            }

            std::size_t hash() const noexcept { return _text()->hash(); }
            operator std::string_view() const noexcept { return std::string_view(*_text()); }
            /** not assignable **/
            DUniqueString & operator=(const DUniqueString &) = delete;

            ///@}
            /** @defgroup duniquestring-printable-methods printable facet methods **/
            ///@{

            bool pretty(const ppindentinfo & ppii) const;

            ///@}
            /** @defgroup duniquestring-gcobject-methods gcobject facet methods **/
            ///@{

            std::size_t shallow_size() const noexcept;

            /** clone unique string, using memory from allocator @p mm. **/
            DUniqueString * shallow_copy(obj<AAllocator> mm) const noexcept;

            /** fixup child pointers (trivial for DUniqueString, no gc-owned children **/
            std::size_t forward_children(obj<ACollector> gc) noexcept;

            ///@}

        private:
            /** @defgroup duniquestring-impl-methods implementation methods **/
            ///@{

            /** default ctor **/
            DUniqueString() = default;

            /** DString containing actual string content immediately follows DUniqueString
             *  in memory; part of same alloc
             **/
            DString * _text() const noexcept;

            //explicit DUniqueString(const DString * text) : text_{text} {}

            /** create instance using memory from @p mm,
             *  with string contents copied from @p sv
             **/
            static DUniqueString * from_view(obj<AAllocator> mm,
                                             std::string_view sv);

            ///@}

            friend class StringTable;

        private:
#ifdef NOPE
            /** interned string.  Note stringtable memory distinct from gc memory,
             *  so gc will not (and should not) traverse this pointer.
             **/
            const DString * text_ = nullptr;
#endif
        };

        /* since unique: just compare addresses */
        inline bool operator==(const DUniqueString & lhs, const DUniqueString & rhs) {
            return (&lhs == &rhs);
        }

        /* since unique: just compare addresses **/
        inline bool operator!=(const DUniqueString & lhs, const DUniqueString & rhs) {
            return (&lhs != &rhs);
        }

        inline bool operator<=(const DUniqueString & lhs, const DUniqueString & rhs) {
            return (DUniqueString::compare(lhs, rhs) <= 0);
        }
    } /*namespace scm*/
} /*namespace xo*/

/* end UniqueString.hpp */
