/**
 * @brief
 *
 * @file
 * @ingroup utils
 */

#include "utils/io/lexer/parser.hpp"

#include <assert.h>
#include <cctype>
#include <limits>
#include <stdexcept>

namespace genesis {
namespace utils {

// =================================================================================================
//     String
// =================================================================================================

/**
 * @brief Read a string in quotation marks from a stream and return it.
 *
 * The first char of the stream is considered to be the opening quotation mark. Everything up until
 * the closing quotation mark (the same char again, whatever it is) is then read.
 *
 * If the optional parameter `use_escapes` is set to `true`, chars preceeded by a backslash are
 * considered to be "escaped". The chars `\r`, `\n` and `\t` are then turned into their
 * respective white space equivalents, while all other chars are copied verbatim to the return
 * string. Thus, by escaping it, a the string can also include the quotation mark itself.
 * Default of this parameter is `true`.
 *
 * The optional parameter `include_qmarks` demtermines whether the quotation marks are included in
 * the output or not. Default is `false`.
 *
 * If the string ends prematurely, i.e., without the closing quotation mark, or right after a
 * backslash if `use_escapes` is used, the function throws an `std::runtime_error`.
 */
std::string parse_quoted_string(
    utils::CountingIstream& source,
    bool use_escapes,
    bool include_qmarks
) {
    if( !source ) {
        return "";
    }

    // Read the introductory quotation mark. We will read until it appears again.
    char qmark = *source;
    ++source;

    // Prepare the return value.
    std::string value;
    if( include_qmarks ) {
        value += qmark;
    }

    while( source && *source != qmark ) {
        // Treat escape sequences.
        if( *source == '\\' && use_escapes ) {

            ++source;

            // We found an escaping backslash. This cannot be the end of the stream.
            if( !source ) {
                throw std::runtime_error(
                    "Unexpected end of string at " + source.at() + "."
                );
            }

            switch( *source ) {
                case 'r' :
                    value += '\r';
                    break;

                case 'n' :
                    value += '\n';
                    break;

                case 't' :
                    value += '\t';
                    break;

                default :
                    value += *source;
            }
            ++source;

        } else {
            // Treat normal (non-escape) chars.
            value += *source;
            ++source;
        }
    }

    // We need to find the closing qmark, otherwise it's an error.
    if( !source ) {
        throw std::runtime_error(
            "Unexpected end of string at " + source.at() + "."
        );
    }

    assert( source && *source == qmark );
    ++source;

    // Finish the return value.
    if( include_qmarks ) {
        value += qmark;
    }
    return value;
}

} // namespace utils
} // namespace genesis
