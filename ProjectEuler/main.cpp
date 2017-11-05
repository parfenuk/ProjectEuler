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
    ull x = (ull)sqrt((dd)(n-0.001));
    while (x*x < n) x++;
    if (x*x == n) return x;
    return 0;
}
// n = 153 -> (1,5,3)
vector<int> digits (ull n, int base = 10)
{
    vector<int> a;
    while (n) {
        
        a.push_back(n%base);
        n /= base;
    }
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
    
    for (int i=(int)v.size()-1; i>=0; i--) {
        v.push_back(n % (matches[i]+1));
        n /= (matches[i] + 1);
    }
    
    reverse(v.begin(), v.end());
    return v;
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

vector<ull> Blub_blub_shum_generator (int n)
{
    vector<ull> v;
    ull a = 14025256;
    
    for (int i=0; i<n; i++) {
        v.push_back(a);
        a = a*a % 20300713;
    }
    
    return v;
}

pdd NOT_FOUND_D = mp(-100000,-100000);

pdd try_solve (pdd p, pdd q)
{
    dd x1 = p.fs, y1 = p.sc, x2 = q.fs, y2 = q.sc;
    
    dd A = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
    dd B = (x2-x1)/2 + 2*x1*x2 - 2*x2*x2 - y1 + y2 + 2*y1*y2 - 2*y2*y2;
    dd C = x2*x2 + y2*y2 - y2 - x2/2 + 0.25;
    
    dd D = B*B - A*C*4;
    //if (fabs(D) < EPS) D = 0;
    if (D < 0) return NOT_FOUND_D;
    
    dd t1 = (sqrt(D) - B)/(2*A), t2 = (sqrt(D) + B)/(2*A);
    
    if (t1 > 0 && t1 < 1) return mp(x1*t1 + x2*(1-t1), y1*t1 + y2*(1-t1));
    if (t2 > 0 && t2 < 1) return mp(x1*t2 + x2*(1-t2), y1*t2 + y2*(1-t2));
    return NOT_FOUND_D;
}

dd final_square (const vector<pdd> pnts)
{
    dd s = 0;
    for (int i=1; i<(int)pnts.size(); i++) s += (pnts[i].sc + pnts[i-1].sc)*(pnts[i].fs - pnts[i-1].fs)/2;
    return s;
}

int main() {
    cout.precision(15);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    vector<dd> v(2);
    
    dd a = 1.0;
    for (int steps=1; steps<=27; steps++) {
        
        cout << steps << endl;
        a /= 2;
        vector<dd> w;
        for (int i=1; i<(int)v.size(); i++) {
            w.push_back(v[i-1]);
            w.push_back(a + (v[i-1] + v[i])/2);
        }
        w.push_back(v.back());
        v.swap(w);
    }
    
    vector<pdd> pnts;
    dd s = 0;
    for (int i=0; i<(int)v.size(); i++) {
        pnts.push_back(mp(s,v[i]));
        s += a;
    }
    
    //for (int i=0; i<(int)pnts.size(); i++) cout << fixed << pnts[i].fs << " " << pnts[i].sc << endl;
    
    vector<pdd> final_pnts;
    
    for (int i=1; i<(int)pnts.size(); i++) {
        
        if (final_pnts.empty()) {
            pdd p = try_solve(pnts[i-1], pnts[i]);
            if (p != NOT_FOUND_D) {
                final_pnts.push_back(p);
                cout << i << endl;
            }
        }
        else {
            if (pnts[i].fs > 0.5) break;
            final_pnts.push_back(pnts[i]);
        }
    }
    
    cout << fixed << final_pnts[0].fs << endl;
    cout << fixed << final_square(final_pnts) << endl;
    
    cout << endl << ans << endl;
}
