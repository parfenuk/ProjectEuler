//
//  Algebra.hpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/4/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

#pragma once

#include "Definitions.hpp"
#include "NumberUtils.hpp"

namespace Algebra
{
    extern vector<bool> isPrime;
    extern vector<int> primes;
    extern vector<int> primePi;
    extern vector<int> eulerPhi;
    extern map<ll,ll> eulerCache;
    extern vector<ll> eulerPhiSum;

    ull GCD (ull a, ull b);
    ull LCM (ull a, ull b);
    ll signedGCD (ll a, ll b);
    pll Extended_Euclid (ll a, ll b);
    vector<ull> Divisors (ull n);
    vector<ull> Divisors_square (ull n);
    ull Divisors_sum (ull n);
    ull Divisors_count (ull n);
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
    ull Chinese_theorem (vector<ll> divs, vector<ll> rests);
    int MoebiusMu (ull n);
    vector<char> MoebuisMuSieve (ull n);
    ull EulerPhi (ull n);
    void EulerPhiSieve (ull n);
    ll EulerPhiSum (ll n, int Q);
    ull square_representations_count (ull n);
    ull coprime_count_in_range (ull N, ull from, ull to);
    ull power_fact (ull n, ull p);
    ull count_divisible_by (ull n, ull lb, ull ub, ull mod = 0);
    ull sum_divisible_by (ull n, ull lb, ull ub);
    char Gauss (vector<vector<dd>> a, vector<dd> &ans);
}
