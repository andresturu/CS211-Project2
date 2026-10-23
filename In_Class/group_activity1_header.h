#pragma once

#include <vector>
using namespace std;

struct Set {
    vector<int> S;
    
    void add(int e);

    void Union(const Set& S2);
};

