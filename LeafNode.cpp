#include <iostream>
#include <climits>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

/*

void LeafNode::addToLeft(int value, int last)
{
  leftSibling->insert(values[0]);

  for(int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  values[count - 1] = last;
  if(parent)
    parent->resetMinimum(this);
} // LeafNode::ToLeft()

void LeafNode::addToRight(int value, int last)
{
  rightSibling->insert(last);

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToRight()

void LeafNode::addToThis(int value)
{
  int i;

  for(i = count - 1; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];

  values[i + 1] = value;
  count++;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToThis()


void LeafNode::addValue(int value, int &last)
{
  int i;

  if(value > values[count - 1])
    last = value;
  else
  {
    last = values[count - 1];

    for(i = count - 2; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];
    // i may end up at -1
    values[i + 1] = value;
  }
} // LeafNode:: addValue()

*/

int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()

/*

LeafNode* LeafNode::insert(int value)
{
  int last;

  if(count < leafSize)
  {
    addToThis(value);
    return NULL;
  } // if room for value

  addValue(value, last);

  if(leftSibling && leftSibling->getCount() < leafSize)
  {
    addToLeft(value, last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < leafSize)
    {
      addToRight(value, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(value, last);
}  // LeafNode::insert()

*/

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


LeafNode* LeafNode::split(int i, int end){
  LeafNode *created = new LeafNode(leafSize, parent, this, rightSibling);

  //put larger half into newly created node
  for(int x = (leafSize + 1) / 2; x < leafSize; x++){
    created->values[created->count++] = values[x];

    //update size of count as elements are inserted
  }

  //input end value on new node
  created->values[created->count++] = end;

  //the count of this node will always be the same after splitting
  count = (leafSize+1)/2;

  //set right sibling's left sibling to the newly created sibling 
  if(rightSibling != NULL){
    rightSibling->setLeftSibling(created);
  }

  //right pointer is now the newly created node
  rightSibling = created;

  //make sure parent has right min for this leaf node
  if(i == values[0] && parent != NULL){
    parent->resetMinimum(this);
  }

  return created;
}

void LeafNode::addLeft(int i, int end){
  //save off value to add to left sibling
  int temp = values[0];
  leftSibling->insert(temp);

  //shift values over
  shiftAddLeft();

  //insert value at the end, dont have to
  //update count bc it remains the same
  values[count - 1] = end;

  //make sure parent has right min for this leaf node
  if(parent != NULL){
    parent->resetMinimum(this);
  }
}

void LeafNode::addRight(int i, int end){
  //add end value to right sibling
  rightSibling->insert(end);

  //make sure parent has right min for this leaf node
  if(i == values[0] && parent != NULL){
    parent->resetMinimum(this);
  }
}

void LeafNode::addHere(int i){
  //shift all values
  int x = shiftAddHere(i);

  //put in value where loop stopped
  values[x + 1] = i;

  //increase count of items by 1
  count++;

  //make sure parent has right min for this leaf node
  if(i == values[0] && parent != NULL){
    parent->resetMinimum(this);
  }
}

void LeafNode::insertValue(int i, int &end){
  int x;
  //set the value at the end to be max
  if(i > values[count - 1]){
    end = i;
  }
  else{
    //if value inserted is not the greatest,
    //the value at the end is max
    end = values[count - 1];

    x = shiftInsertValue(i);

    //put value in correct place
    values[x + 1] = i;
  }


}

//shift over all the values in the array by 1
//will overwrite first value so be sure to save
//before calling this fucntion
void LeafNode::shiftAddLeft(){
  int x, y;
  for(x = 0; x + 1 < count; x++){
    //get value 1 ahead
    y = values[x+1];

    //reassign value 1 space over
    values[x] = y;
  }
}


int LeafNode::shiftAddHere(int i){
  int x;
  //shifts items over to the right
  for(x = count - 1; x >= 0; x--){
    //if the previous value in the array is smaller
    //than the value being inserted, stop as it
    //has found its correct place
    if(values[x] <= i) {
      break;
    }

    values[x+1] = values[x];
  }

  //this will be the index in values where x fits in
  return x;
}


int LeafNode::shiftInsertValue(int i){
  int x;

  //shifts values over from the right until
  //the value finds its correct place
  for(x = count - 2; x >= 0; x--){
    if(values[x] <= i){
      break;
    }

    values[x+1] = values[x];
  }

  //this will be index where x fits in
  return x;
}


LeafNode* LeafNode::insert(int i){
  int end;

  //if there's space insert
  if(hasRoom()){
    addHere(i);
    return NULL;
  }

  //add value into array before looking left and right
  insertValue(i, end);

  if(lookLeft(i, end)){
    return NULL;
  }
  if(lookRight(i, end)){
      return NULL;
  }

  //if this line is reached, the only thing left to do is split
  return split(i, end);
}  // LeafNode::insert()

bool LeafNode::hasRoom(){
  if(count < leafSize)
    return true;
  else
    return false;
}

bool LeafNode::lookLeft(int i, int end){
  if(leftSibling != NULL && leftSibling->getCount() < leafSize){
    addLeft(i, end);
    return true;
  }

  return false;
}

bool LeafNode::lookRight(int i, int end){
  if(rightSibling != NULL && rightSibling->getCount() < leafSize){
    addRight(i,end);
    return true;
  }

  return false;
}
