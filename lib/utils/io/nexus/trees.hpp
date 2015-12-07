#ifndef GENESIS_UTILS_IO_NEXUS_TREES_H_
#define GENESIS_UTILS_IO_NEXUS_TREES_H_

/**
 * @brief
 *
 * @file
 * @ingroup utils
 */

#include "utils/io/nexus/block.hpp"

#include "utils/core/std.hpp"

#include <algorithm>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace genesis {
namespace nexus {

// =================================================================================================
//     Nexus Block Trees
// =================================================================================================

/**
 * @brief
 */
class Trees : public Block
{

    // -----------------------------------------------------
    //     Member Types
    // -----------------------------------------------------

public:

    struct Entry
    {
        std::string name;
        std::vector<std::string> properties;
        std::string tree;
    };

    using container      = std::vector<Entry>;
    using const_iterator = container::const_iterator;

    // -----------------------------------------------------
    //     Constructor and Rule of Five
    // -----------------------------------------------------

public:

    Trees() = default;
    ~Trees() = default;

    Trees( Trees const& ) = default;
    Trees( Trees&& )      = default;

    Trees& operator= ( Trees const& ) = default;
    Trees& operator= ( Trees&& )      = default;

    void swap( Trees& other )
    {
        using std::swap;
        swap( entries_, other.entries_ );
    }

    // -----------------------------------------------------
    //     Operators
    // -----------------------------------------------------

public:

    // friend bool operator == ( Trees const& lhs, Trees const& rhs )
    // {
    //     return lhs.entries_ == rhs.entries_;
    // }
    //
    // friend bool operator != ( Trees const& lhs, Trees const& rhs )
    // {
    //     return !(lhs == rhs);
    // }

    // -----------------------------------------------------
    //     Accessors
    // -----------------------------------------------------

public:

    bool empty() const
    {
        return entries_.empty();
    }

    size_t size() const
    {
        return entries_.size();
    }

    bool has_tree( std::string name ) const
    {
        return entries_.end() != std::find_if(
            entries_.begin(),
            entries_.end(),
            [&name] (Entry const& entry) {
                return entry.name == name;
            }
        );

        // for( auto& entry : entries_ ) {
        //     if( entry.name == name ) {
        //         return true;
        //     }
        // }
        // return false;
    }

    // -----------------------------------------------------
    //     Iterators
    // -----------------------------------------------------

public:

    const_iterator begin() const
    {
        return entries_.cbegin();
    }

    const_iterator end() const
    {
        return entries_.cend();
    }

    // -----------------------------------------------------
    //     Modifiers
    // -----------------------------------------------------

public:

    void add_tree( std::string name, std::string tree )
    {
        if( has_tree(name) ) {
            throw std::invalid_argument(
                "Tree with name '" + name + "' already exists in this tree block."
            );
        }

        // auto e = Entry();
        // e.name = name;
        // e.tree = tree;
        // entries_.push_back(e);

        entries_.push_back({ name, {}, tree });
        // entries_.emplace_back(name, {}, tree);
    }

    void erase_tree( std::string name )
    {
        erase_if(
            entries_,
            [&name] (Entry const& entry) {
                return entry.name == name;
            }
        );
    }

    void clear()
    {
        entries_.clear();
    }

    // -----------------------------------------------------
    //     Virtual Functions
    // -----------------------------------------------------

public:

    std::string block_name() const override
    {
        return "TREES";
    }

    void to_stream( std::ostream& os ) const override
    {
        for( auto& entry : entries_ ) {
            os << "    TREE " << entry.name << " = ";
            for( auto& prop : entry.properties ) {
                os << "[" << prop << "] ";
            }
            os << entry.tree << "\n";
        }
    }

    // -----------------------------------------------------
    //     Data Members
    // -----------------------------------------------------

private:

    container entries_;

};

// =================================================================================================
//     Operators
// =================================================================================================

inline void swap( Trees& lhs, Trees& rhs )
{
    lhs.swap(rhs);
}

} // namespace nexus
} // namespace genesis

#endif // include guard