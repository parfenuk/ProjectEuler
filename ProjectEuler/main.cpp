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

const int Q = 1000000007;
const ll i2 = inverse(2,Q);

ll s1 (ll a, ll b) // Sum(k,k=a..b)
{
    if (a > b) return 0;
    a %= Q; b %= Q;
    return (b+a)*(b-a+Q+1)%Q*i2%Q;
    //Lnum A(a), B(b);
    //return ((B+A)*(B-A+1)/2)[0] % Q;
}


ll A (ll n) // A(n) = Sum(d*(n/d),d=1..n)
{
    ll sum = 0;
    vector<pair<ll,pll>> B = get_division_blocks(n);
    for (int i=0; i<(int)B.size(); i++) {
        ll m = B[i].fs % Q, a = B[i].sc.fs, b = B[i].sc.sc;
        sum += m*s1(a,b);
        sum %= Q;
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
    
    Eratosthenes_sieve(10000000);
    vchar mu = MoebuisMuSieve(10000000);
    
    const ll N = 100000000000000;
    
    for (ll k=1; k*k<=N; k++) {
        if (mu[k] == 0) continue;
        ans += k*(mu[k]+Q)%Q*A(N/k/k);
        ans %= Q;
    }
    
    ans = (ans + Q - s1(1,N)) % Q;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
