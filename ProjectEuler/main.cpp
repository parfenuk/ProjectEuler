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

const int Q = 993353399;

vector<pair<ull,pull>> Divisors (const ull Fn, const vpull &F)
{
    int N = (int)F.size();
    vull P(N);
    for (int i=0; i<N; i++) P[i] = powmod(F[i].fs, F[i].sc);
    
    ull n = 1, phi = 1;
    vector<pair<ull,pull>> a(1,mp(1,mp(1,Fn%Q))); // < divisor d, < phi(d), Fn*phi(d)/d > >
    vull v(N); // 'a' is answer, 'v' is current powers of primes
    while (true) {
        int i = 0;
        while (i < N && v[i] == F[i].sc) {
            n /= P[i];
            phi /= (P[i] - P[i]/F[i].fs);
            v[i] = 0;
            i++;
        }
        if (i == N) break;
        v[i]++;
        n *= F[i].fs;
        if (v[i] == 1) phi *= (F[i].fs-1);
        else phi *= F[i].fs;
        a.push_back(mp(n,mp(phi%Q,phi*((Fn/n)%Q)%Q)));
    }
    
    sort(a.begin(), a.end());
    reverse(a.begin(), a.end());
    return a;
}

vvpull fact(1000000);
vull nums(1000000);
const ull ten16 = powmod(10,16);

ull solutions_count (const ull N)
{
    ull cnt = 0;
    vector<pair<ull,pull>> D = Divisors(N-1,fact[N-1-ten16]);
    for (int i=0; i<(int)D.size(); i++) {
        ull phi = D[i].sc.fs;
        ull add = 0;
        for (int j=0; j<=i; j++) {
            if (D[j].fs % D[i].fs == 0) {
                add += D[j].sc.sc;
                if (add >= Q) add -= Q;
            }
        }
        add = (add*add) % Q;
        cnt = (cnt + phi*add) % Q;
    }
    ull n = (N-1) % Q;
    cnt = (cnt + n*n) % Q;
    return cnt;
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
    
    for (int i=0; i<1000000; i++) nums[i] = ten16 + i;
    
    Eratosthenes_sieve(100000000);
    for (ull p=2; p<=100000000; p++) { // factorization sieve
        if (!isPrime[p]) continue;
        ull k = ten16 % p;
        ull from = k ? p - k : 0;
        for (ull n=from; n<1000000; n+=p) {
            int deg = 0;
            while (nums[n] % p == 0) { nums[n] /= p; deg++; }
            fact[n].push_back(mp(p,deg));
        }
    }
    for (ull n=0; n<1000000; n++) { // last step of factorization sieve
        if (nums[n] != 1) fact[n].push_back(mp(nums[n],1));
    }
    for (ull n=0; n<1000000; n++) {
        if (fact[n].size() == 1 && fact[n][0].sc == 1) {
            ans += solutions_count(ten16 + n);
            if (ans >= Q) ans -= Q;
        }
    }
    
//    const int N = 19;
//
//    vector<int> c(N-1);
//    for (int i=1; i<N-1; i++) for (int j=1; j<N-1; j++) {
//        c[(i*j)%(N-1)]++;
//    }
//    for (int i=0; i<N-1; i++) cout << i << ": " << c[i] << endl;
//
//
//    c[0] += (2*N-3);
//    for (int i=0; i<N-1; i++) ans += c[i]*c[i];
//    ans += (N-1)*(N-1); // case x = y = 0 (mod n)
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
