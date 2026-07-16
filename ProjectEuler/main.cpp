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

const int Q = 100000000;

ll s1 (ll a, ll b) // Sum(k,k=a..b)
{
    if (a > b) return 0;
    Lnum A(a), B(b);
    return ((B+A)*(B-A+1)/2)[0] % Q;
}

ll s2 (ll a, ll b) // Sum(k^2,k=a..b)
{
    if (a > b) return 0;
    Lnum A(a), B(b);
    Lnum C = (B*(B+1)*(2*B+1) - (A-1)*A*(2*A-1))/6;
    return C[0] % Q;
}

ll s3 (ll a, ll b) // Sum(k^3,k=a..b)
{
    ll c1 = s1(1,b);
    ll c2 = s1(1,a-1);
    ll c3 = (c1*c1 - c2*c2) % Q;
    if (c3 < 0) c3 += Q;
    return c3;
}

ll F0 (ll n)
{
    return EulerPhiSum(n,Q);
}

mll e1,e2;
vll es1,es2;

ll F1 (ll n) // Sum(k*Phi(k),k=1..n)
{
    if (n < (ll)es1.size()) return es1[n];
    if (e1.find(n) != e1.end()) return e1[n];
    
    ll u = NumberUtils::lower_integer_sqrt(n);
    ll res = s2(1,n);
    
    for (ll i=2; i<=u; i++) {
        res -= i*F1(n/i) % Q;
        if (res < 0) res += Q;
    }
    
    ll ub = u;
    if (u == n/u) ub--;
    
    for (ll i=1; i<=ub; i++) {
        res -= s1(n/(i+1)+1,n/i)*F1(i) % Q;
        if (res < 0) res += Q;
    }
    
    e1[n] = res;
    return res;
}

ll F2 (ll n) // Sum(k^2*Phi(k),k=1..n)
{
    if (n < (ll)es2.size()) return es2[n];
    if (e2.find(n) != e2.end()) return e2[n];
    
    ll u = NumberUtils::lower_integer_sqrt(n);
    ll res = s3(1,n);
    
    for (ll i=2; i<=u; i++) {
        res -= i*i%Q*F2(n/i) % Q;
        if (res < 0) res += Q;
    }
    
    ll ub = u;
    if (u == n/u) ub--;
    
    for (ll i=1; i<=ub; i++) {
        res -= s2(n/(i+1)+1,n/i)*F2(i) % Q;
        if (res < 0) res += Q;
    }
    
    e2[n] = res;
    return res;
}

ll t0 (ll a, ll b) // Sum(2^(t-1),t=a..b)
{
    return (powmod(2,b,Q) - powmod(2,a-1,Q) + Q) % Q;
}

ll t1 (ll a, ll b) // Sum(t*2^(t-1),t=a..b)
{
    ll plus = (powmod(2,a,Q) + powmod(2,b,Q)*(b%Q)) % Q;
    ll minus = (powmod(2,b,Q) + powmod(2,a-1,Q)*(a%Q)) % Q;
    return (plus-minus+Q) % Q;
}

ll t2 (ll a, ll b) // Sum(t^2*2^(t-1),t=a..b)
{
    ll plus = ((3+powmod(b%Q,2,Q))*powmod(2,b,Q) + powmod(2,a+1,Q)*(a%Q)) % Q;
    ll minus = (3*powmod(2,a,Q) + powmod(a%Q,2,Q)*powmod(2,a-1,Q) + powmod(2,b+1,Q)*(b%Q)) % Q;
    return (plus-minus+Q) % Q;
}

ll powmodL (ll a, Lnum k)
{
    ll b = 1;
    while (k != Lnum::O) {
        if (k%2 == 0) {
            k /= 2;
            a *= a;
            a %= Q;
        }
        else {
            k--;
            b *= a;
            b %= Q;
        }
    }
    return b;
}

const ll N = 100000000000;
const ll L = N+1;

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
    
    const int K = 1000000;
    EulerPhiSieve(K,Q);
    es1 = es2 = vll(K+1);
    for (ll i=1; i<=K; i++) {
        es1[i] = (es1[i-1] + i*eulerPhi[i]) % Q;
        es2[i] = (es2[i-1] + i*i%Q*eulerPhi[i]) % Q;
    }

    ll H = (2*L%Q*(powmod(2,L,Q) - L%Q - 1)) % Q;
    if (H < 0) H += Q;
    
    ll D = 0;
    vector<pair<ll,pll>> A = get_division_blocks(N);
    for (int i=0; i<(int)A.size(); i++) {
        ll m = A[i].fs;
        ll a = A[i].sc.fs, b = A[i].sc.sc;
        ll d1 = 2*t0(a,b)*powmod(L%Q,2,Q)%Q*(2*F0(m)-1)%Q;
        ll d2 = 2*t1(a,b)*(L%Q)%Q*(3*F1(m)-1)%Q;
        ll d3 = 2*t2(a,b)*F2(m)%Q;
        D += (d1-d2+d3);
        D %= Q;
        if (D < 0) D += Q;
    }
    
    ans = (1 + powmod(L%Q,2,Q) + H + D) % Q;
    
    Lnum LL = Lnum(L)*Lnum(L);
    ll total = powmodL(2,LL);
    ll good = (LL*(LL-1)/2)[0] % Q;
    ll bad = (ans + Q - good) % Q;
    ans = (total + Q - bad) % Q;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
