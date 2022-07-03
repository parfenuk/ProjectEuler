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

bool next_combination (vsint &a, int n, int k)
{
    for (int i=k-1; i>=0; i--) {
        if (a[i] > n-k+i) continue;
        a[i]++;
        for (int j=i+1; j<k; j++) a[j] = a[j-1] + 1;
        return true;
    } return false;
}
// TODO: write function that counts i-th combination and reversly returns number of combination
// TODO: same for permutations
vvsint get_combinations (int n, int k) // numeration starts from 1
{
    vsint a;
    vvsint b;
    for (int i=1; i<=k; i++) a.push_back(i);
    b.push_back(a);
    while (next_combination(a,n,k)) b.push_back(a);
    return b;
}

// returns all partitions of n into sums: 1+1+1+1 = 2+1+1 = 2+2 = 3+1 = 4
vvsint sum_partitions (sint n)
{
    vvsint ret;
    
    vsint a;
    for (sint i=0; i<n; i++) a.push_back(1);
    
    ret.push_back(a);
    while (a[0] != n) {
        for (sint j=(sint)a.size()-2; j>=0; j--) {
            if (j && a[j] < a[j-1]) {
                a[j]++;
                int k = 0, p = (int)a.size();
                for (sint t=j+1; t<p; t++) k += a[t];
                for (sint t=j+1; t<p; t++) a.pop_back();
                for (sint t=0; t<k-1; t++) a.push_back(1);
                ret.push_back(a);
                break;
            }
            if (j == 0) {
                sint k = a[0];
                a.clear();
                a.push_back(k+1);
                for (sint t=0; t<n-a[j]; t++) a.push_back(1);
                ret.push_back(a);
            }
        }
    }
    
    return ret;
}

// size is k-th Bell number
// total number of elements is k*Bell[k]
// TODO: rewrite or write an additional function that doesn't keep all partitions in memory
vector<vvchar> fill_partitions (int k)
{
    const int MAXK = 10;
    vector<vvchar> partitions[MAXK+1];
    
    vchar s1(1,1);
    vvchar p1(1,s1);
    partitions[1].push_back(p1);
    
    for (int n=2; n<=MAXK; n++) {
        
        for (int i=0; i<(int)partitions[n-1].size(); i++) {
            
            vvchar p = partitions[n-1][i];
            p.push_back(vchar(1,n));
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

void fill_pythagorean_triples (ull n, vvpull &pits, bool primitive_only = true) // a^2 + b^2 = c^2, c <= n
{
    pits = vvpull(n+1);
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

// returns { n^0, n^1, ... , n^k } % mod
vull generate_powers (ull n, int k, int mod = 0)
{
    vull a(k+1,1);
    for (int i=1; i<=k; i++) {
        a[i] = a[i-1]*n;
        if (mod) a[i] %= mod;
    }
    return a;
}

vll generate_powers (ll n, int k, int mod = 0)
{
    vll a(k+1,1);
    for (int i=1; i<=k; i++) {
        a[i] = a[i-1]*n;
        if (mod) a[i] %= mod;
    }
    return a;
}

// returns { 1^k, 2^k, 3^k, ... , n^k } % mod
vull generate_fixed_powers (ull n, int k, int mod = 0)
{
    vull a(n);
    for (ull i=1; i<=n; i++) {
        a[i-1] = Algebra::powmod(i,k,mod);
    }
    return a;
}

vll generate_fixed_powers (ll n, int k, int mod = 0)
{
    vll a(n);
    for (ll i=1; i<=n; i++) {
        a[i-1] = Algebra::powmod(i,k,mod);
    }
    return a;
}

// returns { 0!, 1!, ... , k! } % mod
vull generate_factorials (int k, int mod = 0)
{
    vull a(k+1,1);
    for (int i=2; i<=k; i++) {
        a[i] = a[i-1]*i;
        if (mod) a[i] %= mod;
    }
    return a;
}

// returns { C(n,0), C(n,1), ... , C(n,n) }
vull generate_binomials (int n, int mod = 0)
{
    vull C(n+1);
    C[0] = 1;
    for (ull i=0; i<n; i++) {
        C[i+1] = C[i]*(n-i);
        if (mod) {
            C[i+1] %= mod;
            C[i+1] *= Algebra::inverse(i+1,mod);
            C[i+1] %= mod;
        }
        else C[i+1] /= (i+1);
    }
    return C;
}

// returns { { C(0,0), C(0,1), ... , C(0,k) }, { C(1,0), ... }, ... , { C(n,0), ... , C(n,k) } }
vvull get_binomials_matrix (int n, int k, int mod = 0)
{
    vvull C(n+1);
    for (int i=0; i<=n; i++) C[i] = vull(k+1);
    for (int i=0; i<=n; i++) C[i][0] = 1; // we assume C(0,0) = 0
    for (int i=1; i<=n; i++) for (int j=1; j<=i && j<=k; j++) {
        C[i][j] = C[i-1][j] + C[i-1][j-1];
        if (mod && C[i][j] >= mod) C[i][j] -= mod;
    }
    return C;
}

}
