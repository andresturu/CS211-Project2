#include "group_activity1_header.h"

void Set::add(int e) {
    if (find (S.begin(), S.end(), e) != S.end())
        return;
}

void Set::Union(const Set& S2) {
    for (int e : S2.S) {
        this -> add(e);
    }
}