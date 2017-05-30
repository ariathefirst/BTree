#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  //constructor, given to us
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
      BTreeNode *right);
  
  //adds to nodes to the left or right of this
  void addLeft(int i, int end);
  void addRight(int i, int end);
  void addHere(int i);
  void insertValue(int i, int &end);

  void shiftAddLeft();
  //addRight doesn't require shifting
  int shiftAddHere(int i);
  int shiftInsertValue(int i);
  //adds a value to this
  
  //return min of a node, given to us  
  int getMinimum() const;

  LeafNode* insert(int i); // returns pointer to new Leaf if splits
  // else NULL


  //checks to see if a node isnt full
  bool hasRoom();

  //checks made before splitting
  bool lookLeft(int i, int end);
  bool lookRight(int i, int end);

  LeafNode* split(int i, int end);
  
  //prints out the node, given to us
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif

