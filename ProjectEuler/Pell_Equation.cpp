//
//  Pell_Equation.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 2/25/18.
//  Copyright © 2018 Miraslau Parafeniuk. All rights reserved.
//

#include <stdio.h>
#include "Lnum.cpp"

ull pell_integer_sqrt (ull n) // returns 0 if n != x*x
{
    if (n == 0) return 0;
    
    ull x = (ull)sqrt((dd)(n-0.001));
    while (x*x < n) x++;
    if (x*x == n) return x;
    return 0;
}

// a continued fraction expansion for (p0+sqrt(d))/q0
// correctness is confirmed for p0 = 0 and q0 = 1 only :)
pair<vector<ull>, int> PQa (ll p0, ll q0, ull d) // d > 0, q0 != 0, (p0*p0 - d) % q0 == 0, d != n^2
{
    dd sd = sqrt(d);
    ll ai = 1, aim = 0;
    ll bi = 0, bim = 1;
    ll gi = q0, gim = -p0;
    ll pi = p0, qi = q0;
    int i = -1, ir = -1000000000;
    vector<ull> alphas;
    
    ll pir = -1000000000, qir = -1000000000;
    while (true) {
        i++;
        dd xii = (pi + sd)/qi;
        dd xibari = (pi - sd)/qi;
        ll alphai = (ll)xii;
        
        ll t = aim; aim = ai; ai = alphai*ai + t;
        t = bim; bim = bi; bi = alphai*bi + t;
        t = gim; gim = gi; gi = alphai*gi + t;
        if (ir == -1000000000 && 1 < xii && -1 < xibari && xibari < 0) {
            ir = i; pir = pi; qir = qi;
        }
        if (ir != -1000000000 && ir != i && pi == pir && qi == qir) break;
        alphas.push_back(alphai);
        
        pi = alphai*qi - pi;
        qi = (d - pi*pi) / qi;
    }
    
    return make_pair(alphas, i-ir);
}

