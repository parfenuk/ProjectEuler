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

#include "Lnum.cpp"
#include "Geometry.cpp"
#include "Fractions.cpp"
#include "Matrix.cpp"
#include "Complex.cpp"
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

ull Binomial (ull n, ull k) // C(n,k)
{
    if (k > n) return 0;
    
    ull s = 1;
    for (ull i=0; i<k; i++) {
        s *= (n-i);
        s /= (i+1);
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

void fill_primePi (int n) // WARNING: call only after Erathosthenes_sieve with size greater than or equal to this n
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
    
    for (ull i=2; i*i<=n; i++) {
        
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

ll miobius (ull n)
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

ull EulerPhi (ull n)
{
    ull s = 1;
    vector<pull> a = factorize(n);
    
    for (int i=0; i<(int)a.size(); i++) {
        ull k = powmod(a[i].fs, a[i].sc) - powmod(a[i].fs, a[i].sc-1);
        s *= k;
    }
    
    return s;
}

// Currently works only for prime rests. Implement inverse function that commented
ull Chinese_theorem (vector<ull> divs, vector<ull> rests)
{
    int k = (int)rests.size();
    vector<ll> x(k);
    for (int i=0; i<k; i++) {
        x[i] = rests[i];
        for (int j=0; j<i; j++) {
            //x[i] = inverse(divs[j],divs[i]) * (x[i]-x[j]);
            x[i] = powmod(divs[j],divs[i]-2,(int)divs[i]) * (x[i]-x[j]);
            x[i] %= (int)divs[i];
            if (x[i] < 0)  x[i] += divs[i];
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

ull power_fact (ull n, ull k) // n! = S * k^x, returns x
{
    if (k > n) return 0;
    
    ull p = 0;
    for (ull i=k; i<=n; i+=k) {
        
        ull s = i;
        while (s % k == 0) { s /= k; p++; }
    }
    return p;
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

ull sum_divisible_by (ull n, ull lb, ull ub) // sum of all numbers in range [lb,ub] that are divisible by n
{
    ull cnt = ub/n - (lb-1)/n;
    if (cnt == 0) return 0;
    
    ull a = lb%n == 0 ? lb : lb + n - lb%n;
    ull b = ub - ub%n;
    
    return (a+b)*cnt/2;
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

vector<pull> *pits;

void fill_piphagorean_triplets (ull n, bool primitive_only = true) // a^2 + b^2 = c^2, c <= n
{
    pits = new vector<pull> [n+1];
    for (ull p=2; p*p<=n; p++) for (ull q=1+p%2; q<p; q+=2) {
        
        if (GCD(p,q) != 1) continue;
        
        ull a = p*p - q*q;
        ull b = 2*p*q;
        ull c = p*p + q*q;
        if (c > n) break;
        
        for (ull k=1;;k++) {
        
            a *= k; b *= k; c *= k;
            if (c <= n) pits[c].push_back(make_pair(a,b));
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
    ull a = 14025256;
    
    vector<bool> used(20300713);
    
    for (int i=0; i<n; i++) {
        if (used[a]) break;
        v.push_back(a);
        used[a] = true;
        a = a*a % 20300713;
    }
    
    return v;
}

const int CFin[14][4] = { // configurations for inputes 1,3,5,7 correspondingly
    {2,4,6,8},
    {2,4,8,6},
    {2,6,4,8},
    {2,8,4,6},
    {2,8,6,4},
    {4,2,6,8},
    {4,2,8,6},
    {6,2,4,8},
    {6,4,2,8},
    {8,2,4,6},
    {8,2,6,4},
    {8,4,2,6},
    {8,4,6,2},
    {8,6,4,2},
};

const int CFed[2][2] = {
    {2,4},
    {4,2}
};

const ull Q = 10000000000;
int N, M, EDGES = 0;
vector<int> g[100];

void enumerate_edges()
{
    int K = 0, E = 0, S = M+1;
    
    // fill edges count
    for (int i=0; i<=N; i++) for (int j=0; j<=M; j++) {
        
        int side = 0;
        if (i == 0 || i == N) side++;
        if (j == 0 || j == M) side++;
        
        if (side == 0) g[K] = vector<int>(9);
        if (side == 1) g[K] = vector<int>(5);
        if (side == 2) g[K] = vector<int>(3);
        K++;
    }
    
    // fill edges
    K = 0;
    for (int i=0; i<=N; i++) for (int j=0; j<=M; j++) {
        
        if (i == 0) { // top case
            
            if (j == 0) { // top-left corner case
                
                g[K][1] = g[K+S][4] = E++;
                g[K][2] = g[K+1][1] = E++;
            }
            else if (j == M) { // top-right corner case
                
                g[K][2] = g[K+S][1] = E++;
            }
            else {
                
                g[K][2] = g[K+S][1] = E++;
                g[K][3] = g[K+S][2] = E++;
                g[K][4] = g[K+1][1] = E++;
            }
        }
        else if (i == N) { // bottom case
            
            if (j == M-1) { // pre-bottom-right case
                
                g[K][1] = g[K+1][2] = E++;
            }
            else if (j == M) { // bottom-right case
                
            }
            else { // including bottom-left case
                
                g[K][1] = g[K+1][4] = E++;
            }
        }
        else if (j == 0) { // left middle case
            
            if (i == N-1) { // pre-bottom-left case
                
                g[K][1] = g[K+S][2] = E++;
                g[K][2] = g[K+1][7] = E++;
                g[K][3] = g[K+1][8] = E++;
            }
            else {
                
                g[K][1] = g[K+S][4] = E++;
                g[K][2] = g[K+1][7] = E++;
                g[K][3] = g[K+1][8] = E++;
            }
        }
        else if (j == M) { // right middle case
            
            g[K][4] = g[K+S][1] = E++;
        }
        else { // inner case
            
            if (i == N-1) {
                
                if (j == M-1) {
                    
                    g[K][3] = g[K+1][2] = E++;
                    g[K][4] = g[K+1][3] = E++;
                    g[K][5] = g[K+S][2] = E++;
                    g[K][6] = g[K+S][3] = E++;
                }
                else {
                    
                    g[K][3] = g[K+1][8] = E++;
                    g[K][4] = g[K+1][7] = E++;
                    g[K][5] = g[K+S][2] = E++;
                    g[K][6] = g[K+S][3] = E++;
                }
            }
            else if (j == M-1) {
                
                g[K][3] = g[K+1][2] = E++;
                g[K][4] = g[K+1][3] = E++;
                g[K][5] = g[K+S][2] = E++;
                g[K][6] = g[K+S][1] = E++;
            }
            else {
                
                g[K][3] = g[K+1][8] = E++;
                g[K][4] = g[K+1][7] = E++;
                g[K][5] = g[K+S][2] = E++;
                g[K][6] = g[K+S][1] = E++;
            }
        }
        
        K++;
    }
    
    EDGES = E;
}

map<string,ull> P_old, P_new;

void save_path (const vector<pii> &v, ull count)
{
    string S;
    for (int i=0; i<(int)v.size(); i++) {
        S += to_string(v[i].fs);
        S += "#";
        S += to_string(v[i].sc);
        S += "#";
    }
    
    P_new[S] = (P_new[S] + count) % Q;
}

void merge_corner_path (vector<pii> v, pii p, ull count) // v in the form <start_edge, end_edge>
{
    for (int i=0; i<(int)v.size(); i++) {
        if (v[i].sc == p.fs) {
            v[i].sc = p.sc;
            break;
        }
        if (v[i].fs == p.sc) {
            v[i].fs = p.fs;
            break;
        }
    }
    
    sort(v.begin(), v.end());
    save_path(v,count);
}

void merge_side_path (vector<pii> v, int K, ull count)
{
    for (int cf=0; cf<2; cf++) {
        
        vector<pii> p;
        p.push_back(mp(g[K][1], g[K][CFed[cf][0]]));
        p.push_back(mp(g[K][3], g[K][CFed[cf][1]]));
        
        int s[2] = {-1,-1}, e[2] = {-1,-1};
        
        bool ok = true;
        vector<pii> w = v;
        for (int j=0; j<2; j++) {
            
            for (int i=0; i<(int)w.size(); i++) {
                if (w[i].fs == p[j].sc) e[j] = i;
                if (w[i].sc == p[j].fs) s[j] = i;
            }
            if (s[j] == e[j] && s[j] != -1) { ok = false; break; }
            
            if (s[j] == -1 && e[j] == -1) w.push_back(p[j]);
            else if (s[j] == -1) w[e[j]].fs = p[j].fs;
            else if (e[j] == -1) w[s[j]].sc = p[j].sc;
            else {
                w[s[j]].sc = w[e[j]].sc;
                w.erase(w.begin()+e[j]);
            }
        }
        
        if (!ok) continue;
        
        sort(w.begin(),w.end());
        save_path(w,count);
    }
}

void merge_inner_path (vector<pii> v, int K, ull count)
{
    for (int cf=0; cf<14; cf++) {
        
        vector<pii> p;
        p.push_back(mp(g[K][1], g[K][CFin[cf][0]]));
        p.push_back(mp(g[K][3], g[K][CFin[cf][1]]));
        p.push_back(mp(g[K][5], g[K][CFin[cf][2]]));
        p.push_back(mp(g[K][7], g[K][CFin[cf][3]]));
        
        int s[4] = {-1,-1,-1,-1}, e[4] = {-1,-1,-1,-1};
//        for (int i=0; i<(int)v.size(); i++) {
//
//            for (int j=0; j<4; j++) {
//                if (v[i].fs == p[j].sc) e[j] = i;
//                if (v[i].sc == p[j].fs) s[j] = i;
//            }
//        }
        
        bool ok = true;
        vector<pii> w = v;
        for (int j=0; j<4; j++) {
            
            for (int i=0; i<(int)w.size(); i++) {
                if (w[i].fs == p[j].sc) e[j] = i;
                if (w[i].sc == p[j].fs) s[j] = i;
            }
            if (s[j] == e[j] && s[j] != -1) { ok = false; break; }
            
            if (s[j] == -1 && e[j] == -1) w.push_back(p[j]);
            else if (s[j] == -1) w[e[j]].fs = p[j].fs;
            else if (e[j] == -1) w[s[j]].sc = p[j].sc;
            else {
                w[s[j]].sc = w[e[j]].sc;
                w.erase(w.begin()+e[j]);
            }
        }
        
        if (!ok) continue;
        
        sort(w.begin(),w.end());
//        if (w.size() == 3 && w[2].sc == 7) {
//            cout << "WHAT?\n";
//        }
        save_path(w,count);
    }
}

int main() {
    cout.precision(14);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    cin >> N >> M; // N >= M for now
    enumerate_edges();
    
    int K = 0;
    for (int i=0; i<=N; i++) for (int j=0; j<=M; j++) {

        cout << K << ": ";
        for (int k=1; k<(int)g[K].size(); k++) cout << g[K][k] << " ";
        cout << endl;
        K++;
    }
    
    K = 0;
    for (int i=0; i<=N; i++) for (int j=0; j<=M; j++) {
        
        if (K == 0) {
            P_old["0#1#"] = 1;
            K++;
            continue;
        }
        if (K == (N+1)*(M+1)-1) {
            string S = to_string(g[K][2]);
            S += "#";
            S += to_string(g[K][1]);
            S += "#";
            ans = P_old[S];
            
            cout << "Iteration #" << K << endl;
            for (map<string,ull>::iterator it = P_old.begin(); it != P_old.end(); it++) {
                cout << (*it).fs << " " << (*it).sc << endl;
            }
            continue;
        }
        
        cout << "Iteration #" << K << endl;
        for (map<string,ull>::iterator it = P_old.begin(); it != P_old.end(); it++) {
            
            ull count = (*it).sc;
            vector<string> vs = parse_by_symbol((*it).fs, '#');
            vector<pii> v;
            for (int i=0; i<(int)vs.size(); i+=2) {
                v.push_back(mp(atoi(vs[i].c_str()), atoi(vs[i+1].c_str())));
            }
            
            //cout << (*it).fs << " " << count << endl;
            
            if ((i == 0 || i == N) && (j == 0 || j == M)) merge_corner_path(v, mp(g[K][1],g[K][2]), count);
            else if (i == 0 || i == N || j == 0 || j == M) merge_side_path(v, K, count);
            else merge_inner_path(v, K, count);
        }
        
        K++;
        P_old.swap(P_new);
        P_new.clear();
    }
    
    cout << endl << ans << endl;
    
    return 0;
}
