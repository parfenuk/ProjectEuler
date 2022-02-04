//
//  NumberUtils.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/3/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

// ********** NUMERIC FUNCTIONS **********

#include "NumberUtils.hpp"

namespace NumberUtils
{

dd decimal_part (dd a)
{
    return a - (int)(a+0.00000001);
}

ull integer_sqrt (ull n) // returns 0 if n != x*x
{
    if (n == 0) return 0;
    
    ull x = (ull)sqrt((dd)(n-0.001));
    while (x*x < n) x++;
    if (x*x == n) return x;
    return 0;
}

ull lower_integer_sqrt (ull n) // returns greatest x such that x*x <= n
{
    ull x = (ull)sqrt((dd)(n+0.001));
    while (x*x > n) x--;
    return x;
}

ull upper_integer_sqrt (ull n) // returns greatest x such that x*x >= n
{
    if (n == 0) return 0;
    
    ull x = (ull)sqrt((dd)(n-0.001));
    while (x*x < n) x++;
    return x;
}

// n = 153 -> (1,5,3)
vint digits (ull n, int base/*=10*/, int min_size/*=1*/)
{
    vint a;
    while (n) {
        a.push_back(n%base);
        n /= base;
    }
    while (a.size() < min_size) a.push_back(0);
    reverse(a.begin(), a.end());
    
    return a;
}

// (4,7,2) -> 472
ull from_digits (const vint &a, int base)
{
    ull s = 0;
    for (int i=0; i<(int)a.size(); i++) {
        s *= base;
        s += a[i];
    }
    return s;
}

bool is_palindromic_number (ull n)
{
    return Containers::isPalindrom(digits(n)); // TODO: think of how to apply template here...
}

dd clock_angle (dd h, dd m, dd s/*=0*/) // h - 0..11, m = 0..59
{
    dd am = 6*m + 0.1*s, ah = 30*h + m/2 + s/120;
    dd ang = fabs(am-ah);
    if (ang > 180) ang = 360 - ang;
    return ang;
}

bool same_digits (ull n, ull m)
{
    vint a = digits(n), b = digits(m);
    
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    if (a.size() != b.size()) return false;
    
    for (int i=0; i<(int)a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// returns 11...1 k times % mod
ull ones_mod (ull k, ull mod)
{
    ull b = 0, n = 1, a = 1;
    while (k) {
        if (k%2 == 0) {
            k /= 2;
            a = a*Algebra::powmod(10,n,mod) + a;
            n *= 2;
            a %= mod;
        }
        else {
            k--;
            b = b*Algebra::powmod(10,n,mod) + a;
            b %= mod;
        }
    }
    return b;
}

ll code_from_v (const vint &v, const vint &matches) // returns number in dynamic base
{
    ll ret = v[0];
    
    for (int i=1; i<(int)v.size(); i++) {
        ret *= (matches[i]+1);
        ret += v[i];
    }
    
    return ret;
}

vint v_from_code (ll n, const vint &matches)
{
    vint v;
    
    for (int i=(int)matches.size()-1; i>=0; i--) {
        v.push_back(n % (matches[i]+1));
        n /= matches[i] + 1;
    }
    
    reverse(v.begin(), v.end());
    return v;
}

}
