#include <set>
#include <bitset>
#include <queue>
#include <deque>
#include <stack>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <ctime>
#include <cstring>
#include <string>
#include <cassert>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <numeric>
#include <random>
#include <utility>

//#include "Lnum.cpp"
#include "Geometry.cpp"
//#include "Geometry3D.cpp"
#include "Fractions.cpp"
#include "Matrix.cpp"
#include "Complex.cpp"
//#include "Pell_Equation.cpp"
#pragma comment(linker, "/STACK:416777216")

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;
typedef unsigned int uint;
typedef int ltype;

using namespace std;

#define pii pair<int,int>
#define pll pair<ll,ll>
#define pull pair<ull,ull>
#define pdd pair<dd, dd>
#define ppii pair<pair<int,int>, pair<int,int>>
#define ppll pair<pair<ll,ll>, pair<ll,ll>>
#define mp make_pair
#define fs first
#define sc second

//const ull POWER[21] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576};
//const ull POWER[12] = {1,3,9,27,81,243,729,2187,6561,19683,59049,177147};
//const ull FACT[12] = {1,1,2,6,24,120,720,5040,40320,362880,3628800,39916800};

void show (const vector<int> &a, bool show_endl = true, bool show_size = false)
{
    if (show_size) cout << a.size() << endl;
    for (int i=0; i<(int)a.size(); i++) cout << a[i] << " ";
    if (show_endl) cout << endl;
}

void show (const vector<ull> &a, bool show_endl = true, bool show_size = false)
{
    if (show_size) cout << a.size() << endl;
    for (int i=0; i<(int)a.size(); i++) cout << a[i] << " ";
    if (show_endl) cout << endl;
}

void show (const vector<pii> &a, bool show_endl = true, bool show_size = false)
{
    if (show_size) cout << a.size() << endl;
    for (int i=0; i<(int)a.size(); i++) cout << "(" << a[i].fs << " " << a[i].sc << ") ";
    if (show_endl) cout << endl;
}

void show (const vector<pull> &a, bool show_endl = true, bool show_size = false)
{
    if (show_size) cout << a.size() << endl;
    for (int i=0; i<(int)a.size(); i++) cout << "(" << a[i].fs << " " << a[i].sc << ") ";
    if (show_endl) cout << endl;
}

int total_vector_sum (const vector<int> &a)
{
    int s = 0;
    for (int i=0; i<(int)a.size(); i++) s += a[i];
    return s;
}

ull total_vector_sum (const vector<ull> &a)
{
    ull s = 0;
    for (int i=0; i<(int)a.size(); i++) s += a[i];
    return s;
}

ull GCD (ull a, ull b)
{
    while (a && b) {
        
        if (a > b) a %= b;
        else b %= a;
    }
    return a + b;
}

pll Extended_Euclid (ll a, ll b)
{
    if (b == 0) return mp(1,0);
    pll qr = mp(a/b, a%b);
    pll st = Extended_Euclid(b, qr.sc);
    return mp(st.sc, st.fs - qr.fs*st.sc);
}

ull LCM (ull a, ull b)
{
    return a*b/GCD(a,b);
}

ull power (ull n, int k)
{
    ull s = 1;
    for (int i=0; i<k; i++) s *= n;
    return s;
}

ull powmod(ull a, ull k, int mod = 0)
{
    ull b = 1;
    while (k) {
        if (k%2==0) {
            k /= 2;
            a = a*a;
            if (mod) a %= mod;
        }
        else {
            k--;
            b = b*a;
            if (mod) b %= mod;
        }
    }
    return b;
}

ull inverse (ull a, ull mod, ull p = 0) // returns x: a*x % mod == 1. mod = p^n, GCD(a,mod) = 1
{
    if (p == 0) p = mod;
    return powmod(a,mod-mod/p-1,(int)mod);
}

ull ones_mod (ull k, int mod) // 11...1 k times % mod
{
    ull b = 0, n = 1, a = 1;
    while (k) {
        if (k%2==0) {
            k /= 2;
            a = a*powmod(10,n,mod) + a;
            n *= 2;
            a %= mod;
        }
        else {
            k--;
            b = b*powmod(10,n,mod) + a;
            b %= mod;
        }
    }
    return b;
}

ull Binomial (ull n, ull k, int p = 0) // C(n,k) mod p. p must be prime and greater than n and k.
{
    if (k > n) return 0;
    
    if (p == 0) {
        ull s = 1;
        for (ull i=0; i<k; i++) {
            s *= (n-i);
            s /= (i+1);
        }
        return s;
    }
    
    ull s = 1;
    for (ull i=0; i<k; i++) {
        s *= (n-i); s %= p;
        s *= inverse((i+1),p); s %= p;
    }
    return s;
}

dd decimal_part (dd a)
{
    return a - (int)(a+0.00000001);
}

ull integer_sqrt (ull n) // returns 0 if n != x*x
{
    if (n == 0) return 0;
    
    ull x = (ull)sqrt((dd)(n-0.001));
    while (x*x < n) x++;
    if (x*x == n) return x;
    return 0;
}

ull integer_part_sqrt (ull n) // returns greatest x such that x*x <= n
{
    if (n == 0) return 0;
    
    ull x = (ull)sqrt((dd)(n-0.001));
    while (x*x < n) x++;
    if (x*x == n) return x;
    return x-1;
}

// n = 153 -> (1,5,3)
vector<int> digits (ull n, int base = 10, int min_size = 1)
{
    vector<int> a;
    while (n) {
        
        a.push_back(n%base);
        n /= base;
    }
    while (a.size() < min_size) a.push_back(0);
    reverse(a.begin(), a.end());
    
    return a;
}

ull from_digits (vector<int> a, int base = 10)
{
    ull s = 0;
    for (int i=0; i<(int)a.size(); i++) {
        s *= base;
        s += a[i];
    }
    return s;
}

vector<int> digits (Lnum A)
{
    vector<int> a;
    for (int i=A.get_size()-1; i>=0; i--) {
        vector<int> b = digits(A[i]);
        if (i != A.get_size()-1) while (b.size() < 9) b.insert(b.begin(),0);
        for (int j=0; j<(int)b.size(); j++) a.push_back(b[j]);
    }
    return a;
}

Lnum from_digits_lnum (vector<int> a)
{
    Lnum s;
    for (int i=0; i<(int)a.size(); i++) {
        s = s * 10;
        s = s + Lnum(a[i]);
    }
    return s;
}

// returns a*b % mod.
// Useful if direct calculations are impossible in int64
// mod*81 should fit in int64
ull product_mod (ull a, ull b, ull mod)
{
    ull s = 0;
    vector<int> da = digits(a); int n = (int)da.size();
    vector<int> db = digits(b); int m = (int)db.size();
    vector<ull> mods;
    mods.push_back(1);
    for (int i=1; i<n+m; i++) mods.push_back(mods.back()*10 % mod);
    
    for (int i=0; i<n; i++) for (int j=0; j<m; j++) {
        int deg10 = n-1-i+m-1-j;
        s += mods[deg10]*da[i]*db[j];
        s %= mod;
    }
    
    return s;
}

vector<bool> isPrime;
vector<int> primes;
vector<int> primePi;

void Eratosthenes_sieve (int n, bool fill_primes = false)
{
    isPrime = vector<bool> (n+1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i=2; i<=n; i++) {
        if (isPrime[i] && i*1ll*i <= n) {
            for (int j=i*i; j<=n; j+=i) isPrime[j] = false;
        }
    }
    
    if (fill_primes) for (int i=2; i<=n; i++) if (isPrime[i]) primes.push_back(i);
}

void fill_primePi (int n) // WARNING: call only after Eratosthenes_sieve with size greater than or equal to this n
{
    primePi = vector<int>(n+1);
    int s = 0;
    for (int i=0; i<=n; i++) {
        s += isPrime[i];
        primePi[i] = s;
    }
}

vector<pull> factorize (ull n)
{
    vector<pull> a;
    
    if (!primes.empty()) {
        
        for (ull i=0; primes[i]*1ll*primes[i]<=n; i++) {
            
            int k = 0;
            while (n % primes[i] == 0) {
                n /= primes[i];
                k++;
            }
            if (k) a.push_back(make_pair(primes[i],k));
        }
        
        ull from = primes.back() + 2;
        if (from == 4) from--; // just in the case primes == {2}, which is almost impossible :)
        
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
    
    int p2 = 0;
    while (n % 2 == 0) {
        n /= 2;
        p2++;
    }
    if (p2) a.push_back(make_pair(2,p2));
    
    for (ull i=3; i*i<=n; i+=2) {
        
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

int moebiusMu (ull n)
{
    vector<pull> v = factorize(n);
    for (int i=0; i<(int)v.size(); i++) if (v[i].sc > 1) return 0;
    return v.size() % 2 ? -1 : 1;
}

ull rad (ull n)
{
    vector<pull> v = factorize(n);
    
    ull s = 1;
    for (int i=0; i<(int)v.size(); i++) s *= v[i].fs;
    
    return s;
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
    }
    
    return true;
}

vector<ull> eulerPhi;

ull EulerPhi (ull n)
{
    if (n < (int)eulerPhi.size()) return eulerPhi[n];
    
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
    eulerPhi = vector<ull>(n+1);
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

map<ll,ll> eulerCache;
vector<ll> eulerPhiSum; // prefill small values for function
ll EulerPhiSum (ll n, int Q) // return EulerPhi(1) + EulerPhi(2) + ... + EulerPhi(n) modulo Q
{
    if (n < (ll)eulerPhiSum.size()) return eulerPhiSum[n];
    if (eulerCache.find(n) != eulerCache.end()) return eulerCache[n];
    
    ll u = integer_part_sqrt(n);
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

// all divs must be coprime and in the form p^n
ull Chinese_theorem (vector<ll> divs, vector<ll> rests)
{
    vector<ll> prime_divs; // find the only p[i] is the divisor of divs[i]
    for (int i=0; i<(int)divs.size(); i++) {
        vector<pull> f = factorize(divs[i]);
        prime_divs.push_back(f[0].fs);
    }
    
    int k = (int)rests.size();
    vector<ll> x(k);
    for (int i=0; i<k; i++) {
        x[i] = rests[i];
        for (int j=0; j<i; j++) {
            x[i] = (x[i] - x[j]) * (ll)inverse(divs[j],divs[i],prime_divs[i]);
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

vector<ull> Divisors (ull n) // returns in sorted order!
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

bool isPractical (ull n)
{
    vector<ull> d = Divisors(n);
    ull sum = 0;
    for (int i=0; i<(int)d.size(); i++) {
        if (sum < d[i]-1) return false;
        sum += d[i];
    }
    return true;
}

ull count_divisible_by (ull n, ull mod, ull lb, ull ub) // returns count of numbers a in range [lb,ub] that a%n == mod
{
    ull add = n - mod;
    if (lb == 0) return (ub+add)/n;
    return (ub+add)/n - (lb+add-1)/n;
}

ull sum_divisible_by (ull n, ull lb, ull ub) // sum of all numbers in range [lb,ub] that are divisible by n
{
    ull cnt = ub/n - (lb-1)/n;
    if (cnt == 0) return 0;
    
    ull a = lb%n == 0 ? lb : lb + n - lb%n;
    ull b = ub - ub%n;
    
    return (a+b)*cnt/2;
}

// returns amount of numbers in range [from,to] that are coprime to N
ull coprime_count_in_range (ull N, ull from, ull to)
{
    if (from > to) return 0;
    if (from != 1) return coprime_count_in_range(N,1,to) - coprime_count_in_range(N,1,from-1);
    
    // now from == 1
    vector<ull> D = Divisors(N);
    ll res = 0;
    for (int i=0; i<(int)D.size(); i++) res += moebiusMu(D[i])*(to/D[i]);
    return res;
}

bool isPalindrom (const vector<int> &a)
{
    int n = (int)a.size();
    for (int i=0; i<n; i++) {
        if (a[i] != a[n-1-i]) return false;
    }
    return true;
}

bool is_palindromic_number (ull n)
{
    return isPalindrom(digits(n));
}

int index_of_object (const vector<ull> &a, ull n)
{
    for (int i=0; i<(int)a.size(); i++) if (a[i] == n) return i;
    return -1;
}

int index_of_object (const vector<int> &a, int n)
{
    for (int i=0; i<(int)a.size(); i++) if (a[i] == n) return i;
    return -1;
}

bool contains (const vector<ull> &a, ull n)
{
    return index_of_object(a,n) != -1;
}

bool contains (const vector<int> &a, int n)
{
    return index_of_object(a,n) != -1;
}

bool is_disjoint (const vector<int> &a, const vector<int> &b)
{
    for (int i=0; i<(int)a.size(); i++) {
        if (contains(b,a[i])) return false;
    }
    return true;
}

vector<int> subvector (const vector<int> &a, int from, int to)
{
    vector<int> b;
    for (int i=from; i<=to; i++) b.push_back(a[i]);
    return b;
}

vector<int> join_vectors (vector<int> a, const vector<int> &b)
{
    for (int i=0; i<(int)b.size(); i++) a.push_back(b[i]);
    return a;
}

void join_vectors (vector<int> &a, const vector<int> &b, bool flag = true)
{
    for (int i=0; i<(int)b.size(); i++) a.push_back(b[i]);
}

int count_greater_than (const vector<int> &a, int value) // a - sorted, returns count of elements in vector that are greater than given value
{
    if (a[0] > value) return (int)a.size();
    if (a.back() <= value) return 0;
    
    int lb = 0, ub = (int)a.size()-1;
    while (lb < ub) {
        int M = (lb+ub)/2;
        if (a[M] <= value) lb = M+1;
        else ub = M;
    }
    
    return (int)a.size() - lb;
}

int count_less_than (const vector<int> &a, int value) // a - sorted, returns count of elements in vector that are less than given value
{
    if (a[0] >= value) return 0;
    if (a.back() < value) return (int)a.size();
    
    int lb = 0, ub = (int)a.size()-1;
    while (lb < ub) {
        int M = (lb+ub+1)/2;
        if (a[M] >= value) ub = M-1;
        else lb = M;
    }
    
    return lb+1;
}

ull join_numbers (ull n, ull m)
{
    return from_digits(join_vectors(digits(n), digits(m)));
}

bool same_digits (ull n, ull m)
{
    vector<int> a = digits(n), b = digits(m);
    
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    if (a.size() != b.size()) return false;
    
    for (int i=0; i<(int)a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool same_letters (string s, string u)
{
    if (s.length() != u.length()) return false;
    vector<char> p, q;
    for (int i=0; i<(int)s.length(); i++) { p.push_back(s[i]); q.push_back(u[i]); }
    
    sort(p.begin(),p.end());
    sort(q.begin(),q.end());
    
    for (int i=0; i<(int)s.length(); i++) if (p[i] != q[i]) return false;
    return true;
}

bool next_combination (vector<int> & a, int n, int k)
{
    for (int i=k-1; i>=0; --i)
        if (a[i] < n-k+i+1) {
            ++a[i];
            for (int j=i+1; j<k; ++j)
                a[j] = a[j-1]+1;
            return true;
        }
    return false;
}

vector<vector<int>> get_combinations (int n, int k)
{
    vector<int>a;
    vector<vector<int>>b;
    for (int i=1; i<=k; i++) a.push_back(i);
    b.push_back(a);
    while (next_combination(a,n,k)) b.push_back(a);
    return b;
}

vector<vector<int>> sum_partitions (int n)
{
    vector<vector<int>> ret;
    
    vector<int> a;
    for (int i=0; i<n; i++) a.push_back(1);
    
    ret.push_back(a);
    while (a[0] != n) {
        for (int j=(int)a.size()-2; j>=0; j--) {
            if (j && a[j] < a[j-1]) {
                a[j]++;
                int k = 0, p = (int)a.size();
                for (int t=j+1; t<p; t++) k += a[t];
                for (int t=j+1; t<p; t++) a.pop_back();
                for (int t=0; t<k-1; t++) a.push_back(1);
                ret.push_back(a);
                break;
            }
            if (j == 0) {
                int k = a[0];
                a.clear();
                a.push_back(k+1);
                for (int t=0; t<n-a[j]; t++) a.push_back(1);
                ret.push_back(a);
            }
        }
    }
    
    return ret;
}

vector<vector<vector<int>>> fill_partitions (int k)
{
    vector<vector<vector<int>>> partitions[10];
    
    vector<int> s1(1,1);
    vector<vector<int>> p1(1,s1);
    partitions[1].push_back(p1);
    
    for (int n=2; n<=9; n++) {
        
        for (int i=0; i<(int)partitions[n-1].size(); i++) {
            
            vector<vector<int>> p = partitions[n-1][i];
            p.push_back(vector<int>(1,n));
            partitions[n].push_back(p);
            p.pop_back();
            
            for (int j=0; j<(int)p.size(); j++) {
                
                p[j].push_back(n);
                partitions[n].push_back(p);
                p[j].pop_back();
            }
        }
    }
    
    return partitions[k];
}

vector<vector<pull>> pits;

void fill_pythagorean_triples (ull n, bool primitive_only = true) // a^2 + b^2 = c^2, c <= n
{
    pits = vector<vector<pull>>(n+1);
    for (ull p=2; p*p<=n; p++) for (ull q=1+p%2; q<p; q+=2) {
        
        if (GCD(p,q) != 1) continue;
        
        ull a = p*p - q*q;
        ull b = 2*p*q;
        ull c = p*p + q*q;
        if (c > n) break;
        
        for (ull k=1;;k++) {
        
            ull A = a*k, B = b*k, C = c*k;
            if (C <= n) pits[C].push_back(make_pair(A,B));
            else break;
            
            if (primitive_only) break;
        }
    }
}

ll code_from_v (const vector<int> &v, const vector<int> &matches) // returns number in dynamic base
{
    ll ret = v[0];
    
    for (int i=1; i<(int)v.size(); i++) {
        ret *= (matches[i]+1);
        ret += v[i];
    }
    
    return ret;
}

vector<int> v_from_code (ll n, const vector<int> &matches)
{
    vector<int> v;
    
    for (int i=(int)matches.size()-1; i>=0; i--) {
        v.push_back(n % (matches[i]+1));
        n /= (matches[i] + 1);
    }
    
    reverse(v.begin(), v.end());
    return v;
}

vector<vector<int>> find_connected_components (vector<int> g[], int N)
{
    vector<vector<int>> w;
    
    vector<bool> used(N);
    for (int v=0; v<N; v++) {
        
        if (!used[v]) {
            
            vector<int> q;
            int h=0, t=0;
            q.push_back(v);
            t++;
            used[v] = true;
            while (h < t)
            {
                int cur = q[h++];
                for (vector<int>::iterator i=g[cur].begin(); i!=g[cur].end(); ++i)
                    if (!used[*i])
                    {
                        used[*i] = true;
                        q.push_back(*i);
                        t++;
                    }
            }
            w.push_back(q);
        }
    }
    
    return w;
}

vector<string> parse_by_symbol (const string &S, char p)
{
    vector<string> v;
    size_t found = 0, first = 0;
    while (first != S.length() && S[first] == p) first++;
    if (first == S.length()) return v;
    
    while (true) {
        
        found = S.find(p, first);
        if (found == string::npos) { v.push_back(S.substr(first, S.length()-first)); break; }
        v.push_back(S.substr(first, found-first));
        while (found != S.length() && S[found] == p) found++;
        if (found == S.length()) break;
        first = found;
    }
    return v;
}

int Gauss (vector<vector<dd>> a, vector<dd> &ans) // 0 - no solutions, 1 - one solution, 2 - infinitely many solutions
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

vector<ull> Lagged_Fibonacci_Generator (int n)
{
    vector<ull> s;
    
    for (ull i=1; i<=n; i++) {
        
        if (i <= 55) s.push_back((i*i*i*300007 - i*200003 + 100003) % 1000000);
        else s.push_back((s[i-56]+s[i-25]) % 1000000);
    }
    
    return s;
}

vector<ull> Blub_Blub_Shum_Generator (int n)
{
    vector<ull> v;
    ull a = 290797;
    
    for (int i=0; i<n; i++) {
        v.push_back(a);
        a = a*a % 50515093;
    }
    
    return v;
}

struct vertex {
    int next[2]; // pointers to the children
    bool leaf; // is forbidden string on this vertex
    int p; // ancestor
    char pch; // edge from acnestor to
    int link; // suffix_link
    int go[2]; // array of suffix_links with appending go[c] symbol
    vertex() { next[0] = next[1] = -1; leaf = false; p = -1; pch = -1; link = -1; go[0] = go[1] = -1; }
};

vector<vertex> g;

void add_string (const string &s)
{
    int v = 0;
    for (size_t i=0; i<s.length(); i++) {
        char c = s[i] - '0';
        if (g[v].next[c] == -1) {
            vertex w;
            w.p = v;
            w.pch = c;
            g[v].next[c] = (int)g.size();
            g.push_back(w);
        }
        v = g[v].next[c];
    }
    g[v].leaf = true;
}

int go (int v, char c);

int get_link (int v)
{
    if (g[v].link == -1) {
        if (v == 0 || g[v].p == 0) g[v].link = 0;
        else g[v].link = go(get_link(g[v].p), g[v].pch);
    }
    return g[v].link;
}

int go (int v, char c)
{
    if (g[v].go[c] == -1) {
        if (g[v].next[c] != -1) g[v].go[c] = g[v].next[c];
        else g[v].go[c] = v==0 ? 0 : go(get_link(v), c);
    }
    return g[v].go[c];
}

vector<ull> total_count;

void fill_total_count (int L) // amount of 'good' numbers with length L
{
    total_count.push_back(0);
    total_count.push_back(1);
    
    int N = (int)g.size();
    int v = g[0].go[1];
    
    vector<ull> a(N);
    a[v] = 1;
    
    for (int z=0; z<L-1; z++) {
        
        vector<ull> b(N);
        for (int i=0; i<N; i++) {
            int l = g[i].go[0], r = g[i].go[1];
            if (!g[l].leaf) b[l] += a[i];
            if (!g[r].leaf) b[r] += a[i];
        }
        a.swap(b);
        
        total_count.push_back(total_vector_sum(a));
    }
}

ull get_count (const string &s, int L) // amount of 'good' numbers with length L and starting from s
{
    int N = (int)g.size();
    int v = 0;
    for (int i=0; i<(int)s.length(); i++) {
        v = g[v].go[s[i]-'0'];
        if (g[v].leaf) return 0;
    }
    
    vector<ull> a(N);
    a[v] = 1;
    
    for (int z=0; z<L-(int)s.length(); z++) {
        
        vector<ull> b(N);
        for (int i=0; i<N; i++) {
            int l = g[i].go[0], r = g[i].go[1];
            if (!g[l].leaf) b[l] += a[i];
            if (!g[r].leaf) b[r] += a[i];
        }
        a.swap(b);
    }
    
    return total_vector_sum(a);
}

string string_by_number (ull n) // returns n'th good number in binary string form
{
    int L = 1;
    ull cnt = 0;
    string ret = "1";
    
    while (true) {
        if (cnt + total_count[L] >= n) break;
        cnt += total_count[L];
        L++;
    }
    
    // now we know what length this number will have: L
    ull res = n - cnt;
    
    for (int l=2; l<=L; l++) {
        
        string ret0 = ret + "0", ret1 = ret + "1";
        ull c = get_count(ret0,L);
        if (c < res) { res -= c; ret = ret1; }
        else ret = ret0;
    }
    
    return ret;
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    //vector<string> forbidden_strings = {"000","111"};
    vector<string> forbidden_strings = {"000","111","00100","01010","10101","11011"};
    
    // generation forbidden strings of sizes 8, 14, 26, 50, 98, 194, 386, 770
    for (int z=1; z<=3; z++) {

        int n = (int)forbidden_strings.size();
        for (int i=n-4; i<=n-1; i++) {
            string s = forbidden_strings[i];
            string u; u.push_back(s[0]);
            for (int j=1; j<(int)s.length()-1; j++) {
                if (s[j] == '0') u += "01";
                else u += "10";
            }
            if (s.back() == '0') u.push_back('1');
            else u.push_back('0');
            forbidden_strings.push_back(u); // TODO: not to keep them in memory
        }
    }
    
    // init suffix automata by creation a root vertex
    g.push_back(vertex()); // adding tree root
    for (int i=0; i<(int)forbidden_strings.size(); i++) add_string(forbidden_strings[i]);
    for (int i=0; i<(int)g.size(); i++) { get_link(i); go(i,0); go(i,1); }

    // *** TREE CONSTRUCTION TEST ***
    
    cout << "Tree size: " << g.size() << endl;
//    for (int i=0; i<(int)g.size(); i++) {
//        cout << i << ": " << g[i].next[0] << " " << g[i].next[1] << endl;
//        cout << g[i].leaf << " " << g[i].link << " " << g[i].go[0] << " " << g[i].go[1] << endl;
//        cout << endl;
//    }
    
    // **** COUNTINGS ****
    
    fill_total_count(1000);
    for (int n=33; n<1000; n++) {
        if (total_count[n] == total_count[n-32] + total_count[n-16]) {
            cout << "Equality holds for n >= " << n << endl;
            break;
        }
    }
    for (int n=1; n<200; n++) {
        //cout << n << " " << total_count[n] << " ";
        if (n >= 1) {
            cout << get_count("0",n) << " " << get_count("00",n+1) << " ";
            cout << get_count("01",n+1) << " " << get_count("0010",n+3);
        }
        cout << endl;
    }
    
    // ***** NUMBERS OBTAINING *****
    
//    for (int i=1; i<=1000; i++) {
//        string s = string_by_number(i);
//        vector<int> d;
//        for (int j=0; j<(int)s.length(); j++) d.push_back(s[j]-'0');
//        cout << from_digits(d,2) << endl;
//    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
