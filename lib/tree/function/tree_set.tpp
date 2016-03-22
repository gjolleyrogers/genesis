/**
 * @brief
 *
 * @file
 * @ingroup tree
 */

#include "tree/function/operators.hpp"

#include <stdexcept>
#include <vector>

namespace genesis {
namespace placement {

// =================================================================================================
//     Tree Set Functions
// =================================================================================================

/**
 * @brief Get the first Tree in a TreeSet that is stored with a given name,
 * or `nullptr` if not found.
 */
template <class TreeSetType>
typename TreeSetType::TreeType* find_tree ( TreeSetType& tset, std::string const& name)
{
    for (auto& tree : tset) {
        if( tree.name == name ) {
            return &tree.tree;
        }
    }
    return nullptr;
}

/**
 * @brief Get the first Tree in a TreeSet that is stored with a given name,
 * or `nullptr` if not found.
 */
template <class TreeSetType>
typename TreeSetType::TreeType const* find_tree ( TreeSetType const& tset, std::string const& name)
{
    for (auto& tree : tset) {
        if( tree.name == name ) {
            return &tree.tree;
        }
    }
    return nullptr;
}

/**
 * @brief Return a Tree where the branch lengths are the average of the Tree%s in the TreeSet,
 * given that they all have the same topology.
 *
 * The function works only under the following conditions:
 *
 *     * All trees must have the same topology.
 *     * The TreeType must provide a data member `branch_length` for the edges.
 *
 * Otherwise, the function throws an `std::runtime_error`. It does not check for node names,
 * but the returned tree will contain the names of the first tree in the set.
 *
 * TODO this function assumes that the tree edge has a branch_length. move it to default tree.
 */
template <class TreeSetType>
typename TreeSetType::TreeType average_branch_length_tree( TreeSetType const& tset )
{
    using TreeType = typename TreeSetType::TreeType;

    if( tset.size() == 0 ) {
        return TreeType();
    }

    if( ! all_identical_topology( tset )) {
        throw std::runtime_error( "Trees in TreeSet do not have the same topology." );
    }

    // Prepare storage for average branch lengths.
    size_t num_edges = tset.at(0).tree.edge_count();
    auto avgs = std::vector<double>(num_edges, 0.0);

    // We traverse all trees (again, because all_identical_topology() already did this). This is
    // probably a bit slower than the previous version of this method which worked with less
    // traversals, but way easier to understand and debug.
    for( auto& ct : tset ) {
        // Use an index for position in the preorder traversal. This makes sure that the
        // index actually always points to the correct edges, indepently of their order in
        // different trees in the set.
        size_t idx = 0;

        // Do a preorder traversal and collect branch lengths.
        for( auto it : preorder(ct.tree) ) {
            // The first iteration points to an edge which will be covered later again.
            // Skip it to prevent double coverage.
            if (it.is_first_iteration()) {
                continue;
            }

            avgs[idx] += it.edge().data.branch_length;
            ++idx;
        }
    }

    // We know that all trees have the same topology. So we take a copy of the first one
    // (thus, also copying its node names) and modify its branch lengths.
    TreeType tree = TreeType( tset.at(0).tree );

    // Do the same kind of traversal as before in order to keep the indexing order (preorder) and
    // set the branch lengths.
    size_t idx = 0;
    for( auto it : preorder(tree) ) {
        // The first iteration points to an edge which will be covered later again.
        // Skip it to prevent double coverage.
        if (it.is_first_iteration()) {
            continue;
        }

        it.edge().data.branch_length = avgs[idx] / tset.size();
        ++idx;
    }

    return tree;
}

// =================================================================================================
//     Comparators
// =================================================================================================

/**
 * @brief Compare whether all Tree%s in a TreeSet are equal using a given comparator functional.
 *
 * See Tree::equal() for more information.
 */
template <class TreeSetType>
bool all_equal(
    TreeSetType const& tset,
    std::function<bool(
        typename TreeSetType::TreeType::NodeType const&,
        typename TreeSetType::TreeType::NodeType const&
    )> node_comparator,
    std::function<bool(
        typename TreeSetType::TreeType::EdgeType const&,
        typename TreeSetType::TreeType::EdgeType const&
    )> edge_comparator
) {
    // If all pairs of two adjacent trees are equal, all of them are.
    // Thus, we do not need a complete pairwise comparision.
    // TODO the namespace thing is weird, but currently neccesary because of an ambiguous call...
    for (size_t i = 1; i < tset.size(); i++) {
        if( ! tree::equal( tset[i-1].tree, tset[i].tree, node_comparator, edge_comparator )) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Compare whether all Tree%s in a TreeSet are equal using their default comparision
 * operators for nodes and edges.
 */
template <class TreeSetType>
bool all_equal( TreeSetType const& tset )
{
    // If all pairs of two adjacent trees are equal, all of them are.
    // Thus, we do not need a complete pairwise comparision.
    for (size_t i = 1; i < tset.size(); i++) {
        if( ! equal( tset[i-1].tree, tset[i].tree )) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Return true iff all Tree%s in a TreeSet have an identical topology.
 */
template <class TreeSetType>
bool all_identical_topology( TreeSetType const& tset )
{
    // If all pairs of two adjacent trees have same the topology, all of them have.
    // Thus, we do not need a complete pairwise comparision.
    for (size_t i = 1; i < tset.size(); i++) {
        if( ! identical_topology( tset[i-1].tree, tset[i].tree )) {
            return false;
        }
    }
    return true;
}

} // namespace tree
} // namespace genesis
