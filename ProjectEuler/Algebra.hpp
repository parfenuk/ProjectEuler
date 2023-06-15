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
    extern vbool isPrime;
    extern vint primes;
    extern vint primePi;
    extern vint eulerPhi;
    extern mll eulerCache;
    extern vll eulerPhiSum;

    ull GCD (ull a, ull b);
    ull LCM (ull a, ull b);
    ll signedGCD (ll a, ll b);
    void precalc_GCD (ull N);
    vpull factorized_GCD (const vpull &F, const vpull &G);
    vpull factorized_GCD (const vpull &F, ull n);
    pll Extended_Euclid (ll a, ll b);
    vull Divisors (ull n, bool sorted = true);
    vull Divisors (const vpull &F, bool sorted = true);
    vull Divisors_product (const vull &a, bool sorted = true);
    vull Divisors_square (ull n, bool sorted = true);
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
    vpull factorize (ull n);
    vpull merged_factorize (const vpull &F, const vpull &G);
    ll primitiveRoot (ll p);
    ull rad (ull n);
    ull Chinese_theorem (vll divs, vll rests);
    char MoebiusMu (ull n);
    vchar MoebuisMuSieve (ull n);
    ull EulerPhi (ull n);
    void EulerPhiSieve (ull n);
    ll EulerPhiSum (ll n, int Q);
    ull square_representations_count (ull n);
    ull coprime_count_in_range (ull N, ull from, ull to);
    int max_power (ull n, ull p);
    ull power_fact (ull n, ull p);
    ull count_divisible_by (ull n, ull lb, ull ub, ull mod = 0);
    ull sum_divisible_by (ull n, ull lb, ull ub);
    char Gauss (vvdd a, vdd &ans);
}
