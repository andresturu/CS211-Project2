/*map.h*/

#pragma once

#include <iostream>
#include <utility>  // pair, make_pair()

using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

//
// class map:
//
template<typename KeyT, typename ValueT>
class map {
private:

  struct NODE {
    pair<KeyT, ValueT> Pair;
    NODE*              Left;
    NODE*              Right;

    NODE(KeyT key, ValueT value)
      : Pair(make_pair(key, value)), Left(nullptr), Right(nullptr)
    { }
  };

  //
  // data members:
  //
  NODE*   Root;  // pointer to root of tree
  size_t  Size;  // # of nodes in tree

  void _print

public:
  // 
  // constructor:
  //
  map()
    : Root(nullptr), Size(0)
  { }

  //
  // copy constructor:
  //
  map(const map& other)
  {
    // 
    // TODO:
    //
  }

  //
  // destructor:
  //
  ~map()
  {
    // 
    // TODO:
    //
  }

  //
  // size:
  //
  size_t size() 
  {
    return this->Size;
  }

  //
  // []: search, insert if not found
  //
  ValueT& operator[](KeyT key)
  {
    //
    // DELETE THIS CODE, it's incorrect and here so the
    // released code compiles and runs:
    //

    NODE* prev = nullptr;
    NODE* cur = this->Root;
    while(cur != nullptr) {
      if (key == cur->Pair.first) {
        return cur->Pair.second;
      }
      else if (key < cur->Pair.first) {
        prev = cur;
        go left;
      }
      else {
        prev = cur;
        go right;
      }
    }

    ValueT default_value = ValueT(); // gets the default value based on the type
    NODE* newNode = new NODE(key, default_value); // 

    if(prev == nullptr)
      this->Root = newNode;
    else if (key < prev->Pair.first)
      prev->Left = newNode;
    else
      prev->Right = newNode;

    this->Root = newNode; // 
    this->Size++; // increases size

    return newNode->Pair.second; //gets the value within the Pair class, which is the value

    //
    // TODO:
    //
  }

  // 
  // print: for debugging output to console
  //
  void print()
  {
    cout << "**Tree Contents**" << endl;
    
      void traverse(NODE* cur) {
      if (cur == nullptr) {
        cout << 
        return;
      } else {
        traverse (left sub-tree);
        visit cur;
        traverse(right sub-tree);
      }
    }
  }

    
};
