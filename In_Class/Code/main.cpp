#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

#include "sailboat.h"

using namespace std;


/** 
 * @brief returns the given string in lowercase
 *
 * @param s string
 * @return equivalent string in lowercase
 */
string toLower(string s) {

  for (char& c : s) {
    c = tolower(c);
  }

  return s;
}


/** 
 * @brief outputs each boat in the vector, one per line
 *
 * @param boats vector of sailboat objects
 * @return void
 */
void output_boats(const vector<Sailboat>& boats)
{
  cout << endl;
  
  // output all boats: name (brand): length, maxspeed
  for (const Sailboat& s : boats) {
    
    cout << s.getName() << " ("
         << s.getBrand() << "): "
         << s.getLength() << ", maxspeed="
         << s.maxspeed() << " kts" << endl;
  }

  cout << endl;
  return;
}

/** 
 * @brief searches for first boat with matching name
 *
 * @param boats vector of sailboat objects
 * @param name sailboat you are searching for
 * @return index of first boat that matches, -1 if none
 */
int search(const vector<Sailboat>& boats, string name)
{
  for (size_t i = 0; i < boats.size(); i++) {
    if (toLower(boats[i].getName()) == toLower(name)) {
      return i;
    }

    // keep searching
  }

  // if we get here, not found
  return -1;
}

bool was_opened(vector<Sailboat>& the_vector, string this_file) {
  ifstream file;

  file.open(this_file);
  if (file.fail()) { 
    return false;
  }

  while (true) {
    string name, brand;
    double len;
        
    file >> name;
    file >> brand;
    file >> len;
        
    if (file.fail())
      break;
      
    Sailboat s(name, brand, len);

    the_vector.push_back(s);
  }

  return true;
}

int main()
{
  vector<Sailboat>  boats;

  string the_file;
  cin >> the_file;
  bool state = was_opened(boats, the_file);
  
  if(state) {
    //
    // let's output all the boats:
    //
    output_boats(boats);
    
    //
    // let user search for boats to output:
    //
    string name;
    cout << "Enter a boat name (# to exit)> ";
    cin  >> name;
    
    while (name != "#") {

      int index = search(boats, name);

      if (index < 0) {
        cout << "not found" << endl;
      }
      else {
        // output boat's max speed:
        cout << boats[index].getName() 
            << ": maxspeed: "
            << boats[index].maxspeed() 
            << endl;
      }
      
      cout << endl;
      cout << "Enter a boat name (# to exit)> ";
      cin  >> name;
    }//while
  } else {
    cout << "input file not found" << endl;  
  }
  
  //
  // done:
  //
  return 0;
}
