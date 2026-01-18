/** @file ExpressionParser.hpp
*
 *  @author Roland Conybeare, Jan 2026
 **/

#include "ExprState.hpp"
#include <xo/alloc2/Allocator.hpp>
#include <xo/alloc2/arena/IAllocator_DArena.hpp>
#include <xo/facet/obj.hpp>

namespace xo {
    namespace scm {
        /** @class ExpressionParser
         *  @brief Assemble Schematika expressions from token sequences
         *
         *  Parser represents Each partially assembled expression by
         *  an ExprState object.
         *  Expreesions form a tree:
         *  each expression belongs to at most one parent.
         *
         **/
        class ExpressionParser {
        public:
            void push_exprstate(obj<AExprState> xstate);

        private:
            /* TODO:
             *   ASymbolTable
             *   DLocalSymtab
             *   DGlobalSymtab
             *
             * Will also need
             *   DVariable
             *   DLambda
             *
             * For DGlobalSymtab perhaps use DArenaHashMap.
             * May also want to use DArenaHashMap+DArena to intern strings
             *
             * Also:
             *   TypeUnifier
             */

            /** Arena for internal parsing stack.
             *  Must be owned exclusively because destructively
             *  modified as parser completes parsing of each sub-expression
             *
             *  Contents will be a stack of ExprState instances
             **/
            DArena parser_alloc_;

#ifdef NOT_YET
            /** Arena for internal environment stack.
             *  This represents just nesting for environments.
             *  Details for each frame survive parsing and are
             *  stored in @ref expr_alloc_.
             *  Maybe that means we don't need env_alloc_
             **/
            DArena env_alloc_;
#endif

            /** Allocator for parsed expressions.
             *  Information available during subsequent execution
             *  (whether compiling or interpreting) must be stored here.
             *
             *  Also use this allocator for error messages arising
             *  during parsing
             *
             *  Memory use patterns for executions are not predictable,
             *  and require garbage collection, e.g. DX1Collector.
             *
             *  May alternatively be able to use DArena in a compile-only
             *  scenario, where top-level Expressions can be discarded
             *  once compiled.
             **/
            obj<AAllocator> expr_alloc_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end ExpressionParser.hpp */
