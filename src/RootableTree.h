

tree::nodeP tree::rootTreeOnBranch(tree &newTree, const size_t branchNum, const MDOUBLE splitPoint) {
	if (branchNum >= getNodesNum()-1 || branchNum < 0) throw std::out_of_range("Branch number does not exist");
	if (splitPoint >= 1.0 || splitPoint <= 0.0 ) throw std::out_of_range("split point should be between 0.0 and 1.0 exclusive"); 

	newTree.createRootNode();
    newTree.getRoot()->setName("newRoot");

    std::vector<tree::TreeNode*> parents;
    std::vector<tree::TreeNode*> sons;

    getAllBranches(parents, sons);


	MDOUBLE branchLength = sons[branchNum]->dis2father();

    newTree.recursiveBuildTreeUp(newTree.getRoot(), parents[branchNum], sons[branchNum]->id());
    newTree.recursiveBuildTree(newTree.getRoot(), sons[branchNum]);
    newTree.getRoot()->getSon(0)->setDisToFather(branchLength * splitPoint);
    newTree.getRoot()->getSon(1)->setDisToFather(branchLength - (branchLength * splitPoint));

	return newTree.getRoot();
}
