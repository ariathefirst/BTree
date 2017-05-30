#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()

int InternalNode::getMinimum() const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()

void InternalNode::addLeft(BTreeNode *end){
  //rewritten from leafnode
  // BTreeNode* temp = children[0];
  ((InternalNode*)leftSibling)->insert(children[0]);

  shiftAddLeft();

  children[count - 1] = end; 
  keys[count - 1] = end->getMinimum();
  
  end->setParent(this);

  if(parent != NULL){
    parent->resetMinimum(this);
  }
}

void InternalNode::addRight(BTreeNode* p, BTreeNode* end){
  //rewritten from leafnode
  ((InternalNode*) rightSibling)->insert(end);

  if(p == children[0] && parent != NULL){
    parent->resetMinimum(this);
  }
}

BTreeNode* InternalNode::insertPointer(BTreeNode* p, int position){
  //based off of leafnode
  if(position == internalSize)
    return p;
  
  BTreeNode* end = children[count - 1];

  shiftInsertPointer(position);

  children[position] = p;
  keys[position] = p->getMinimum();
  p->setParent(this);
  return end;
}

//shift over all the values in the array by 1
//will overwrite first value so be sure to save
//before calling this fucntion
void InternalNode::shiftAddLeft(){
  int x;
  for(x = 0; x < count - 1; x++){
    //reassign child values
    children[x] = children[x+1];

    //reassign key values
    keys[x] = keys[x + 1];
  }
}


void InternalNode::shiftInsertPointer(int position){
  for(int x = count - 2; x >= position; x--){
    children[x+1] = children[x];
    keys[x+1] = keys[x];
  }
}

InternalNode* InternalNode::insert(int i){
  int position;

  for(position = count - 1; position > 0; position--){
    if(keys[position] <= i){
      break;
    }
  }

  BTreeNode *end, *p = children[position]->insert(i);
  
  if(p == NULL)
    return NULL;

  if(hasRoom()){
    addHere(p, position+1);
    return NULL;
  }

  end = insertPointer(p, position + 1);

  if(lookLeft(end)){
    return NULL;
  }
  else if(lookRight(p, end)){
    return NULL;
  }

  //if this line is reached, the only thing left to do is split
  return split(end);
}  // LeafNode::insert()


bool InternalNode::hasRoom(){
  if(count < internalSize)
    return true;
  else
    return false;
}

bool InternalNode::lookLeft(BTreeNode *end){
  if(leftSibling != NULL && leftSibling->getCount() < internalSize){
    addLeft(end);
    return true;
  }
  else{
    return false;
  }
}

bool InternalNode::lookRight(BTreeNode* p, BTreeNode* end){
  if(rightSibling != NULL && rightSibling->getCount() < internalSize){
    addRight(p, end);
    return true;
  }
  else{
    return false;
  }
}

void InternalNode::insert(BTreeNode* prevRoot, BTreeNode* n){
  keys[0] = prevRoot->getMinimum();
  keys[1] = n->getMinimum();

  children[0] = prevRoot;
  children[0]->setLeftSibling(NULL);
  count =2; 

  children[1] = n;
  children[0]->setRightSibling(children[1]);

  children[1]->setLeftSibling(children[0]);
  children[1]->setRightSibling(NULL);

  prevRoot->setParent(this);
  n->setParent(this);
}

void InternalNode::shiftAddHere(int i){
  for(int x = count - 1; x >= i; x--){
    children[x+1] = children[x];
    keys[x+1] = keys[x];
  }
}

void InternalNode::addHere(BTreeNode* p, int position){
  //rewritten from leafnode
  shiftAddHere(position);
  
  keys[position] = p->getMinimum();
  children[position] = p;

  count++;
  p->setParent(this);

  //make sure parent has right min for this node
  if(position == 0 && parent != NULL){
    parent->resetMinimum(this);
  }
}

void InternalNode::insert(BTreeNode *n){
  if(n->getMinimum() > keys[0]){
    addHere(n, count);
    return;
  }
  else{
    addHere(n, 0);
    return;
  }
}

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


void InternalNode::resetMinimum(const BTreeNode* child)
{
  for(int i = 0; i < count; i++)
    if(children[i] == child)
    {
      keys[i] = children[i]->getMinimum();
      if(i == 0 && parent)
        parent->resetMinimum(this);
      break;
    }
} // InternalNode::resetMinimum()

InternalNode* InternalNode::split(BTreeNode *end)
{
  InternalNode *created = new InternalNode(internalSize, leafSize,
    parent, this, rightSibling);


  if(rightSibling != NULL)
    rightSibling->setLeftSibling(created);

  rightSibling = created;

  for(int i = (internalSize + 1) / 2; i < internalSize; i++)
  {
    created->children[created->count] = children[i];
    created->keys[created->count++] = keys[i];
    children[i]->setParent(created);
  }

  created->children[created->count] = end;
  created->keys[created->count++] = end->getMinimum();
  end->setParent(created);
  count = (internalSize + 1) / 2;
  return created;
} // split()
