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
    
    const int N = 10000000;
    
    Eratosthenes_sieve(N);
    // works much faster with 'Eratosthenes_sieve' call before
    vector<char> mu = MoebuisMuSieve(N);
    
    for (ll r=1; r*r<=N; r++) {
        for (ll s = NumberUtils::lower_integer_sqrt(3*r*r)+1; s*s < 4*r*r; s++) {
            ll x = 4*r*r - s*s;
            ll z = s*s - 3*r*r;
            ll t1 = r*(2*r+s), t2 = r*(2*r-s);
            for (ll d=1;;d++) {
                if (mu[d] == 0) continue;
                if (t2 > z && (x+t2)*d > N) break;
                if (t2 <= z && (x+t1)*d > N) break;
                if ((x+t1)*d <= N) ans += (x+t1)*d;
                if (t2 > z) ans += (x+t2)*d;
            }
        }
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
