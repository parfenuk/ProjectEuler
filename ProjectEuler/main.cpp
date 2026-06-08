#include "Definitions.hpp"

#include "Algebra.hpp"
#include "Combinatorics.cpp"
#include "Complex.cpp"
// That's why we include .cpp here despite of having header. Ugly workaround
// https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
#include "Containers.cpp"
#include "Geometry.cpp"
#include "Geometry3D.cpp"
#include "FFT.cpp"
#include "Fractions.cpp"
#include "Lnum.hpp"
#include "Matrix.cpp"
#include "NumberUtils.hpp"
#include "Pell_Equation.cpp"
#include "StringUtils.cpp"
#include "Utils.cpp"

using namespace Algebra;
using namespace Containers;

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
    
    const ll Q = 1000000007;
    const ll N = powmod(7,13);
    EulerPhiSieve(7000000,Q);
    
    vector<pair<ll,pll>> A = get_division_blocks(N);
    vector<pair<ll,pll>> B(A.size()); // <value, <count % Q, count % (Q-1)>> // last needed for correct power
    for (int i=0; i<(int)A.size(); i++) {
        ll v = A[i].fs % Q;
        ll c = EulerPhiSum(A[i].sc.sc,Q) - EulerPhiSum(A[i].sc.fs-1,Q);
        c = (c+Q)*4 % Q;
        B[i].fs = v;
        B[i].sc.fs = c;
    }
    
    for (int i=1; i<=7000000; i++) {
        eulerPhiSum[i] = eulerPhiSum[i-1] + eulerPhi[i];
        if (eulerPhiSum[i] >= Q-1) eulerPhiSum[i] -= Q-1;
    }
    eulerCache.clear();
    for (int i=0; i<(int)A.size(); i++) {
        ll c = EulerPhiSum(A[i].sc.sc,Q-1) - EulerPhiSum(A[i].sc.fs-1,Q-1);
        c = (c+Q-1)*4 % (Q-1);
        B[i].sc.sc = c;
    }
    
    ll t = 1;
    for (int i=0; i<(int)B.size(); i++) {
        t = t*powmod(B[i].fs+1,B[i].sc.sc,Q) % Q;
    }
    ll n = N%Q;
    ll b = n*(n+1)%Q*4%Q * t%Q;
    for (int i=0; i<(int)B.size(); i++) {
        b -= B[i].fs*B[i].fs%Q * B[i].sc.fs%Q;
        if (b < 0) b += Q;
    }
    ans = (t*t + Q - b - 1) % Q;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
