/** @file Reader.hpp
 *
 *  @author Roland Conybeare, Jan 2026
 **/

#include <xo/tokenizer2/Tokenizer.hpp>

namespace xo {
    namespace scm {
        /** @class Reader
         *  @brief Assemble Schematika expressions from lexical tokens
         **/
        class Reader {
        public:
        private:
            /** tokenizer: assembles Schematika tokens from text **/
            Tokenizer tokenizer_;

            /** parser: assemble Schematika expressions from token sequences **/
            ExpressionParser parser_;
        };
    } /*namespace scm*/
} /*namespace xo*/

/* end Reader.hpp */
