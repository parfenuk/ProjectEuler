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

const int N = 64000;
int K = 1;
map<pii,int> childs;
vector<int> tree_type(N+1);
vvull numbers_of_type(1000);

pull mid_divisors (ull n)
{
    vull D = Divisors(n);
    int s = (int)D.size();
    if (s % 2 == 0) return mp(D[s/2-1],D[s/2]);
    return mp(D[s/2],D[s/2]);
}

pull mid_divisors_product (vull a)
{
    vull D = Divisors_product(a,true);
    int s = (int)D.size();
    if (s % 2 == 0) return mp(D[s/2-1],D[s/2]);
    return mp(D[s/2],D[s/2]);
}

ull min_n (ull k)
{
    // minimum search constants, obtained by research
    const int CHECK_SMALL = 10;
    const int CHECK_BIG = 264;
    const int CHECK_1_i = 10;
    const int CHECK_1_j = 23;
    const int CHECK_2_i = 125;
    const int CHECK_2_j = 33;
    
    ull n = 1;
    for (ull i=k; i>1; i-=2) n *= i;
    if (n <= N && isPrime[n]) return 2;
    if (k % 2 == 0 && k > 20 && k < 30) return n; // this is indeed true
    pull d = mid_divisors(n);
    ull ret = n;
    if (d.sc <= N) {
        int t1 = tree_type[d.fs], t2 = tree_type[d.sc];
        for (int i=0; i<(int)numbers_of_type[t1].size() && i < CHECK_SMALL; i++) {
            for (int j=0; j<(int)numbers_of_type[t2].size() && j < CHECK_SMALL; j++) {
                ull a = numbers_of_type[t1][i];
                ull b = numbers_of_type[t2][j];
                if (a > b) continue;
                pull d = mid_divisors_product({a,b});
                if (d.fs == a && d.sc == b) {
                    if (a*b < ret) {
                        ret = a*b;
                        break;
                    }
                }
            }
        }
    }
    else {
        pull d1 = mid_divisors(d.fs), d2 = mid_divisors(d.sc);
        int t1 = tree_type[d1.fs], t2 = tree_type[d1.sc];
        vull candidates1, candidates2;
        for (int i=0; i<(int)numbers_of_type[t1].size() && i < CHECK_1_i; i++) {
            for (int j=0; j<(int)numbers_of_type[t2].size() && j < CHECK_1_j; j++) {
                ull a = numbers_of_type[t1][i];
                ull b = numbers_of_type[t2][j];
                if (a > b) continue;
                pull ds = mid_divisors_product({a,b});
                if (ds.fs == a && ds.sc == b) candidates1.push_back(a*b);
            }
        }
        t1 = tree_type[d2.fs]; t2 = tree_type[d2.sc];
        for (int i=0; i<(int)numbers_of_type[t1].size() && i < CHECK_2_i; i++) {
            for (int j=0; j<(int)numbers_of_type[t2].size() && j < CHECK_2_j; j++) {
                ull a = numbers_of_type[t1][i];
                ull b = numbers_of_type[t2][j];
                if (a > b) continue;
                pull ds = mid_divisors_product({a,b});
                if (ds.fs == a && ds.sc == b) candidates2.push_back(a*b);
            }
        }
        /* it works even without sorting, but it's just luck :) */
        sort(candidates1.begin(), candidates1.end());
        sort(candidates2.begin(), candidates2.end());
        for (int i=0; i<(int)candidates1.size() && i < CHECK_BIG; i++) {
            for (int j=0; j<(int)candidates2.size() && j < CHECK_BIG; j++) {
                ull a = candidates1[i];
                ull b = candidates2[j];
                if (a > b) continue;
                pull ds = mid_divisors_product({a,b});
                if (ds.fs == a && ds.sc == b) {
                    if (a*b < ret) {
                        ret = a*b;
                        break;
                    }
                }
            }
        }
    }
    
    return ret;
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
    
    Eratosthenes_sieve(N);
    
    for (ull n=2; n<=N; n++) {
        if (isPrime[n]) tree_type[n] = 0;
        else {
            pull d = mid_divisors(n);
            if (childs[mp(tree_type[d.fs],tree_type[d.sc])] == 0) {
                childs[mp(tree_type[d.fs],tree_type[d.sc])] = K++;
            }
            tree_type[n] = childs[mp(tree_type[d.fs],tree_type[d.sc])];
        }
        numbers_of_type[tree_type[n]].push_back(n);
    }
    
    for (int n=2; n<=31; n++) {
        ull k = min_n(n);
        cout << "M(" << n << ") = " << k << endl;
        ans += k;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
