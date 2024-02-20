//
//  Containers.hpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/4/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

#pragma once

// TODO: Global think: if eventually all methods are templated - maybe, we should make a class instead in order to avoid linker errors and make use of Solution 1 from the link
// TODO: vector comparison operators
namespace Containers
{

// VECTORS
template <class Trzx>
    void show (const vector<Trzx> &a, char separator = ' ', bool show_endl = true, bool show_size = false);
template <class Trzx>
    void show_pair (const vector<Trzx> &a, bool show_endl = true, bool show_size = false);
template <class Trzx>
    void show_matrix (const vector<vector<Trzx>> &a);
template <class Trzx>
    void show_tensor (const vector<vector<vector<Trzx>>> &a);
template <class Trzx>
    Trzx total_min (const vector<Trzx> &a);
template <class Trzx>
    Trzx total_max (const vector<Trzx> &a);
template <class Trzx>
    Trzx total_vector_sum (const vector<Trzx> &a);
template <class Trzx>
    Trzx total_vector_product (const vector<Trzx> &a);
template <class Trzx>
    int index_of_object (const vector<Trzx> &a, Trzx n);
template <class Trzx>
    bool contains (const vector<Trzx> &a, Trzx n);
template <class Trzx>
    bool is_disjoint (const vector<Trzx> &a, const vector<Trzx> &b);
template <class Trzx>
    vector<Trzx> subvector (const vector<Trzx> &a, int from, int to);
template <class Trzx>
    vector<Trzx> operator+ (vector<Trzx> a, const vector<Trzx> &b);
template <class Trzx>
    void append_to (vector<Trzx> &a, const vector<Trzx> &b);
template <class Trzx>
    void append_reversed (vector<Trzx> &a, const vector<Trzx> &b);
template <class Trzx>
    vector<Trzx> flatten (const vector<vector<Trzx>> &a);
template <class Trzx>
    int count_greater_than (const vector<Trzx> &a, Trzx value);
template <class Trzx>
    int count_less_than (const vector<Trzx> &a, Trzx value);

// PAIRS
template <class Trzx>
    vector<pair<Trzx,Trzx>> to_pairs (const vector<Trzx> &a);
template <class Trzx>
    pair<Trzx,Trzx> operator+ (const pair<Trzx,Trzx> &p, const pair<Trzx,Trzx> &q);
template <class Trzx>
    pair<Trzx,Trzx> operator- (const pair<Trzx,Trzx> &p, const pair<Trzx,Trzx> &q);
template <class Trzx>
    pair<Trzx,Trzx> operator* (const pair<Trzx,Trzx> &p, const pair<Trzx,Trzx> &q);
template <class Trzx>
    ostream& operator<< (ostream &os, const pair<Trzx,Trzx> &p);

// MAPS
template <class Tkey, class Tval>
    typename map<Tkey,Tval>::iterator max_less (map<Tkey,Tval> &M, Tkey k);
template <class Tkey, class Tval>
    typename map<Tkey,Tval>::iterator max_less_or_equal (map<Tkey,Tval> &M, Tkey k);
template <class Tkey, class Tval>
    typename map<Tkey,Tval>::iterator min_greater (map<Tkey,Tval> &M, Tkey k);
template <class Tkey, class Tval>
    typename map<Tkey,Tval>::iterator min_greater_or_equal (map<Tkey,Tval> &M, Tkey k);

// SETS
template <class Trzx>
    void show (const set<Trzx> &a, bool show_endl = true, bool show_size = false);
template <class Trzx>
    typename set<Trzx>::iterator max_less (set<Trzx> &S, Trzx k);
template <class Trzx>
    typename set<Trzx>::iterator max_less_or_equal (set<Trzx> &S, Trzx k);
template <class Trzx>
    typename set<Trzx>::iterator min_greater (set<Trzx> &S, Trzx k);
template <class Trzx>
    typename set<Trzx>::iterator min_greater_or_equal (set<Trzx> &S, Trzx k);

    bool isPalindrom (const vint &a);
}
