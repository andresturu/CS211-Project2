/*sailboat.cpp*/

#include <iostream>
#include <string>
#include <cmath>

#include "sailboat.h"

using namespace std;

//
// constructor to initialize object:
//
Sailboat::Sailboat(string name, string brand, double length)
  : Name(name), Brand(brand), Length(length)
{ }

//
// member function to compute max speed of a sailboat:
//
double Sailboat::maxspeed() const
{
  return 1.3 * sqrt(this->Length);
}

//
// getters:
//
string Sailboat::getName() const { return this->Name; }
string Sailboat::getBrand() const { return this->Brand; }
double Sailboat::getLength() const { return this->Length; }