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

const int Q = 1234567891;

vull inv;
map<ll,vll> M[30];

ll C (ll N, int k)
{
    ll s = 1;
    for (ll n=N; n>N-k; n--) s = s*n % Q;
    for (ll i=2; i<=k; i++) s = s*inv[i] % Q;
    return s;
}

ll F (ll N, int m, int k) // k^m <= N is always checked before
{
    if (m == 1) return 1;
    if (m == 2) return 1 + 2*max(0ll,N/k-k);
    vll &v = M[m][N];
    if (v.size() > k && v[k] != -1) return v[k];
    
    ll sum = 1;
    
    ll k1p = 1;
    vll kp = Combinatorics::generate_powers((ll)k,m-1,Q);
    for (int p=m-1; p>=1; p--) {
        k1p = k1p*(k+1)%Q; // powmod(k+1,m-p)
        ll nkp = N/kp[p];
        if (k1p > nkp) break;
        
        ll s = 0;
        for (int i=k+1;;i++) {
            if (powmod(i,m-p) > nkp) break;
            ll val = F(nkp,m-p,i);
            s += val;
            if (s >= Q) s -= Q;
        }
        s = s*C(m,p) % Q;
        sum += s;
        if (sum >= Q) sum -= Q;
    }
    
    while (v.size() <= k) v.push_back(-1);
    v[k] = sum;
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
    
    const ll N = 1000000000;
    
    inv = Algebra::generate_inverses(30,Q);
    ans = 1 + N*(N-1)%Q; // case 0 and 1 values > 2
    
    for (int m=2;;m++) {
        cout << "m = " << m << endl;
        if (powmod(2,m) > N) break;
        ll s = 0;
        for (int k=2; powmod(k,m)<=N; k++) {
            s += F(N,m,k);
            if (s >= Q) s -= Q;
        }
        s = s*C(N,m)%Q;
        ans += s;
        if (ans >= Q) ans -= Q;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
