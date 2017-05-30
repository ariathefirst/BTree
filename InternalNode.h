#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);

  void addLeft(BTreeNode *end);
  void addRight(BTreeNode* p, BTreeNode* end);
  void addHere(BTreeNode* p, int position); 
  BTreeNode* insertPointer(BTreeNode* p, int position);

  void shiftAddLeft();
  //shiftAddRight() not needed
  void shiftAddHere(int i); 
  void shiftInsertPointer(int position);


  InternalNode* insert(int i);
  void insert(BTreeNode* prevRoot, BTreeNode* n); //OK
  void insert(BTreeNode *n); 

  bool hasRoom();
  bool lookLeft(BTreeNode* end);
  bool lookRight(BTreeNode* p, BTreeNode* end);
  InternalNode* split(BTreeNode* end); //OK

  int getMinimum() const;
  void resetMinimum(const BTreeNode* n);

  void print(Queue <BTreeNode*> &queue);
}; // InternalNode class

#endif

