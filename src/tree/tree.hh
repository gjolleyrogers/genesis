#ifndef GNS_TREE_TREE_H_
#define GNS_TREE_TREE_H_

/**
 * @brief Provides the basic interface for phylogenetic trees.
 *
 * For more information, see Tree class.
 *
 * @file
 * @ingroup tree
 */

#include <string>
#include <vector>

#include "tree/tree_branch.hh"
#include "tree/tree_broker.hh"
#include "tree/tree_link.hh"
#include "tree/tree_node.hh"

namespace genesis {

class Tree
{
public:
    Tree() {};
    virtual ~Tree();
    void clear();

    bool FromNewickFile (const std::string& fn);
    void ToNewickFile   (const std::string& fn);

    bool FromNewickString (const std::string& tree);
    std::string ToNewickString ();

    void FromTreeBroker (const TreeBroker& broker);

protected:
    void InitBifurcatingTree(const int leaf_count);

    std::vector<TreeLink*>   links_;
    std::vector<TreeNode*>   nodes_;
    std::vector<TreeBranch*> branches_;
};

} // namespace genesis

#endif // include guard
