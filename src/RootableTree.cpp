#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include "../libs/doctest.h"
#include "../libs/Phylolib/includes/tree.h"

class RootableTree
{
private:
    tree _unrootedTree;
    std::vector<tree::TreeNode*> _parents;
    std::vector<tree::TreeNode*> _sons;

public:
    RootableTree(const char* newickTree): _unrootedTree(newickTree, false) {
        _unrootedTree.getAllBranches(_parents, _sons);
    }

    tree rootTreeOnBranch(const size_t branchNum, const MDOUBLE splitPoint) {
        if (branchNum >= _unrootedTree.getNodesNum()-1 || branchNum < 0) throw std::out_of_range("Branch number does not exist");
        if (splitPoint >= 1.0 || splitPoint <= 0.0 ) throw std::out_of_range("split point should be between 0.0 and 1.0 exclusive"); 

        tree newTree;

        newTree.createRootNode();
        newTree.getRoot()->setName("newRoot");

        MDOUBLE branchLength = _sons[branchNum]->dis2father();

        newTree.recursiveBuildTreeUp(newTree.getRoot(), _parents[branchNum], _sons[branchNum]->id());
        newTree.recursiveBuildTree(newTree.getRoot(), _sons[branchNum]);
        newTree.getRoot()->getSon(0)->setDisToFather(branchLength * splitPoint);
        newTree.getRoot()->getSon(1)->setDisToFather(branchLength - (branchLength * splitPoint));

        return newTree;
    }

    ~RootableTree() {};
};




TEST_CASE("testing rooting correctnes with 3 nodes unrooted tree") {
    RootableTree testTree("(A:0.1,B:0.2,C:0.5);");
    SUBCASE("root at A") {
        tree rootedTree = testTree.rootTreeOnBranch(0,0.5);
        std::string treeString = rootedTree.stringTreeInPhylipTreeFormat();
        CHECK(rootedTree.isRooted());
        CHECK(treeString=="((B:0.2,C:0.5):0.05,A:0.05);");
    }
    SUBCASE("root at B") {
        tree rootedTree = testTree.rootTreeOnBranch(1,0.7);
        std::string treeString = rootedTree.stringTreeInPhylipTreeFormat();
        CHECK(rootedTree.isRooted());
        CHECK(treeString=="((A:0.1,C:0.5):0.13999,B:0.06);");
    }

    SUBCASE("root at C") {
        tree rootedTree = testTree.rootTreeOnBranch(2,0.2);
        std::string treeString = rootedTree.stringTreeInPhylipTreeFormat();
        CHECK(rootedTree.isRooted());
        CHECK(treeString=="((A:0.1,B:0.2):0.1,C:0.4);");
    }

}