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

vector<pair<Lnum,Lnum>> pell_fundamentals (ull d, ull n)
{
    pair<Lnum,Lnum> tu = pell1_min(d);
    vector<pair<Lnum,Lnum>> funds;
    Lnum D(d), N(n);
    
    for (Lnum y=Lnum(); y*y*2*D <= N*(tu.fs-one); y=y+one) {
        
        Lnum x = lnum_sqrt(y*y*D + N);
        if (x == O) continue;
        funds.push_back(make_pair(x,y));
        if (divmod(x*x + D*y*y, N).sc != O || divmod(x*y*2, N).sc != O) funds.push_back(make_pair(x*Lnum(-1),y));
    }
    return funds;
}

vector<pair<Lnum,Lnum>> pell_bf (ull d, ull n, Lnum max_x)
{
    vector<pair<Lnum,Lnum>> funds = pell_fundamentals(d,n);
    set<pair<Lnum,Lnum>> sols;
    
    //cout << "FUNDS:\n";
    for (int i=0; i<(int)funds.size(); i++) {
        Lnum x = funds[i].fs, y = funds[i].sc;
        //x.show(); cout << " "; y.show(); cout << endl;
        if (x.isNegative()) x.change_sign();
        if (y.isNegative()) y.change_sign();
        sols.insert(make_pair(x,y));
    }
    
    pair<Lnum, Lnum> min_sol = pell1_min(d);
    //cout << "SOLUTIONS:\n";
    if (min_sol.fs != Lnum(-1)) {
        Lnum t = min_sol.fs, u = min_sol.sc;
        Lnum x = t, y = u;
        while (true) {
            Lnum T = x, U = y;
            //T.show(); cout << " "; U.show(); cout << endl;
            bool added = false;
            for (int i=0; i<(int)funds.size(); i++) {
                Lnum r = funds[i].fs, s = funds[i].sc;
                Lnum X = r*T + s*U*Lnum(d);
                Lnum Y = r*U + s*T;
                if (lnum_abs(X) <= max_x) {
                    sols.insert(make_pair(lnum_abs(X), lnum_abs(Y)));
                    added = true;
                }
            }
            if (!added) break;
            
            Lnum tmp = x; x = t*x + u*y*Lnum(d); y = t*y + tmp*u;
        }
    }
    
    vector<pair<Lnum,Lnum>> v;
    for (set<pair<Lnum,Lnum>>::iterator it = sols.begin(); it != sols.end(); it++) {
        pair<Lnum,Lnum> p = *it;
        v.push_back(p);
    }
    
    return v;
}

// a*x^2 - b*y^2 == c
vector<pair<Lnum,Lnum>> quad_s (ull a, ull b, ull c, Lnum max_x)
{
    Lnum A(a);
    vector<pair<Lnum,Lnum>> res, v = pell_bf(a*b, a*c, max_x*A);
//    cout << "PELL_BF:\n";
//    for (int i=0; i<(int)v.size(); i++) {
//        v[i].fs.show(); cout << " "; v[i].sc.show(); cout << endl;
//    }
    
    for (int i=0; i<(int)v.size(); i++) {
        pair<Lnum,Lnum> xrem = divmod(v[i].fs,A);
        if (xrem.sc == O) res.push_back(make_pair(xrem.fs,v[i].sc));
    }
    return res;
}

