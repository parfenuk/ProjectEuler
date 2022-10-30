#include "Definitions.hpp"

#include "Algebra.hpp"
#include "Combinatorics.cpp"
#include "Complex.cpp"
// That's why we include .cpp here despite of having header. Ugly workaround
// https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
#include "Containers.cpp"
#include "Geometry.cpp"
#include "Geometry3D.cpp"
#include "Fractions.cpp"
#include "Lnum.hpp"
#include "Matrix.cpp"
#include "NumberUtils.hpp"
#include "Pell_Equation.cpp"
#include "StringUtils.cpp"
#include "Utils.cpp"

using namespace Algebra;
using namespace Containers;

const int P = 1000000007;
const int Q = 1000000009;
vector<pll> p2(1000), inv(1000);

void fill_algebra() {
    const int iP = (P+1)/2, iQ = (Q+1)/2;
    for (int i=1; i<1000; i++) {
        p2[i] = p2[i-1] + p2[i-1];
        if (p2[i].fs >= P) p2[i].fs -= P;
        if (p2[i].sc >= Q) p2[i].sc -= Q;
        inv[i].fs = inv[i-1].fs * iP % P;
        inv[i].sc = inv[i-1].sc * iQ % Q;
    }
}

struct Hash {
    pll A;
    Hash() { A = mp(0,0); }
    Hash(ll a) { A = mp(a,a); }
    Hash(pll a) { A = a; }
    Hash(ll a, ll b) { A = mp(a,b); }
    void remove_first (const bool &bit);
};

bool operator== (const Hash &a, const Hash &b) {
    return a.A == b.A;
}

Hash operator+ (const Hash &a, const Hash &b) {
    Hash c(a.A.fs+b.A.fs, a.A.sc+b.A.sc);
    if (c.A.fs >= P) c.A.fs -= P;
    if (c.A.sc >= Q) c.A.sc -= Q;
    return c;
}

Hash operator- (const Hash &a, const Hash &b) {
    Hash c(a.A.fs-b.A.fs, a.A.sc-b.A.sc);
    if (c.A.fs < 0) c.A.fs += P;
    if (c.A.sc < 0) c.A.sc += Q;
    return c;
}

Hash operator* (const Hash &a, const Hash &b) {
    return Hash(a.A.fs*b.A.fs % P, a.A.sc*b.A.sc % Q);
}

void Hash::remove_first (const bool &bit) {
    A.fs = (A.fs - bit)*inv[1].fs % P;
    A.sc = (A.sc - bit)*inv[1].sc % Q;
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    fill_algebra();
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
