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
    
    const ull N = powmod(10,14);
    const int Q = 1234567891;
    vector<pair<ll,pll>> A = get_division_blocks(N);
    
    ull n = N%Q;
    ans = n*(n+1)%Q*powmod(2,(N-2)%(Q-1),Q)%Q;
    ll i2 = inverse(2,Q);
    
    for (int i=0; i<(int)A.size(); i++) {
        ull a = A[i].fs, k1 = A[i].sc.fs%Q, k2 = A[i].sc.sc%Q;
        ull sum = (k1+k2)*(Q+k2-k1+1)%Q*i2%Q;
        sum = sum*powmod(2,(N-a)%(Q-1),Q)%Q;
        ans = (ans + Q - sum) % Q;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
