/*set.cpp*/

/** 
 * @file set.cpp
 * @brief Set class
 *
 * Implements the notion of a mathematical set of
 * integers: no duplicates, in ascending order.
 * Provides set operations intersection, union, and
 * difference.
 *
 * @author Prof Joe Hummel, Northwestern University
 * @author Andres Turullols
 * @author Charlie Zhang
 * @author Evan Wu
*/

#include <algorithm>

#include "set.h"

using namespace std;

/** 
 * @brief returns # of elements in the set
 *
 * @return # of elements 
 */
size_t Set::size() const
{
  return this->S.size();
}

/** 
 * @brief returns ith element in the set (0-based)
 *
 * @return ith element in the set 
 */
int Set::operator[](int i) const
{
  return this->S[i];
}

/** 
 * @brief returns iterator to start of iteration space
 *
 * @return iterator to start of iteration space
 */
vector<int>::iterator Set::begin()
{
  return this->S.begin();
}

/** 
 * @brief returns iterator to end of iteration space
 *
 * @return iterator to end of iteration space
 */
vector<int>::iterator Set::end()
{
  return this->S.end();
}

/** 
 * @brief searches if e is element of set
 *
 * @return true if e is an element, false if not
 */
bool Set::contains(int e)
{
  bool found=false;
  vector<int>::iterator iter = binarySearch(e, found);
  //circumvent warning -> error message
  iter=S.begin();
  return found;
}

//assumes a sorted vector<int>
vector<int>::iterator Set::binarySearch(const int& e, bool& found) {
  //checks for size 0 and size 1
  if(S.size()==1) {if(S[0]==e) {found=true; return S.begin();} else {found = false; if(S[0]>e) return S.begin();else return S.begin()+1;}}
  int lowerBound=1;
  int upperBound=S.size();
  int pos=(S.size())/2+S.size()%2;
  //checks for endpoints 
   if(S[lowerBound-1]==e) {found = true; return S.begin();}
   if(S[upperBound-1]==e){ found = true; return S.begin()+(upperBound-1);}
   if(S[lowerBound-1]>e){found = false; return S.begin();}
   if(S[upperBound-1]<e){found = false; return S.end();}
   
  while(true){
   if(S[pos-1]==e){
    found=true; 
    return S.begin()+(pos-1);
   }else if(S[pos-1]<e) lowerBound=pos;
   else upperBound=pos;

   if(upperBound-lowerBound==1){found = false; 
    return S.begin()+lowerBound;}
   pos=(lowerBound+upperBound)/2;
  }
}

/** 
 * @brief inserts e into set if not already a member
 *
 * @return void
 */
void Set::insert(int e)
{
  bool found = false;
  if(S.size()==0) {S.push_back(e); return;}
  vector<int>::iterator iter = binarySearch(e, found);

  if (found)
      return;

  S.insert(iter, e);
}

/** 
 * @brief removes e from set if a member
 *
 * @return void
 */
void Set::remove(int e)
{
  // is e in the set?
  bool found;
  vector<int>::iterator iter = binarySearch(e, found);
  if(found){
    this->S.erase(iter);
  }
}

// my solution for intersection() O(n*m)
void Set::intersection(const Set& S2) {
  vector<int> intersected;
  for (size_t i = 0; i < this->size(); i++) {
    int index = static_cast<int>(i);
    int curr_number = this->operator[](index);
    vector<int>::const_iterator iter = std::find(S2.S.begin(), S2.S.end(), curr_number);

    if (iter != S2.S.end()) {
      intersected.push_back(curr_number);
    }
  }
  this->S = intersected;
}
// AI solution
// utilizes two pointers
/*
void Set::intersection(const Set& S2) {
  const std::vector<int>& B = S2.S;
  size_t i = 0, j = 0, k = 0;

  while (i < S.size() && j < B.size()) {
    if (S[i] < B[j])       ++i;
    else if (B[j] < S[i])  ++j;
    else {
      S[k++] = S[i];
      ++i; ++j;
    }
  }
  S.resize(k);
}
*/

void Set::merge(const Set& S2) {
  bool found = false;
  for(int value : S2.S) {
    vector<int>::iterator iter = binarySearch(value, found);
    if(found) {
      continue;
    }
    this -> S.insert(iter, value);
  }
}

bool Set::disjoint(const Set& S2) const{
  size_t i=0;
  size_t j=0;

  while(i<this->S.size()&& j<S2.size()){
    if(this->S[i]==S2[j])return false;
    if(this->S[i]>S2[j]) j++;
    else i++;
  }

  return true;
}