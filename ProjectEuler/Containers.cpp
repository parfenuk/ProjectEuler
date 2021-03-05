//
//  Containers.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/3/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

// ********** BASIC STL CONTAINERS AND SEARCH FUNCTIONALITY **********

#include "Containers.hpp"

namespace Containers
{

template <class Trzx>
void show (const vector<Trzx> &a, bool show_endl/*=true*/, bool show_size/*=false*/)
{
    if (show_size) cout << a.size() << endl;
    for (int i=0; i<(int)a.size(); i++) cout << a[i] << " ";
    if (show_endl) cout << endl;
}

template <class Trp>
void show_pair (const vector<Trp> &a, bool show_endl/*=true*/, bool show_size/*=false*/)
{
    if (show_size) cout << a.size() << endl;
    for (int i=0; i<(int)a.size(); i++) cout << "(" << a[i].fs << " " << a[i].sc << ") ";
    if (show_endl) cout << endl;
}

template <class Trzx>
void show_matrix (const vector<vector<Trzx>> &a)
{
    for (int i=0; i<(int)a.size(); i++) {
        for (int j=0; j<(int)a[i].size(); j++) cout << a[i][j] << " ";
        cout << endl;
    }
}

template <class Trzx>
void show_tensor (const vector<vector<vector<Trzx>>> &a)
{
    for (int i=0; i<(int)a.size(); i++) {
        for (int j=0; j<(int)a[i].size(); j++) {
            for (int k=0; k<(int)a[i][j].size(); k++) cout << a[i][j][k] << " ";
            cout << endl;
        }
        cout << "------------------------------\n";
    }
}

template <class Trzx>
Trzx total_vector_sum (const vector<Trzx> &a)
{
    Trzx s = 0;
    for (int i=0; i<(int)a.size(); i++) s += a[i];
    return s;
}

template <class Trzx>
int index_of_object (const vector<Trzx> &a, Trzx n)
{
    for (int i=0; i<(int)a.size(); i++) if (a[i] == n) return i;
    return -1;
}

template <class Trzx>
bool contains (const vector<Trzx> &a, Trzx n)
{
    return index_of_object(a,n) != -1;
}

template <class Trzx>
bool is_disjoint (const vector<Trzx> &a, const vector<Trzx> &b)
{
    for (int i=0; i<(int)a.size(); i++) {
        if (contains(b,a[i])) return false;
    }
    return true;
}

template <class Trzx>
vector<Trzx> subvector (const vector<Trzx> &a, int from, int to)
{
    vector<Trzx> b;
    for (int i=max(0,from); i<=min((int)a.size()-1,to); i++) b.push_back(a[i]);
    return b;
}

template <class Trzx>
vector<Trzx> operator+ (vector<Trzx> a, const vector<Trzx> &b)
{
    for (int i=0; i<(int)b.size(); i++) a.push_back(b[i]);
    return a;
}

template <class Trzx>
void append_to (vector<Trzx> &a, const vector<Trzx> &b)
{
    for (int i=0; i<(int)b.size(); i++) a.push_back(b[i]);
}

template <class Trzx>
vector<Trzx> flatten (const vector<vector<Trzx>> &a)
{
    vector<Trzx> b;
    for (int i=0; i<(int)a.size(); i++) for (int j=0; j<(int)a[i].size(); j++) b.push_back(a[i][j]);
    return b;
}

// a - sorted, returns count of elements in vector that are greater than given value
template <class Trzx>
int count_greater_than (const vector<Trzx> &a, Trzx value)
{
    if (a[0] > value) return (int)a.size();
    if (a.back() <= value) return 0;

    int lb = 0, ub = (int)a.size()-1;
    while (lb < ub) {
        int M = (lb+ub)/2;
        if (a[M] <= value) lb = M+1;
        else ub = M;
    }

    return (int)a.size() - lb;
}

// a - sorted, returns count of elements in vector that are less than given value
template <class Trzx>
int count_less_than (const vector<Trzx> &a, Trzx value)
{
    if (a[0] >= value) return 0;
    if (a.back() < value) return (int)a.size();

    int lb = 0, ub = (int)a.size()-1;
    while (lb < ub) {
        int M = (lb+ub+1)/2;
        if (a[M] >= value) ub = M-1;
        else lb = M;
    }

    return lb+1;
}

bool isPalindrom (const vector<int> &a)
{
    int n = (int)a.size();
    for (int i=0; i<n-1-i; i++) if (a[i] != a[n-1-i]) return false;
    return true;
}

}
