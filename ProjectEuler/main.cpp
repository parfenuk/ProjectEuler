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
const ull FACT[12] = {1,1,2,6,24,120,720,5040,40320,362880,3628800,39916800};

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
    
    for (ll i=2; i*i<=n; i++) {
        
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

struct Partition {
    int id;
    vector<int> v; // sorted in decreasing order
    vector<pii> p; // p[i] = positions of first and second number i in v;
    void fill_p() {
        p = vector<pii>(v[0]+1,mp(0,0));
        for (int i=0; i<(int)v.size(); i++) {
            int t = v[i];
            if (p[t].fs == 0) p[t].fs = i+1;
            else if (p[t].sc == 0) p[t].sc = i+1;
        }
    }
    void show() { cout << id << ": "; ::show(v); }
};

bool operator< (Partition a, Partition b)
{
    if (a.v.size() < b.v.size()) return true;
    if (a.v.size() > b.v.size()) return false;
    
    for (int i=0; i<(int)a.v.size(); i++) {
        if (a.v[i] < b.v[i]) return true;
        if (a.v[i] > b.v[i]) return false;
    }
    
    return false;
}

bool operator== (Partition a, Partition b)
{
    if (a.v.size() != b.v.size()) return false;
    
    for (int i=0; i<(int)a.v.size(); i++) {
        if (a.v[i] != b.v[i]) return false;
    }
    
    return false;
}

vector<Partition> P[41];

int find_partition_id (int N, const vector<int> &v)
{
    Partition p;
    p.v = v;
    
    vector<Partition>::iterator it = lower_bound(P[N].begin(), P[N].end(), p);
    
    return (*it).id;
}

pii new_indeces (Partition &a, int left_number, int right_number)
{
    pii p;
    p.fs = a.p[left_number].fs;
    p.sc = a.p[right_number].fs;
    if (p.fs && p.sc == p.fs) p.sc = a.p[right_number].sc;
    return p;
}

int main() {
    cout.precision(12);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    for (int n=1; n<=40; n++) {
        
        int k = 0;
        vector<vector<int>> a = sum_partitions(n);
        
        for (int i=0; i<(int)a.size(); i++) {
            if (a[i].size()+n-1 <= 40) {
                k++;
                Partition p;
                p.v = a[i];
                p.fill_p();
                P[n].push_back(p);
            }
        }
        cout << n << ": " << a.size() << " " << k << endl;
        
        sort(P[n].begin(), P[n].end());
        for (int i=0; i<k; i++) P[n][i].id = i;
    }
    
    for (int i=0; i<(int)P[5].size(); i++) P[5][i].show();
    vector<int> test_vector;
    test_vector.push_back(2);
    test_vector.push_back(2);
    test_vector.push_back(1);
    int test_value = find_partition_id(5, test_vector);
    cout << test_value << endl;
    
    int N = 40;
    dd *dp[4242];
    for (int i=0; i<4242; i++) dp[i] = new dd [160000];
    dd *new_dp[4242];
    for (int i=0; i<4242; i++) new_dp[i] = new dd [160000];
    
    // fill dp for N-1
    for (int i=0; i<(int)P[N-1].size(); i++) for (int j=0; j<160000; j++) dp[i][j] = 0;
    for (int i=0; i<N; i++) {
        if (i == 0 || i == N-1) {
            vector<int> a;
            a.push_back(N-1);
            vector<int> config(4); // max_blocks, cur_blocks, index_left, index_right
            config[2] = 1;
            dp[0][from_digits(config,20)] += 1.0/N;
        }
        else {
            vector<int> a;
            a.push_back(i);
            a.push_back(N-i-1);
            if (a[0] < a[1]) swap(a[0],a[1]);
            vector<int> config(4); // max_blocks, cur_blocks, index_left, index_right
            config[2] = 1;
            config[3] = 2;
            dp[find_partition_id(N-1,a)][from_digits(config,20)] += 1.0/N;
        }
    }
    
    for (int i=0; i<42; i++) for (int j=0; j<160000; j++) {
        if (dp[i][j]) cout << fixed << dp[i][j] << endl;
    }
    
    // fill next dp for n basing on dp for n+1
    for (int n=N-2; n>=1; n--) {
        cout << "n = " << n << endl;
        for (int i=0; i<(int)P[n].size(); i++) for (int j=0; j<160000; j++) new_dp[i][j] = 0;
        
        for (int x=0; x<(int)P[n+1].size(); x++) for (int y=0; y<160000; y++) {
            if (dp[x][y] == 0) continue;
            
            vector<int> v = P[n+1][x].v; // sum == n+1
            vector<int> config = digits(y,20,4);
            //int max_blocks = config[0];
            //int cur_blocks = config[1];
            int index_left = config[2];
            int index_right = config[3];
            int left_number = index_left ? v[index_left-1] : 0;
            int right_number = index_right ? v[index_right-1] : 0;
            
            vector<int> new_config(4);
            
            for (int i=0; i<(int)v.size(); i++) {
                
                vector<int> a = v;
                if (i+1 == index_left) {
                    if (v[i] == 1) {
                        dd prob = dp[x][y]/(n+1);
                        new_config = config;
                        a.pop_back();
                        Partition p = P[n][find_partition_id(n,a)];
                        pii q = new_indeces(p, 0, right_number);
                        new_config[2] = q.fs;
                        new_config[3] = q.sc;
                        new_dp[p.id][from_digits(new_config,20)] += prob;
                    }
                    else {
                        // if close to next block
                        dd prob = dp[x][y]/(n+1);
                        new_config = config;
                        a[i]--; sort(a.rbegin(), a.rend());
                        Partition p = P[n][find_partition_id(n,a)];
                        pii q = new_indeces(p, left_number-1, right_number);
                        new_config[2] = q.fs;
                        new_config[3] = q.sc;
                        new_dp[p.id][from_digits(new_config,20)] += prob;
                        
                        // if close to border
                        new_config = config;
                        new_config[1]++;
                        if (new_config[1] > new_config[0]) new_config[0]++;
                        q = new_indeces(p, 0, right_number);
                        new_config[2] = q.fs;
                        new_config[3] = q.sc;
                        new_dp[p.id][from_digits(new_config,20)] += prob;
                        
                        // if middle
                        for (int j=1; j<v[i]-1; j++) {
                            a = v;
                            a[i] = j;
                            a.push_back(v[i]-j-1);
                            sort(a.rbegin(), a.rend());
                            new_config = config;
                            new_config[1]++;
                            if (new_config[1] > new_config[0]) new_config[0]++;
                            
                            Partition p = P[n][find_partition_id(n,a)];
                            pii q = new_indeces(p, j, right_number);
                            new_config[2] = q.fs;
                            new_config[3] = q.sc;
                            
                            new_dp[p.id][from_digits(new_config,20)] += prob;
                        }
                    }
                }
                else if (i+1 == index_right) {
                    if (v[i] == 1) {
                        dd prob = dp[x][y]/(n+1);
                        new_config = config;
                        a.pop_back();
                        Partition p = P[n][find_partition_id(n,a)];
                        pii q = new_indeces(p, left_number, 0);
                        new_config[2] = q.fs;
                        new_config[3] = q.sc;
                        new_dp[p.id][from_digits(new_config,20)] += prob;
                    }
                    else {
                        // if close to next block
                        dd prob = dp[x][y]/(n+1);
                        new_config = config;
                        a[i]--; sort(a.rbegin(), a.rend());
                        Partition p = P[n][find_partition_id(n,a)];
                        pii q = new_indeces(p, left_number, right_number-1);
                        new_config[2] = q.fs;
                        new_config[3] = q.sc;
                        new_dp[p.id][from_digits(new_config,20)] += prob;
                        
                        // if close to border
                        new_config = config;
                        new_config[1]++;
                        if (new_config[1] > new_config[0]) new_config[0]++;
                        q = new_indeces(p, left_number, 0);
                        new_config[2] = q.fs;
                        new_config[3] = q.sc;
                        new_dp[p.id][from_digits(new_config,20)] += prob;
                        
                        // if middle
                        for (int j=1; j<v[i]-1; j++) {
                            a = v;
                            a[i] = j;
                            a.push_back(v[i]-j-1);
                            sort(a.rbegin(), a.rend());
                            new_config = config;
                            new_config[1]++;
                            if (new_config[1] > new_config[0]) new_config[0]++;
                            
                            Partition p = P[n][find_partition_id(n,a)];
                            pii q = new_indeces(p, left_number, j);
                            new_config[2] = q.fs;
                            new_config[3] = q.sc;
                            
                            new_dp[p.id][from_digits(new_config,20)] += prob;
                        }
                    }
                }
                else {
                    
                    if (v[i] == 1) {
                        dd prob = dp[x][y]/(n+1);
                        new_config = config;
                        new_config[1]--;
                        a.pop_back();
                        new_dp[find_partition_id(n,a)][from_digits(new_config,20)] += prob;
                    }
                    else {
                        // border case
                        dd prob = 2*dp[x][y]/(n+1);
                        a[i]--; sort(a.rbegin(), a.rend());
                        new_config = config;
                        Partition p = P[n][find_partition_id(n,a)];
                        pii q = new_indeces(p, left_number, right_number);
                        new_config[2] = q.fs;
                        new_config[3] = q.sc;
                        new_dp[p.id][from_digits(new_config,20)] += prob;
                        
                        // middle case
                        prob = dp[x][y]/(n+1);
                        for (int j=1; j<v[i]-1; j++) {
                            a = v;
                            a[i] = j;
                            a.push_back(v[i]-j-1);
                            sort(a.rbegin(), a.rend());
                            new_config = config;
                            new_config[1]++;
                            if (new_config[1] > new_config[0]) new_config[0]++;
                            Partition p = P[n][find_partition_id(n,a)];
                            
                            pii q = new_indeces(p, left_number, right_number);
                            new_config[2] = q.fs;
                            new_config[3] = q.sc;
                            
                            new_dp[p.id][from_digits(new_config,20)] += prob;
                        }
                    }
                }
            }
        }
        
        for (int i=0; i<(int)P[n].size(); i++) for (int j=0; j<160000; j++) dp[i][j] = new_dp[i][j];
    }
    
    dd total[21];
    for (int i=0; i<21; i++) total[i] = 0;
    for (int j=0; j<160000; j++) {
        vector<int> config = digits(j,20,4);
        total[config[0]+1] += dp[0][j];
    }
    
    for (int i=1; i<21; i++) cout << i << " " << fixed << total[i] << endl;
    dd res = 0;
    for (int i=1; i<21; i++) res += i*total[i];
    cout << endl << fixed << res << endl;
    
    cout << endl << ans << endl;
    
    return 0;
}
