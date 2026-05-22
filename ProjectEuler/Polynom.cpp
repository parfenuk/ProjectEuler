//
//  Polynom.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 22.05.26.
//  Copyright © 2026 Miraslau Parafeniuk. All rights reserved.
//

#include <stdio.h>

namespace Polynom
{

struct Polynom
{
    int Q;
    vll A;
    
    Polynom(int q) { Q = q; }
    Polynom(const vll &a, int q) { A = a; Q = q; }
    
    ll operator[] (int k) { return A[k]; }
    
    ll value_at(ll x) {
        ll val = 0, s = 1;
        for (int i=0; i<(int)A.size(); i++) {
            val = (val + s*A[i]) % Q;
            s = s*x % Q;
        }
        return val;
    }
    
    void show() {
        for (int i=0; i<(int)A.size(); i++) {
            cout << A[i];
            if (i) cout << "*x";
            if (i > 1) cout << "^" << i;
            if (i+1 != (int)A.size()) cout << " + ";
        } cout << endl;
    }
    
    // TODO: add, subtract, multiply, fft_multiply, remainder
};

Polynom interpolating_polynom (const vpll &B, int Q) // B: [<x,value>]
{
    Polynom P(Q);
    
    vvll G;
    for (int i=0; i<(int)B.size(); i++) {
        vll g(B.size()+1);
        g[0] = 1;
        for (int j=1; j<B.size(); j++) {
            g[j] = g[j-1]*B[i].fs % Q;
        }
        g.back() = B[i].sc;
        G.push_back(g);
    }
    
    Algebra::Gauss(G,P.A,Q);
    
    return P;
}

ll sum_values_to (Polynom &P, const ll n) // P(1) + P(2) + ... + P(n)
{
    int Q = P.Q;
    int N = (int)P.A.size();
    vll B = Combinatorics::generate_Bernoulli_numbers(N,Q);
    vull F = Combinatorics::generate_factorials(N,false,Q);
    vull FI = Combinatorics::generate_factorials(N,true,Q);
    
    ll res = P[0]*n % Q; // for degree 0
    for (int k=1; k<N; k++) { // degerees
        ll sum = 0;
        for (int s=0; s<=k; s++) {
            ll tmp = F[k+1]*FI[s] % Q * FI[k+1-s] % Q;
            tmp = tmp*B[s] % Q;
            tmp = tmp*Algebra::powmod((n+1)%Q,k+1-s,Q) % Q;
            sum += tmp;
            if (sum >= Q) sum -= Q;
        }
        sum = sum*Algebra::inverse(k+1,Q) % Q;
        sum = sum*P[k] % Q;
        res += sum;
        if (res >= Q) res -= Q;
    }
    
    return res;
}

}
