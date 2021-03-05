//
//  Combinatorics.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/3/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

// ********** VARIOUS COMBINATIONS AND GENERATIONS **********

namespace Combinatorics
{
// TODO: write 2-dimensional binomials generator
ull Binomial (ull n, ull k, int p = 0) // C(n,k) mod p. p must be prime and greater than n and k.
{
    if (k > n) return 0;
    
    if (p == 0) {
        ull s = 1;
        for (ull i=0; i<k; i++) {
            s *= (n-i);
            s /= (i+1);
        }
        return s;
    }
    
    ull s = 1;
    for (ull i=0; i<k; i++) {
        s *= (n-i); s %= p;
        s *= Algebra::inverse(i+1,p); s %= p;
    }
    return s;
}

bool next_combination (vector<sint> &a, int n, int k)
{
    for (int i=k-1; i>=0; i++) {
        if (a[i] > n-k+i) continue;
        a[i]++;
        for (int j=i+1; j<k; j++) a[j] = a[j-1] + 1;
        return true;
    } return false;
}
// TODO: write function that counts i-th combination and reversly return number of combination
// TODO: same for permutations
vector<vector<sint>> get_combinations (int n, int k) // numeration starts from 1
{
    vector<sint> a;
    vector<vector<sint>> b;
    for (int i=1; i<=k; i++) a.push_back(i);
    b.push_back(a);
    while (next_combination(a,n,k)) b.push_back(a);
    return b;
}

vector<vector<int>> sum_partitions (int n)
{
    vector<vector<int>> ret;
    
    vector<int> a;
    for (int i=0; i<n; i++) a.push_back(1);
    
    ret.push_back(a);
    while (a[0] != n) {
        for (int j=(int)a.size()-2; j>=0; j--) {
            if (j && a[j] < a[j-1]) {
                a[j]++;
                int k = 0, p = (int)a.size();
                for (int t=j+1; t<p; t++) k += a[t];
                for (int t=j+1; t<p; t++) a.pop_back();
                for (int t=0; t<k-1; t++) a.push_back(1);
                ret.push_back(a);
                break;
            }
            if (j == 0) {
                int k = a[0];
                a.clear();
                a.push_back(k+1);
                for (int t=0; t<n-a[j]; t++) a.push_back(1);
                ret.push_back(a);
            }
        }
    }
    
    return ret;
}

// size is k-th Bell number
// total number of elements is k*Bell[k]
// TODO: rewrite or write an additional function that doesn't keep all partitions in memory
vector<vector<vector<char>>> fill_partitions (int k)
{
    const int MAXK = 10;
    vector<vector<vector<char>>> partitions[MAXK+1];
    
    vector<char> s1(1,1);
    vector<vector<char>> p1(1,s1);
    partitions[1].push_back(p1);
    
    for (int n=2; n<=MAXK; n++) {
        
        for (int i=0; i<(int)partitions[n-1].size(); i++) {
            
            vector<vector<char>> p = partitions[n-1][i];
            p.push_back(vector<char>(1,n));
            partitions[n].push_back(p);
            p.pop_back();
            
            for (int j=0; j<(int)p.size(); j++) {
                
                p[j].push_back(n);
                partitions[n].push_back(p);
                p[j].pop_back();
            }
        }
    }
    
    return partitions[k];
}

void fill_pythagorean_triples (ull n, vector<vector<pull>> &pits, bool primitive_only = true) // a^2 + b^2 = c^2, c <= n
{
    pits = vector<vector<pull>>(n+1);
    for (ull p=2; p*p<=n; p++) for (ull q=1+p%2; q<p; q+=2) {
        
        if (Algebra::GCD(p,q) != 1) continue;
        
        ull a = p*p - q*q;
        ull b = 2*p*q;
        ull c = p*p + q*q;
        if (c > n) break;
        
        for (ull k=1;;k++) {
        
            ull A = a*k, B = b*k, C = c*k;
            if (C <= n) pits[C].push_back(make_pair(A,B));
            else break;
            
            if (primitive_only) break;
        }
    }
}

// returns { n^0, n^1, ... , n^(k-1) } % mod
vector<ull> get_powers (ull n, int k, int mod = 0)
{
    vector<ull> a(k,1);
    for (int i=1; i<k; i++) {
        a[i] = a[i-1]*n;
        if (mod) a[i] %= mod;
    }
    return a;
}

// returns { 0!, 1!, ... , (k-1)! } % mod
vector<ull> get_factorials (int k, int mod = 0)
{
    vector<ull> a(k,1);
    for (int i=2; i<k; i++) {
        a[i] = a[i-1]*i;
        if (mod) a[i] %= mod;
    }
    return a;
}

}
