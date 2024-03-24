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
using namespace Fractions;

// K-Optimal: DP for different dens
// V0-Optimal: DP with decimal numbers

const int MAX_DEN = 2632; // 10/47 requires such interim denominator
vector<ftype> rec (ftype M, ftype N, int K = 13, ll L = 376, int from = 1)
{
    if (N > MAX_DEN) return {};
    if (M == 1 && N > L) return {};
    if (M == 1 && N >= from) return { N };
    if (K == 1) return {};
    
    fraction F(M,N);
    if (F > fraction(K,from)) return {}; // 1/from + 1/(from+1) + ... + 1/(from+K-1) < K/from
    if (F < fraction(1,L)) return {};
    
    for (int i=from; i<=L; i++) {
        fraction f = F - fraction(1,i);
        if (f.num == 0) return { i };
        if (f.num < 0) continue;
        vector<ftype> a = rec(f.num, f.den, K-1, L, i+1);
        if (!a.empty()) { a.push_back(i); return a; }
    }
    return {};
}

namespace DPFractions {

// M/N = (k1+k2+...+ks)/D, s <= K, D is multiple of N
vector<ull> dp_fractions (ull M, ull N, ull D, int maxK) // -> minimize K
{
    ull coef = D/N;
    vull d = Divisors_product({ N, coef });
    ull target = M*coef;
    
    // dp[count][n] = last_number where 'count' summands equals to 'n'
    vector<vector<int>> dp(maxK+1);
    for (int i=0; i<=maxK; i++) dp[i] = vector<int>(target+1, -1);
    dp[0][0] = 0;
    
    for (int cnt=1; cnt<=maxK; cnt++) { // count
        
        for (ll m=target; m>=0; m--) { // current number
            if (dp[cnt-1][m] == -1) continue;
            
            for (int i=0; i<(int)d.size(); i++) {
                if (d[i] <= dp[cnt-1][m]) continue;
                ull n = m + d[i];
                if (n > target) break;
                
                // determine how number n was obtained before if it was
                int num = -1;
                for (int j=maxK; j>=0; j--) {
                    if (dp[j][n] != -1) { num = dp[j][n]; break; }
                }
                
                if (num == -1 || num > d[i]) dp[cnt][n] = (int)d[i];
            }
        }
        
        if (dp[cnt][target] != -1) { // we got an answer!
            vull ans;
            ull t = target;
            for (int i=cnt; i>0; i--) {
                ans.push_back(D/dp[i][t]);
                t -= dp[i][t];
            }
            
            return ans;
        }
    }
    
    return {};
}

vector<ull> get_optimal_count (ull M, ull N)
{
    int K = 13;
    vull ret;
    int cur_coef = 0;
    for (int coef=1; coef<=1200; coef++) {
        vull a = dp_fractions(M, N, N*coef, K);
        if (a.empty()) continue;
        if (a.size() == K && (ret.empty() || a.back() < ret.back())) {
            ret = a;
            cur_coef = coef;
        }
        if (a.size() < K) { 
            ret = a;
            K = (int)a.size();
            cur_coef = coef;
        }
        if (K == 2) break;
    }
    cout << cur_coef << endl;
    return ret;
}

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
    
    ofstream out("interim_table.csv");
    
    for (int M=2; M<=50; M++) for (int N=1; N<=50; N++) {
        if (M > 3*N) continue;
        if (GCD(M,N) != 1) continue;
        
        cout << M << " " << N << endl;
        out << M << "/" << N << ";";
        
        vector<ftype> a = rec(M,N);
        reverse(a.begin(), a.end());
        out << a.size() << ": ";
        for (int i=0; i<(int)a.size(); i++) {
            out << a[i];
            if (i+1 != (int)a.size()) out << " ";
        }
        out << ";";
        
        vull ak = DPFractions::get_optimal_count(M,N);
        out << ak.size() << " (";
        for (int i=0; i<(int)ak.size(); i++) {
            out << ak[i];
            if (i+1 == (int)ak.size()) out << ")";
            else out << " ";
        }
        out << "\n";
        
        // original code that generates solutions for all tests EXTREMELY fast!!!
        // solutions are not guaranteed to be K-optimal or V0-optimal
//        clock_t Total_Time = clock();
//        vector<ftype> a = rec(M,N);
//        cout << M << "/" << N << ": size = " << a.size() << " in ";
//        Total_Time = clock() - Total_Time;
//        cout << fixed << ((float)Total_Time)/CLOCKS_PER_SEC << endl;
//        for (int i=0; i<(int)a.size(); i++) cout << a[i] << " "; cout << endl;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
