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

const ll N = 1000000000000;
const ll B = 1000000;
vvint pdivisors(B+1);
vchar mu;

vll squarefree_divisors (ll b)
{
    vll D;
    int n = (int)pdivisors[b].size();
    for (int k=0; k<powmod(2,n); k++) {
        vint v = NumberUtils::digits(k,2,n);
        ll d = 1;
        for (int i=0; i<n; i++) {
            if (v[i]) d *= pdivisors[b][i];
        }
        D.push_back(d);
    }
    return D;
}

ll CCP (ll M, const vll &D) // count of 1 <= a <= M, such that GCD(a,b) == 1. We only care about all squarefree divisors of b, stored in D
{
    if (M <= 0) return 0;
    ll sum = 0;
    for (int i=0; i<(int)D.size(); i++) {
        sum += (M/D[i])*mu[D[i]];
    }
    return sum;
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
    
    mu = MoebuisMuSieve(B);
    
    for (int i=2; i<=B; i++) {
        if (!pdivisors[i].empty()) continue;
        for (int j=i; j<=B; j+=i) pdivisors[j].push_back(i);
    }
    
    for (ll b=1; b*(b+1)<=N; b++) {
        ll M = N/b;
        vll D = squarefree_divisors(b);
        vector<pair<ll,pll>> A = get_division_blocks(M);
        for (int i=0; i<(int)A.size(); i++) {
            ll L = max(A[i].sc.fs-b,0ll);
            if (L >= b) continue;
            ll R = min(max(A[i].sc.sc-b,0ll),b-1);
            if (R <= 0) continue;
            ans += A[i].fs * (CCP(R,D) - CCP(L-1,D));
        }
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
