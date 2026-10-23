/*vector.h*/

/**
  * @brief simple implementation of std::vector with just push_back
  *
  * @note since vector is templated, there is no separate .cpp file.
  *
  * @note Prof Joe Hummmel
  * @note Northwestern University
  */

#pragma once

template <typename T>
class vector
{
private:
  T* A;             // ptr to underlying array
  size_t Size;      // # of elements in array
  size_t Capacity;  // # of locations in array

public:
  // constructor:
  vector()
    : A(new T[4]), Size(0), Capacity(4)
  { }

  // destructor:
  ~vector() 
  {
    delete[] A;
  }

  // size: # of elements in the vector
  size_t size()
  { 
    return this->Size; 
  }

  // [i]: return ith element, no error checking
  T& operator[](size_t index)
  {
    return this->A[index];
  }

  // push_back: insert e @ end of vector
  void push_back(T e)
  {
    //
    // assume there's space, store e @ end:
    //
    if(Size == Capacity) {
      T* temp = new T[Capacity * 2];
    }

    A[Size] = e;
    Size++;
    
  }

};
