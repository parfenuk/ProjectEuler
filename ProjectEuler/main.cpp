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

const int Q1 = 41*41;
const int Q2 = 271*271;
// so, Q = Q1*Q2 = 123454321
const ll N = 100000000000000;

// f[1] = f, f[i] = k*f[i-1]+b, returns (f[1] + f[2] + ... + f[n]) mod q
ll group_count (ll n, ll f, ll k, ll b, ll q)
{
    ll g = powmod(k,n,q) - 1; if (g < 0) g += q;
    ll kinv = inverse(k-1, q, NumberUtils::integer_sqrt(q));
    g = g*kinv % q;
    ll F = f*g % q;
    ll B = b*kinv % q;
    B = B*(g-n%q) % q; if (B < 0) B += q;
    
    return (F+B) % q;
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
    
    vector<int> A = {1,2,3,4,3,2};
    for (int i=0; i<210; i++) for (int j=0; j<6; j++) A.push_back(A[j]);
    
    vector<string> w(2);
    int s = 0, cur_sum_needed = 1;
    for (int i=0; w.size() < 80; i++) {
        s += A[i]; w.back().push_back(A[i] + '0');
        if (s == cur_sum_needed) {
            s = 0;
            cur_sum_needed++;
            w.push_back("");
        }
    }
    
    ll ans1 = 0, ans2 = 0;
    for (int j=1; j<=15; j++) {
//        cout << "j = " << j << endl;
//        for (int i=j; i<(int)w.size(); i+=15) cout << w[i] << endl;
        ll f1 = atoi(w[j].c_str());
        ll k = 1000000;
        ll b = atol(w[j+15].c_str()) - f1*k;
        ll n = count_divisible_by(15, 0, N-1, j-1);
//        cout << f1 << " " << k << " " << b << " " << n << endl << endl;
        ans1 += group_count(n,f1,k,b,Q1); if (ans1 >= Q1) ans1 -= Q1;
        ans2 += group_count(n,f1,k,b,Q2); if (ans2 >= Q2) ans2 -= Q2;
    }
    
    ans = Chinese_theorem({Q1,Q2},{ans1,ans2});
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
