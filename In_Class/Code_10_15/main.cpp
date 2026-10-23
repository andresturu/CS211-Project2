#include <iostream>
#include <vector>
//#include "vector.h"

using namespace std;

void print(std::vector<int>& V)
{
  for (size_t i=0; i<V.size(); i++)
    cout << V[i] << endl; 
}

int main()
{
  std::vector<int>  V;
    
  V.push_back(3);
  V.push_back(9);
  V.push_back(-2);
  V.push_back(11);
    
  print(V);

  return 0;
}
