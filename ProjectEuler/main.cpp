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
#include <ctime>
#include <cstring>
#include <cassert>

#include <math.h>
#include <vector>
#include <string>
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
//#include "Complex.cpp"
//#include "Pell_Equation.cpp"
#pragma comment(linker, "/STACK:16777216")

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;
typedef unsigned int uint;
typedef int ltype;

using namespace std;

#define pbb pair<bool,bool>
#define pcc pair<char,char>
#define psii pair<sint,sint>
#define pii pair<int,int>
#define pll pair<ll,ll>
#define pull pair<ull,ull>
#define pdd pair<dd,dd>
#define ppii pair<pair<int,int>, pair<int,int>>
#define ppll pair<pair<ll,ll>, pair<ll,ll>>
#define pss pair<string,string>
#define mp make_pair
#define fs first
#define sc second

// TODO: make generator function of all this stuff
//const ull POWER[21] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576};
//const ull POWER[12] = {1,3,9,27,81,243,729,2187,6561,19683,59049,177147};
//const ull FACT[12] = {1,1,2,6,24,120,720,5040,40320,362880,3628800,39916800};

template <class Trzx>
void show (const vector<Trzx> &a, bool show_endl = true, bool show_size = false)
{
    if (show_size) cout << a.size() << endl;
    for (int i=0; i<(int)a.size(); i++) cout << a[i] << " ";
    if (show_endl) cout << endl;
}

template <class Trp>
void show_pair (const vector<Trp> &a, bool show_endl = true, bool show_size = false)
{
    if (show_size) cout << a.size() << endl;
    for (int i=0; i<(int)a.size(); i++) cout << "(" << a[i].fs << " " << a[i].sc << ") ";
    if (show_endl) cout << endl;
}

template <class Trzx>
Trzx total_vector_sum (const vector<Trzx> &a)
{
    Trzx s = 0;
    for (int i=0; i<(int)a.size(); i++) s += a[i];
    return s;
}

// TODO: use templates
void show_matrix (const vector<vector<int>> &a)
{
    for (int i=0; i<(int)a.size(); i++) {
        for (int j=0; j<(int)a[i].size(); j++) cout << a[i][j] << " ";
        cout << endl;
    }
}

void show_tensor (const vector<vector<vector<int>>> &a)
{
    for (int i=0; i<(int)a.size(); i++) {
        for (int j=0; j<(int)a[i].size(); j++) {
            for (int k=0; k<(int)a[i][j].size(); k++) cout << a[i][j][k] << " ";
            cout << endl;
        }
        cout << "------------------------------\n";
    }
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

ll powmod (ll a, ll k, ll mod = 0)
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

ll inverse (ll a, ll mod, ll p = 0) // returns x: a*x % mod == 1. mod = p^n, GCD(a,mod) = 1
{
    if (p == 0) p = mod;
    return powmod(a, mod-1-mod/p, mod);
}

ull ones_mod (ull k, ull mod) // 11...1 k times % mod
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
        s *= inverse(i+1,p); s %= p;
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

vector<bool> isPrime;
vector<int> primes = {2}; // this is a special prime number, which is always here
vector<int> primePi;

void Eratosthenes_sieve (int n, bool fill_primes = false, bool fill_pi = false)
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

ull rad (ull n)
{
    vector<pull> v = factorize(n);
    ull s = 1;
    for (int i=0; i<(int)v.size(); i++) s *= v[i].fs;
    return s;
}

vector<int> eulerPhi;

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

// TODO: add functions least_divisible_by and greatest_divisible_by
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
    for (int i=0; i<(int)D.size(); i++) res += MoebiusMu(D[i])*(to/D[i]);
    return res;
}

bool isPalindrom (const vector<int> &a)
{
    int n = (int)a.size();
    for (int i=0; i<n-1-i; i++) if (a[i] != a[n-1-i]) return false;
    return true;
}

bool is_palindromic_number (ull n)
{
    return isPalindrom(digits(n));
}

// TODO: OMG, so many functions that MUST be put in vectors section and with templates
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
    for (int i=max(0,from); i<=min((int)a.size()-1,to); i++) b.push_back(a[i]);
    return b;
}

// TODO: think about operator+ here
vector<int> join_vectors (vector<int> a, const vector<int> &b)
{
    for (int i=0; i<(int)b.size(); i++) a.push_back(b[i]);
    return a;
}

void append_to (vector<int> &a, const vector<int> &b)
{
    for (int i=0; i<(int)b.size(); i++) a.push_back(b[i]);
}

vector<int> flatten (const vector<vector<int>> &a)
{
    vector<int> b;
    for (int i=0; i<(int)a.size(); i++) for (int j=0; j<(int)a[i].size(); j++) b.push_back(a[i][j]);
    return b;
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

// TODO: test if operator== works here
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
    
    sort(s.begin(), s.end());
    sort(u.begin(), u.end());
    
    for (int i=0; i<(int)s.length(); i++) if (s[i] != u[i]) return false;
    return true;
}

bool next_combination (vector<int> &a, int n, int k)
{
    for (int i=k-1; i>=0; i++) {
        if (a[i] < n-k+i+1) {
            a[i]++;
            for (int j=i+1; j<k; j++) a[j] = a[j-1] + 1;
            return true;
        }
    } return false;
}

vector<vector<int>> get_combinations (int n, int k) // numeration starts from 1
{
    vector<int> a;
    vector<vector<int>> b;
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

// size is k-th Bell number
// total number of elements is k*Bell[k]
// TODO: rewrite or write an additional function that doesn't keep all partitions in memory
vector<vector<vector<char>>> fill_partitions (int k)
{
    const int MAXK = 10;
    vector<vector<vector<char>>> partitions[MAXK+1];
    
    vector<char> s1(1,1);
    vector<vector<char>> p1(1,s1);
    partitions[1].push_back(p1);
    
    for (int n=2; n<=MAXK; n++) {
        
        for (int i=0; i<(int)partitions[n-1].size(); i++) {
            
            vector<vector<char>> p = partitions[n-1][i];
            p.push_back(vector<char>(1,n));
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

void fill_pythagorean_triples (ull n, vector<vector<pull>> &pits, bool primitive_only = true) // a^2 + b^2 = c^2, c <= n
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
        n /= matches[i] + 1;
    }
    
    reverse(v.begin(), v.end());
    return v;
}

vector<vector<int>> find_connected_components (vector<int> g[], int N)
{
    vector<vector<int>> w;
    
    vector<bool> used(N);
    for (int v=0; v<N; v++) {
        
        if (used[v]) continue;
            
        vector<int> component;
        int h = 0, t = 0;
        component.push_back(v);
        t++;
        used[v] = true;
        while (h < t) {
            int cur = component[h++];
            for (int i=0; i<(int)g[cur].size(); i++) {
                int to = g[cur][i];
                if (used[to]) continue;
                used[to] = true;
                component.push_back(to);
                t++;
            }
        }
        w.push_back(component);
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
        
        found = S.find(p,first);
        if (found == string::npos) { v.push_back(S.substr(first, S.length()-first)); break; }
        v.push_back(S.substr(first, found-first));
        while (found != S.length() && S[found] == p) found++;
        if (found == S.length()) break;
        first = found;
    }
    return v;
}

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

// TODO: use templates
int random_integer (int from, int to)
{
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(from,to); // guaranteed unbiased
    return uni(rng);
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
        
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
