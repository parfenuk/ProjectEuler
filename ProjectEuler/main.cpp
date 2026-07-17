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

const int Q = 1000000000;

ll s1 (ll a, ll b) // Sum(k,k=a..b)
{
    if (a > b) return 0;
    Lnum A(a), B(b);
    return ((B+A)*(B-A+1)/2)[0] % Q;
}

vll aSmall, mSmall;
mll aCache, mCache;

ll A (ll n) // A(n) = Sum(d*(n/d),d=1..n)
{
    if (n < aSmall.size()) return aSmall[n];
    if (aCache.find(n) != aCache.end()) return aCache[n];
    
    ll sum = 0;
    vector<pair<ll,pll>> B = get_division_blocks(n);
    for (int i=0; i<(int)B.size(); i++) {
        ll m = B[i].fs % Q, a = B[i].sc.fs, b = B[i].sc.sc;
        sum += m*s1(a,b);
        sum %= Q;
    }
    
    aCache[n] = sum;
    return sum;
}

ll M (ll n) // M(n) = Sum(k*mu[k],k=1..n)
{
    if (n == 0) return 0;
    if (n < mSmall.size()) return mSmall[n];
    if (mCache.find(n) != mCache.end()) return mCache[n];
    
    ll sum = 1;
    vector<pair<ll,pll>> B = get_division_blocks(n);
    for (int i=1; i<(int)B.size(); i++) { // skip (m,(1,1)) block
        ll m = B[i].fs, a = B[i].sc.fs, b = B[i].sc.sc;
        if (b == 1) continue;
        if (a == 1) a = 2;
        sum -= M(m)*s1(a,b);
        sum %= Q;
        if (sum < 0) sum += Q;
    }
    
    mCache[n] = sum;
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
        
    const ll N = 100000000000;
    const int K = 1000000;
    
    Eratosthenes_sieve(K);
    vchar mu = MoebuisMuSieve(K);
    
    mSmall = vll(K+1);
    for (ll i=1; i<=K; i++) {
        mSmall[i] = mSmall[i-1] + i*mu[i];
        if (mSmall[i] >= Q) mSmall[i] -= Q;
        if (mSmall[i] < 0) mSmall[i] += Q;
    }
    
    vector<pair<ll,pll>> B = get_division_blocks(N);
    for (int i=0; i<(int)B.size(); i++) {
        ll m = B[i].fs, a = B[i].sc.fs, b = B[i].sc.sc;
        if (a == b) ans += (a*mu[a]+Q)*powmod(A(m),2,Q);
        else ans += (M(b)-M(a-1)+Q)*powmod(A(m),2,Q);
        ans %= Q;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
