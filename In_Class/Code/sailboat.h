/*sailboat.h*/

#pragma once

#include <string>

using namespace std;

class Sailboat
{
private:
  string  Name;
  string  Brand;
  double  Length;

public:
  // constructor:
  Sailboat(string name, string brand, double length);

  // member function to compute max speed of boat:
  double maxspeed() const;

  // getters:
  string getName() const;
  string getBrand() const;
  double getLength() const;
};
