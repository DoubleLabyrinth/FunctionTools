#ifndef BINARYTREE_H_INCLUDED
#define BINARYTREE_H_INCLUDED

template <typename _Type>
class BinaryTreeNode{
public:
    BinaryTreeNode<_Type>* LeftChildNode = nullptr;
    _Type NodeData;
    BinaryTreeNode<_Type>* RightChildNode = nullptr;

    BinaryTreeNode() {

    }

    BinaryTreeNode(const _Type& srcData, const BinaryTreeNode<_Type>* srcLeftChildNode = nullptr,
                                         const BinaryTreeNode<_Type>* srcRightChildNode = nullptr) {
        NodeData = srcData;
    }

    BinaryTreeNode(const struct BinaryTreeNode<_Type>& srcNode) = delete;

    ~BinaryTreeNode() {
        delete LeftChildNode;
        delete RightChildNode;
    }

    BinaryTreeNode operator=(const BinaryTreeNode& srcNode) = delete;
};

template <typename _Type>
class BinaryTree{
public:
    BinaryTreeNode<_Type>* RootNode;

    BinaryTree() {
        RootNode = nullptr;
    }

    BinaryTree(const _Type& srcData) {
        RootNode = new BinaryTreeNode<_Type>;
        RootNode->NodeData = srcData;
    }

    ~BinaryTree() {
        delete RootNode;
    }

};

#endif //BINARYTREE_H_INCLUDED
