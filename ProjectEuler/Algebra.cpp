//
//  Algebra.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/3/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

// ********** ALGEBRAIC AND NUMERIC FUNCTIONS **********

#include "Algebra.hpp"

namespace Algebra
{

vector<bool> isPrime;
vector<int> primes = {2}; // this is a special prime number, which is always here
vector<int> primePi;
vector<int> eulerPhi;
map<ll,ll> eulerCache;
vector<ll> eulerPhiSum; // prefill small values for function

ull GCD (ull a, ull b)
{
    while (a && b) {
        if (a > b) a %= b;
        else b %= a;
    }
    return a + b;
}

ull LCM (ull a, ull b)
{
    return a*b / GCD(a,b);
}

pll Extended_Euclid (ll a, ll b) // returns <k1,k2>: a*k1 + b*k2 == GCD(a,b)
{
    if (b == 0) return mp(1,0);
    pll qr = mp(a/b, a%b);
    pll st = Extended_Euclid(b, qr.sc);
    return mp(st.sc, st.fs - qr.fs*st.sc);
}

vector<ull> Divisors (ull n) // returns in sorted order
{
    vector<ull> a, b;
    for (ull i=1; i*i<=n; i++) {
        
        if (n%i == 0) {
            a.push_back(i);
            if (i*i != n) b.push_back(n/i);
        }
    }
    while (!b.empty()) { a.push_back(b.back()); b.pop_back(); }
    return a;
}

vector<ull> Divisors_square (ull n) // returns divisors of n^2
{
    vector<ull> a = Divisors(n);
    
    set<ull> s;
    for (int i=0; i<(int)a.size(); i++) for (int j=0; j<(int)a.size(); j++) {
        s.insert(a[i]*a[j]);
    }
    a.clear();
    for (set<ull>::iterator it = s.begin(); it != s.end(); it++) a.push_back(*it);
        
    return a;
}

ull Divisors_sum (ull n)
{
    vector<ull> a = Divisors(n);
    if (a.empty()) return 0;
    a.pop_back();
    ull sum = 0;
    for (int i=0; i<(int)a.size(); i++) sum += a[i];
    return sum;
}

ull Divisors_count (ull n)
{
    vector<pull> a = factorize(n);
    ull s = 1;
    for (int i=0; i<(int)a.size(); i++) s *= (a[i].sc+1);
    return s;
}

// returns a*b % mod.
// Useful if direct calculations are impossible in int64
ull productmod (ull a, ull n, ull mod)
{
    ull b = 0;
    while (n) {
        if (n%2 == 0) {
            n /= 2;
            a = a + a;
            if (a >= mod) a -= mod;
        }
        else {
            n--;
            b = b + a;
            if (b >= mod) b -= mod;
        }
    }
    return b;
}

ll powmod (ll a, ll k, ll mod/*=0*/)
{
    ll b = 1;
    while (k) {
        if (k%2 == 0) {
            k /= 2;
            a *= a;
            if (mod) a %= mod;
        }
        else {
            k--;
            b *= a;
            if (mod) b %= mod;
        }
    }
    return b;
}

ll inverse (ll a, ll mod, ll p/*=0*/) // returns x: a*x % mod == 1. mod = p^n, GCD(a,mod) = 1
{
    if (p == 0) p = mod;
    return powmod(a, mod-1-mod/p, mod);
}

ull modular_sqrt (ull a, ull p) // finds r, r^2 == a mod p (prime), returns 0 if doesn't exist
{
    if (powmod(a,(p-1)/2,p) == p-1) return 0;
    ll b = 2;
    while (powmod(b,(p-1)/2,p) != p-1) b++;
    ull t = p-1, s = 0;
    while (t % 2 == 0) { t /= 2; s++; }
    ll ai = inverse(a,p);
    ll c = powmod(b,t,p), r = powmod(a,(t+1)/2,p), e = powmod(2,s-1,p);
    
    for (int i=1; i<s; i++) {
        
        e = e*(p+1)/2 % p;
        ll d = powmod((r*r % p)*ai % p, e, p);
        if (d == p-1) r = r*c % p;
        c = c*c % p;
    }
    
    return r;
}

void Eratosthenes_sieve (int n, bool fill_primes/*=false*/, bool fill_pi/*=false*/)
{
    isPrime = vector<bool> (n+1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i=2; i<=n; i++) {
        if (isPrime[i] && i*1ll*i <= n) {
            for (int j=i*i; j<=n; j+=i) isPrime[j] = false;
        }
    }
    
    if (fill_primes) for (int i=3; i<=n; i++) if (isPrime[i]) primes.push_back(i);
    if (fill_pi) {
        primePi = vector<int>(n+1);
        for (int i=2; i<=n; i++) primePi[i] = primePi[i-1] + isPrime[i];
    }    
}

bool primeQ (ull n)
{
    if (n < (int)isPrime.size()) return isPrime[n];
    if (n%2 == 0 && n != 2) return false;
    for (ull i=3; i*i<=n; i+=2) if (n % i == 0) return false;
    return true;
}

bool isProbablePrime (ull n)
{
    for (int i=0; i<(int)primes.size(); i++) {
        if (primes[i]*1ll*primes[i] > n) break;
        if (n % primes[i] == 0) return false;
    } return true;
}

ull least_prime_divisor (ull n)
{
    for (int i=0; i<(int)primes.size() && primes[i]*1ll*primes[i]<=n; i++) {
        if (n % primes[i] == 0) return primes[i];
    }
    ull from = primes.back() == 2 ? 3 : primes.back() + 2;
    for (ull i=from; i*i<=n; i+=2) {
        if (n%i == 0) return i;
    } return n;
}

vector<pull> factorize (ull n)
{
    vector<pull> a;
            
    for (int i=0; i<(int)primes.size() && primes[i]*1ll*primes[i]<=n; i++) {
        
        int k = 0;
        while (n % primes[i] == 0) {
            n /= primes[i];
            k++;
        }
        if (k) a.push_back(make_pair(primes[i],k));
    }
    
    ull from = primes.back() == 2 ? 3 : primes.back() + 2;
    
    for (ull i=from; i*i<=n; i+=2) {
        
        int k = 0;
        while (n % i == 0) {
            n /= i;
            k++;
        }
        if (k) a.push_back(make_pair(i,k));
    }
    
    if (n != 1) a.push_back(make_pair(n,1));
    return a;
}

ll primitiveRoot (ll p) // p is prime
{
    vector<pull> f = factorize(p-1);
    
    for (ll res=2; res<p; res++) {
        bool ok = true;
        for (int i=0; i<(int)f.size() && ok; i++) ok &= powmod(res, (p-1)/f[i].fs, p) != 1;
        if (ok) return res;
    }
    
    return -1;
}

ull rad (ull n)
{
    vector<pull> v = factorize(n);
    ull s = 1;
    for (int i=0; i<(int)v.size(); i++) s *= v[i].fs;
    return s;
}

// all divs must be coprime and in the form p^n
ull Chinese_theorem (vector<ll> divs, vector<ll> rests)
{
    vector<ll> prime_divs; // find the only p[i] is the divisor of divs[i]
    for (int i=0; i<(int)divs.size(); i++) {
        prime_divs.push_back(least_prime_divisor(divs[i]));
    }
    
    int k = (int)rests.size();
    vector<ll> x(k);
    for (int i=0; i<k; i++) {
        x[i] = rests[i];
        for (int j=0; j<i; j++) {
            x[i] = (x[i] - x[j]) * (ll)inverse(divs[j], divs[i], prime_divs[i]);
            x[i] %= divs[i];
            if (x[i] < 0) x[i] += divs[i];
        }
    }
    
    ull a = 0;
    for (int i=0; i<k; i++) {
        ull b = x[i], s = 1;
        for (int j=0; j<i; j++) s *= divs[j];
        b *= s;
        a += b;
    }
    
    return a;
}

int MoebiusMu (ull n)
{
    vector<pull> v = factorize(n);
    for (int i=0; i<(int)v.size(); i++) if (v[i].sc > 1) return 0;
    return v.size() % 2 ? -1 : 1;
}

vector<char> MoebuisMuSieve (ull n)
{
    vector<char> mu(n+1,1);
    for (ull i=2; i<=n; i++) {
        if (primeQ(i)) {
            for (ull j=i; j<=n; j+=i) mu[j] = -mu[j];
            for (ull j=i*i; j<=n; j+=i*i) mu[j] = 0;
        }
    }
    return mu;
}

ull EulerPhi (ull n)
{
    if (n < (int)eulerPhi.size()) return (ull)eulerPhi[n];
    
    ull s = 1;
    vector<pull> a = factorize(n);
    
    for (int i=0; i<(int)a.size(); i++) {
        ull k = powmod(a[i].fs, a[i].sc) - powmod(a[i].fs, a[i].sc-1);
        s *= k;
    }
    
    return s;
}

void EulerPhiSieve (ull n)
{
    eulerPhi = vector<int>(n+1);
    vector<bool> sieve(n+1);
    for (int i=1; i<=n; i++) eulerPhi[i] = i;
    for (int i=2; i<=n; i++) {
        if (!sieve[i]) {
            for (int j=i; j<=n; j+=i) {
                sieve[j] = true;
                eulerPhi[j] /= i;
                eulerPhi[j] *= i-1;
            }
        }
    }
}

ll EulerPhiSum (ll n, int Q) // return EulerPhi(1) + EulerPhi(2) + ... + EulerPhi(n) modulo Q
{
    if (n < (ll)eulerPhiSum.size()) return eulerPhiSum[n];
    if (eulerCache.find(n) != eulerCache.end()) return eulerCache[n];
    
    ll u = NumberUtils::lower_integer_sqrt(n);
    ll res;
    if (n % 2 == 0) res = ((n/2)%Q)*((n+1)%Q) % Q;
    else res = (((n+1)/2)%Q)*(n%Q) % Q;
    
    for (ll i=2; i<=u; i++) {
        res -= EulerPhiSum(n/i, Q);
        if (res < 0) res += Q;
    }
    
    ll ub = u;
    if (u == n/u) ub--;
    
    for (ll i=1; i<=ub; i++) {
        res -= (((n/i - n/(i+1))%Q)*EulerPhiSum(i,Q) % Q);
        if (res < 0) res += Q;
    }
    
    eulerCache[n] = res;
    return res;
}

ull square_representations_count (ull n) // n = a^2 + b^2, 0 < a <= b
{
    vector<pull> f = factorize(n);
    int B = 1, a0 = 0;
    for (int i=0; i<(int)f.size(); i++) {
        if (f[i].fs == 2) a0 = (int)f[i].sc;
        else if (f[i].fs % 4 == 1) B *= (f[i].sc+1);
        else if (f[i].fs % 4 == 3 && f[i].sc % 2) return 0;
    }
    if (B % 2 == 0) return B/2;
    if (a0 % 2 == 0) B--;
    else B++;
    return B/2;
}

// returns amount of numbers in range [from,to] that are coprime to N
ull coprime_count_in_range (ull N, ull from, ull to)
{
    if (from > to) return 0;
    if (from != 1) return coprime_count_in_range(N,1,to) - coprime_count_in_range(N,1,from-1);
    
    // now from == 1
    vector<ull> D = Divisors(N);
    ll res = 0;
    for (int i=0; i<(int)D.size(); i++) res += MoebiusMu(D[i])*(to/D[i]);
    return res;
}

ull power_fact (ull n, ull p) // n! = S * p^x, returns x, p is prime
{
    // TODO: investigate https://proofwiki.org/wiki/Factorial_Divisible_by_Prime_Power
    if (p > n) return 0;
    
    ull cnt = 0, s = p;
    while (true) {
        cnt += n/s;
        if (s > n/p) break;
        s *= p;
    }
    
    return cnt;
}

// TODO: add functions least_divisible_by and greatest_divisible_by
// returns count of numbers a in range [lb,ub] that a%n == mod
ull count_divisible_by (ull n, ull lb, ull ub, ull mod/*=0*/)
{
    ull add = n - mod;
    if (lb == 0) return (ub+add)/n;
    return (ub+add)/n - (lb+add-1)/n;
}

// TODO: add mod parameter
// sum of all numbers in range [lb,ub] that are divisible by n
ull sum_divisible_by (ull n, ull lb, ull ub)
{
    ull cnt = ub/n - (lb-1)/n;
    if (cnt == 0) return 0;
    
    ull a = lb%n == 0 ? lb : lb + n - lb%n;
    ull b = ub - ub%n;
    
    return (a+b)*cnt/2;
}

// TODO: redesign so it can return a subspace of solutions
// 0 - no solutions, 1 - one solution, 2 - infinitely many solutions
char Gauss (vector<vector<dd>> a, vector<dd> &ans)
{
    int n = (int)a.size();
    int m = (int)a[0].size() - 1;
    
    vector<int> where(m,-1);
    for (int col=0, row=0; col<m && row<n; col++) {
        
        int sel = row;
        for (int i=row; i<n; i++) {
            if (abs(a[i][col]) > abs(a[sel][col])) sel = i;
        }
        if (abs(a[sel][col]) < EPS) continue;
        for (int i=col; i<=m; i++) swap(a[sel][i],a[row][i]);
        where[col] = row;
        
        for (int i=0; i<n; i++) {
            if (i != row) {
                dd c = a[i][col] / a[row][col];
                for (int j=col; j<=m; ++j) a[i][j] -= a[row][j]*c;
            }
        }
        row++;
    }
    
    ans.assign(m,0);
    for (int i=0; i<m; i++) {
        if (where[i] != -1) ans[i] = a[where[i]][m] / a[where[i]][i];
    }
    for (int i=0; i<n; i++) {
        dd sum = 0;
        for (int j=0; j<m; ++j) sum += ans[j]*a[i][j];
        if (abs(sum - a[i][m]) > EPS) return 0;
    }
    
    for (int i=0; i<m; ++i) {
        if (where[i] == -1) return 2;
    }
    
    return 1;
}

}
