/**
 * @brief
 *
 * @file
 * @ingroup utils
 */

#include "utils/io/json/writer.hpp"

#include <assert.h>
#include <fstream>
#include <ostream>
#include <stdexcept>

#include "utils/core/fs.hpp"
#include "utils/io/json/document.hpp"
#include "utils/text/string.hpp"

namespace genesis {
namespace utils {

// =================================================================================================
//     Writing
// =================================================================================================

/**
 * @brief Write a JsonDocument to a stream.
 */
void JsonWriter::to_stream( JsonDocument const& document, std::ostream& out ) const
{
    print_object( &document, out, 0 );
}

/**
 * @brief Write a JsonDocument to a file.
 *
 * If the file already exists or cannot be written to, the function throws `std::runtime_error`.
 */
void JsonWriter::to_file( JsonDocument const& document, std::string const& filename ) const
{
    if( utils::file_exists(filename) ) {
        throw std::runtime_error( "Json file '" + filename + "' already exist." );
    }

    std::ofstream fstr( filename );
    if( !fstr ) {
        throw std::runtime_error( "Cannot write Json file '" + filename + "'." );
    }
    print_object( &document, fstr, 0 );
}

/**
 * @brief Give the Json string representation of a JsonDocument.
 */
void JsonWriter::to_string( JsonDocument const& document, std::string& output ) const
{
    output = to_string(document);
}

/**
 * @brief Return the Json representation of a JsonDocument.
 */
std::string JsonWriter::to_string( JsonDocument const& document ) const
{
    std::stringstream sstr;
    print_object( &document, sstr, 0 );
    return sstr.str();
}

// =================================================================================================
//     Printing
// =================================================================================================

/**
 * @brief Write the Json representation of a Json Value to a stream.
 */
void JsonWriter::print_value(
    JsonValue const* value,
    std::ostream& out
) const {
    switch(value->type()) {
        case JsonValue::kNull:
        case JsonValue::kBool:
            out << value->to_string();
            break;

        case JsonValue::kNumber:
            out << utils::to_string_precise(json_value_to_number(value)->value, precision);
            break;

        case JsonValue::kString:
            out << "\"" + utils::escape(json_value_to_string(value)->value) + "\"";
            break;

        // This function is only called from within print_array() and print_object(), and both of
        // them handle the following two cases separately. So the assertion holds as long as this
        // function is not called illegaly from a different context.
        // Also, add a return to make the compiler happy ;-)
        case JsonValue::kArray:
        case JsonValue::kObject:
        default:
            assert(false);
    }
}

/**
 * @brief Write the Json representation of a Json Array to a stream.
 */
void JsonWriter::print_array(
    JsonValueArray const* value,
    std::ostream& out,
    int indent_level
) const {
    int il = indent_level + 1;
    std::string in (il * indent, ' ');

    // Check if array contains non-simple values. If so, we use better bracket
    // placement to make document look nicer.
    bool has_large = false;
    for (JsonValueArray::const_iterator it = value->cbegin(); it != value->cend(); ++it) {
        JsonValue* v = *it;
        has_large |= (v->is_array() || v->is_object());
    }

    out << "[ ";
    bool first = true;
    for (JsonValueArray::const_iterator it = value->cbegin(); it != value->cend(); ++it) {
        JsonValue* v = *it;
        if (!first) {
            out << ", ";
        }
        if (has_large) {
            out << "\n" << in;
        }
        if (v->is_array()) {
            print_array(json_value_to_array(v), out, il);
        } else if (v->is_object()) {
            print_object(json_value_to_object(v), out, il);
        } else {
            print_value(v, out);
        }
        first = false;
    }

    if (has_large) {
        out << "\n" << std::string(indent_level * indent, ' ');
    } else {
        out << " ";
    }
    out << "]";
}

/**
 * @brief Write the Json representation of a Json Object to a stream.
 */
void JsonWriter::print_object(
    JsonValueObject const* value,
    std::ostream& out,
    int indent_level
) const {
    int il = indent_level + 1;
    std::string in (il * indent, ' ');
    out << "{";

    bool first = true;
    for (JsonValueObject::const_iterator it = value->cbegin(); it != value->cend(); ++it) {
        JsonValueObject::ObjectPair v = *it;
        if (!first) {
            out << ",";
        }
        out << "\n" << in << "\"" << v.first << "\": ";
        if (v.second->is_array()) {
            print_array( json_value_to_array(v.second), out, il );
        } else if (v.second->is_object()) {
            print_object( json_value_to_object(v.second), out, il );
        } else {
            print_value( v.second, out );
        }
        first = false;
    }

    out << "\n" << std::string(indent_level * indent, ' ') << "}";
}

} // namespace utils
} // namespace genesis
