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

ull a=0,b=0,c=0,d=0;

bool isStealthy (ull n)
{
    vector<ull> D = Divisors(n);
    vector<pair<ull,pull>> v;
    for (int i=0; i<(int)D.size(); i++) {
        if (D[i] > n/D[i]) break;
        v.push_back(mp(D[i]+n/D[i],mp(D[i],n/D[i])));
    }
    sort(v.begin(),v.end());
    for (int i=1; i<(int)v.size(); i++) {
        if (v[i].fs == v[i-1].fs+1) {
            a = v[i-1].sc.fs;
            b = v[i-1].sc.sc;
            c = v[i].sc.fs;
            d = v[i].sc.sc;
            return true;
        }
    }
    return false;
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
    
    const ull N = 100000000000000, A = 10000000;
    vull S;
    
    for (ull d1=1; d1*d1<=A; d1++) {
        // k^2 + k*(b-a+1) - a == 0
        for (ull a = d1*d1; a<=A; a+=d1) {
            ll d2 = a/d1;
            ll k1 = d1, k2 = -d2;
            ll b = a-1-k1-k2;
            if (k1 < a && b >= a && a*b <= N) S.push_back(a*b);
            k1 = d2; k2 = -d1;
            b = a-1-k1-k2;
            if (k1 < a && b >= a && a*b <= N) S.push_back(a*b);
        }
    }
    sort(S.begin(),S.end());
    S.erase(unique(S.begin(),S.end()),S.end());
    
    ans = (ull)S.size();
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
// set - 370s, 3.5 GB
// vector - 265s, 591 MB
// divisors optimization - 200s
// divisors inversion - 12.77s (!!!)
