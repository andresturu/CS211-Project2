/*main.cpp*/

#include <iostream>
#include <string>

//#include <map>
#include "map.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

int main()
{
  map<string, int> animals;

  cout << endl;
  cout << "** Starting" << endl;
  cout << endl;

  // insert some animals to get started:
  animals["elephant"] = 99;
  animals["cat"] = 3;
  animals["owl"] = 1;
  animals["dog"] = 2;
  animals["bear"] = 1;
  animals["moth"] = 9;
  animals["pig"] = 4;
  animals["ferret"] = 6;
  animals["zebra"] = 99;
  
  // make sure we don't insert duplicates, but update instead:
  animals["elephant"] = 1;
  animals["zebra"] = 1;

  // how many do we have?
  cout << "# of animals: "<< animals.size() << endl;
  cout << endl;

  // print contents of the tree:
  animals.print();

  //
  // done:
  //
  cout << endl;
  cout << "** Done **" << endl;
  cout << endl;

  return 0;
}
