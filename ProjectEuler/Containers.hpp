//
//  Containers.hpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/4/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

#pragma once

// TODO: Global think: if eventually all methods are templated - maybe, we should make a class instead in order to avoid linker errors and make use of Solution 1 from the link
namespace Containers
{
template <class Trzx>
    void show (const vector<Trzx> &a, bool show_endl = true, bool show_size = false);
template <class Trp>
    void show_pair (const vector<Trp> &a, bool show_endl = true, bool show_size = false);
template <class Trzx>
    void show_matrix (const vector<vector<Trzx>> &a);
template <class Trzx>
    void show_tensor (const vector<vector<vector<Trzx>>> &a);
template <class Trzx>
    Trzx total_vector_sum (const vector<Trzx> &a);
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
    vector<Trzx> flatten (const vector<vector<Trzx>> &a);
template <class Trzx>
    int count_greater_than (const vector<Trzx> &a, Trzx value);
template <class Trzx>
    int count_less_than (const vector<Trzx> &a, Trzx value);

    bool isPalindrom (const vint &a);
}
