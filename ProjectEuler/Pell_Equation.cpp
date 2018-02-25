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


// Minimal solution of x^2 - d*y^2 = +-1
pair<Lnum, Lnum> pell1_min (ull d, bool plus_one = true)
{
    pair<vector<ull>,int> pv = PQa(0,1,d);
    vector<ull> alphas = pv.fs; int l = pv.sc;
    
    int index;
    if (l & 1) {
        index = plus_one ? 2*l - 1 : l-1;
    }
    else {
        if (!plus_one) return make_pair(Lnum(-1), Lnum(-1));
        index = l-1;
    }
    Lnum bi, bim(1), gi(1), gim;
    int prel = (int)alphas.size() - l;
    Lnum alphai;
    for (int i=0; i<1+index; i++) {
        if (i < prel) alphai = Lnum(alphas[i]);
        else alphai = Lnum(alphas[prel+(i-prel)%l]);
        Lnum t = bim; bim = bi; bi = alphai*bi + t;
        t = gim; gim = gi; gi = alphai*gi + t;
    }
    
    return make_pair(gi,bi);
}

