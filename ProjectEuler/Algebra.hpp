//
//  Algebra.hpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/4/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

#pragma once

#include <math.h>
#include <set>
#include <map>
#include <vector>
#include "Globals.cpp"
#include "NumberUtils.hpp"

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;

using namespace std;

#define pll pair<ll,ll>
#define pull pair<ull,ull>
#define mp make_pair
#define fs first
#define sc second

namespace Algebra
{
    extern vector<bool> isPrime;
    extern vector<int> primes; // this is a special prime number, which is always here
    extern vector<int> primePi;
    extern vector<int> eulerPhi;
    extern map<ll,ll> eulerCache;
    extern vector<ll> eulerPhiSum; // prefill small values for function

    ull GCD (ull a, ull b);
    ull LCM (ull a, ull b);
    pll Extended_Euclid (ll a, ll b);
    vector<ull> Divisors (ull n);
    vector<ull> Divisors_square (ull n);
    ull Divisors_sum (ull n);
    ull productmod (ull a, ull n, ull mod);
    ll powmod (ll a, ll k, ll mod = 0);
    ll inverse (ll a, ll mod, ll p = 0);
    ull modular_sqrt (ull a, ull p);
    void Eratosthenes_sieve (int n, bool fill_primes = false, bool fill_pi = false);
    bool primeQ (ull n);
    bool isProbablePrime (ull n);
    ull least_prime_divisor (ull n);
    vector<pull> factorize (ull n);
    ll primitiveRoot (ll p);
    ull rad (ull n);
    ull Divisors_count (ull n); // TODO: can we replace upper? =)
    ull square_representations_count (ull n);
    ull Chinese_theorem (vector<ll> divs, vector<ll> rests);
    int MoebiusMu (ull n);
    vector<char> MoebuisMuSieve (ull n);
    ull EulerPhi (ull n);
    void EulerPhiSieve (ull n);
    ll EulerPhiSum (ll n, int Q);
    ull coprime_count_in_range (ull N, ull from, ull to);
    ull power_fact (ull n, ull p);
    ull count_divisible_by (ull n, ull lb, ull ub, ull mod = 0);
    ull sum_divisible_by (ull n, ull lb, ull ub);
    char Gauss (vector<vector<dd>> a, vector<dd> &ans);
}
