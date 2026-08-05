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

vpll pairs (ll n)
{
    bool is_divisible_by_two = (n%2 == 0);
    if (is_divisible_by_two) n /= 2;
    vpull factors;
    for (ll i=1; primes[i]*1ll*primes[i]<=n; i++) {
        if (n % primes[i] == 0) {
            factors.push_back(mp(primes[i],0));
            while (n % primes[i] == 0) {
                n /= primes[i];
                factors.back().sc++;
            }
            if (n > 1 && factors.size() == 2) return {};
        }
    }
    if (n > 1) factors.push_back(mp(n,1));
    vpll a;
    if (factors.size() == 1) {
        // 1, p, p^2, p^3, ... , p^k
        ll p = factors[0].fs, k = factors[0].sc;
        ll low = 1, high = powmod(p,k);
        while (low <= high) {
            if (is_divisible_by_two) {
                a.push_back(mp(2*low,high));
                if (a.back().fs > a.back().sc) swap(a.back().fs,a.back().sc);
                if (low != high) a.push_back(mp(low,2*high));
            } else {
                a.push_back(mp(low,high));
            }
            low *= p;
            high /= p;
        }
        return a;
    }
    ll p = powmod(factors[0].fs, factors[0].sc);
    ll q = powmod(factors[1].fs, factors[1].sc);
    if (p > q) swap(p,q);
    if (is_divisible_by_two) {
        a.push_back(mp(2*p,q));
        if (a[0].fs > a[0].sc) swap(a[0].fs,a[0].sc);
        a.push_back(mp(p,2*q));
    } else {
        a.push_back(mp(p,q));
    }
    return a;
}

struct Node {
    pii p;
    vint childs; // ids
    Node (pii _p) { p = _p; }
};

vector<Node> G;
map<pii,int> M;
const int N = 1000000;

void build_graph (pii P)
{
    G.clear();
    G.push_back(Node(P));
    for (int i=0; i<(int)G.size(); i++) {
        pii p = G[i].p;
        map<pii,int>::iterator it = M.find(p);
        if (it != M.end() && it->sc <= N) {
            G.push_back(Node(mp(p.fs,it->sc)));
            G.push_back(Node(mp(p.sc,it->sc)));
            G[i].childs.push_back((int)G.size()-2);
            G[i].childs.push_back((int)G.size()-1);
        }
    }
}

pll dfs (int v = 0) // < bracelets count, bracelets sum >
{
    pll p = mp(0,0);
    if (v) { // means we count root alone too
        p.fs++;
        p.sc += G[v].p.sc;
    }
    
    if (G[v].childs.empty()) return p;
    
    pll a = dfs(G[v].childs[0]);
    pll b = dfs(G[v].childs[1]);
    
    p.fs += a.fs*b.fs;
    p.sc += a.fs*b.sc + b.fs*a.sc;
    
    if (v == 0) { // initial vertex, restore answer
        p.sc /= 2;
        p.sc += (a.fs*b.fs)*(G[v].p.fs + G[v].p.sc);
    }
    else {
        p.sc += (a.fs*b.fs)*G[v].p.sc;
    }
    
    return p;
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;

    Eratosthenes_sieve(2*N,true);
    
    // reading from pre-generated input.txt file, faster (~0.09 seconds)
//    for (int i=0; i<3986; i++) {
//        int a,b,c;
//        char z;
//        cin >> a >> z >> b >> c;
//        M[mp(b,c)] = a;
//    }
    
    // this code generates input.txt file, works for ~23.32 seconds, but this is clean solution
    vvll GG(N+1);
    for (ll n=2; n<=N; n++) {
        vpll a = pairs(n*n+1);
        for (int i=0; i<(int)a.size(); i++) {
            if (a[i].sc <= N) GG[a[i].sc].push_back(a[i].fs);
        }
    }
    for (int i=1; i<=N; i++) {
        if (GG[i].size() < 2) continue;
        cout << i << ": ";
        for (int j=0; j<(int)GG[i].size(); j++) cout << GG[i][j] << " ";
        cout << endl;
        M[mp(GG[i][0],GG[i][1])] = i;
    }
    
    for (map<pii,int>::iterator it=M.begin(); it!=M.end(); it++) {
        if (it->sc > N) continue;
        build_graph(it->fs);
        pll p = dfs();
        cout << it->fs << " " << p << endl;
        ans += p.sc;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
