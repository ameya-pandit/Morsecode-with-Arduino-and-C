#include "Tree.h"
#include "BOARD.h"
/**
 * This function creates a binary tree of a given size given a serialized array of data. All nodes
 * are allocated on the heap via `malloc()` and store the input data in their data member. Note that
 * this parameter is serialized in a top-down, left-to-right order. This means it follows the
 * left-hand-side of the tree to the bottom and then backs it's way back up.
 *
 * So for a tree like:
 *           A
 *        B     C
 *      D   E F   G
 * The data variable is ordered as [A B D E C F G].
 * 
 * This function uses recursion for generating the tree in this top-down, left-right order. This
 * tree, and therefore the input data set, is assumed to be full and balanced therefore each level
 * has 2^level number of nodes in it. If the `data` input is not this size, then this functions
 * behavior is undefined. Since the input data is copied directly into each node, they can
 * originally be allocated on the stack.
 *
 * If allocating memory fails during TreeCreate() then it will return NULL. Additionally, if the
 * creation of either of the subtrees fails within TreeCreate(), it should return NULL. This means
 * that if TreeCreate() returns a non-NULL value, that means that a perfect tree has been created,
 * so all nodes that should exist have been successfully malloc()'d onto the heap.
 *
 * This function does not completely clean up after itself if malloc() fails at any point. Some
 * nodes may be successfully free()d, but a failing TreeCreate() is likely to leave memory in the
 * heap unaccessible.
 *
 * @param level How many vertical levels the tree will have.
 * @param data A serialized array of the character data that will be stored in all nodes. This array
 *              should be of length `2^level - 1`.
 * @return The head of the created tree or NULL if malloc fails for any node.
 */
Node *TreeCreate(int level, const char *data)   {
    Node *treeNode = malloc(sizeof(Node));              //allocating memory for the treeNodes
    
    if (treeNode == NULL)   {                           //if memory is NULL, return NULL
        return NULL;
    } 
    
    treeNode->data = *data;                             //set treeNode data to the data pointer (in our case the char) passed in
    if (level == 1) {                                   //first level (the blank circle in the PDF), there are no "childs"
        treeNode->leftChild = NULL;
        treeNode->rightChild = NULL;
    } else if (level > 1)   {                           //as levels go up (till 6)
        treeNode->leftChild = TreeCreate((level - 1), data + 1);    //setting leftChild
        treeNode->rightChild = TreeCreate((level - 1), data + (1 << (level - 1)));  //setting rightChild
    }
    return treeNode;                                    //return this value
}