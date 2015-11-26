#ifndef GENESIS_TREE_DEFAULT_NEWICK_MIXIN_H_
#define GENESIS_TREE_DEFAULT_NEWICK_MIXIN_H_

/**
 * @brief Header of DefaultTreeNewickAdapter class.
 *
 * @file
 * @ingroup tree
 */

#include "tree/default/tree.hpp"
#include "tree/io/newick/adapter.hpp"
#include "tree/io/newick/processor.hpp"

namespace genesis {

// =================================================================================================
//     Default TreeNewick Adapter
// =================================================================================================

/**
 * @brief
 */
template <typename TreeType>
class DefaultTreeNewickAdapter : public NewickAdapter<TreeType>
{
public:

    std::string default_leaf_name     = "Leaf Node";
    std::string default_internal_name = "Internal Node";
    std::string default_root_name     = "Root Node";

    /**
     * @brief If set to true, unnamed nodes are named using one of the default names.
     *
     * The default names can be set using `default_leaf_name`, `default_internal_name` and
     * `default_root_name`. They are used both when parsing and printing a Newick file.
     */
    bool        use_default_names = false;

    inline bool to_tree_edge (
        NewickBrokerElement const& element, typename TreeType::EdgeType& edge
    ) {
        edge.data.branch_length = element.branch_length;
        return true;
    }

    inline bool to_tree_node (
        NewickBrokerElement const& element, typename TreeType::NodeType& node
    ) {
        node.data.name = element.name;

        // if (node->name.empty() && use_default_names) {
        //     if (node->is_leaf) {
        //         node->name = default_leaf_name;
        //     } else {
        //         node->name = default_internal_name;
        //     }
        // }
        //
        // if (node->name.empty() && use_default_names) {
        //     if (node->is_leaf) {
        //         node->name = default_leaf_name;
        //     } else {
        //         node->name = default_internal_name;
        //     }
        // }
        //
        // if (node->name.empty() && use_default_names) {
        //     node->name = default_root_name;
        // }

        return true;
    }

    inline void from_tree_edge (
        const typename TreeType::EdgeType& edge, NewickBrokerElement& element
    ) {
        NewickAdapter<TreeType>::set_branch_length(edge.data.branch_length, element);
    }

    inline void from_tree_node (
        const typename TreeType::NodeType& node, NewickBrokerElement& element
    ) {
        NewickAdapter<TreeType>::set_name(node.data.name, element);


        // // filter out default names if needed
        // if (!use_default_names && bn.name != "" && (
        //     bn.name == default_leaf_name ||
        //     bn.name == default_internal_name ||
        //     bn.name == default_root_name
        // )) {
        //     bn.name = "";
        // }
    }

};

// =================================================================================================
//     Typedef
// =================================================================================================

typedef NewickProcessor<DefaultTreeNewickAdapter<DefaultTree>> DefaultTreeNewickProcessor;

} // namespace genesis

#endif // include guard
