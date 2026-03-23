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
    
    const int Q = 1000000007;
    
    const ull N = powmod(10,16);
    const ull K = 100000000;
    
    vull F = Combinatorics::generate_factorials(K,Q);
    vull inv = generate_inverses(K,Q);
    vull FI(K+1,1); for (int i=2; i<=K; i++) FI[i] = FI[i-1]*inv[i] % Q;

    ull t = powmod(2,N,Q);
    ull t2 = inverse(powmod(4,N/K,Q), Q);
    
    for (int i=0; i<=K/2; i++) {
        ull s = F[K]*FI[i] % Q * FI[i] % Q * FI[K-2*i] % Q;
        s = s*t % Q;
        t = t*t2 % Q;
        ans += s;
        if (ans >= Q) ans -= Q;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
