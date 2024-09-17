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

const int Q = 999999017;
const ll i2 = inverse(2,Q);
const ll i6 = inverse(6,Q);

const int M = 7000000; // cache size
vll r(M+1,-1);
map<ll,ll> rc;

void set_caches()
{
    EulerPhiSieve(M);
    r[1] = 1;
    for (ll i=2; i<=M; i++) {
        r[i] = (r[i-1] + i*EulerPhi(i)) % Q;
    }
}

ll S1 (ll n) { n %= Q; return n*(n+1) % Q * i2 % Q; }
ll S2 (ll n) { n %= Q; return n*(n+1) % Q * (2*n+1) % Q * i6 % Q; }

ll R (ll n) // calculates Sum[k*phi(k), 1 <= k <= n]
{
    if (n <= M && r[n] != -1) return r[n];
    if (n > M && rc.find(n) != rc.end()) return rc[n];
    
    ll res = 0;
    ll q = NumberUtils::lower_integer_sqrt(n);
    
    // using this formula: https://math.stackexchange.com/questions/316376/how-to-calculate-these-totient-summation-sums-efficiently
    for (ll k=2; k<=n/q; k++) {
        res += (Q + S1(k) - S1(k-1)) * R(n/k);
        res %= Q;
    }
    for (ll m=1; m<q; m++) {
        res += (Q + S1(n/m) - S1(n/(m+1))) * R(m);
        res %= Q;
    }
    res = (Q + S2(n) - res) % Q;
    
    if (n <= M) r[n] = res;
    else rc[n] = res;
    return res;
}

ll R (ll a, ll b)
{
    ll res = R(b) - R(a-1);
    if (res < 0) res += Q;
    return res;
}

ull F (ll N)
{
    ull ans = N % Q;
    
    const ll n = NumberUtils::lower_integer_sqrt(N);
    
    ull sum = 0;
    for (ll k=2; k<=n; k++) {
        sum += (N/k) % Q * k % Q * EulerPhi(k) % Q;
        sum %= Q;
    }
    for (ll a=1; a<=n; a++) {
        ll k1 = N/(a+1) + 1, k2 = N/a;
        if (k1 <= n) k1 = n+1;
        if (k1 > k2) continue;
        
        sum += a * R(k1,k2) % Q;
        sum %= Q;
    }
    
    ans = (ans + sum*i2) % Q;
    
    return ans;
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
    
    set_caches();
    
    ans = F(99999999019);
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
