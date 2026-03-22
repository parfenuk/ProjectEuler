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

const int Q = 1000000007;

ll G1 (ll N, int H)
{
    ll res = 0;
    
    Matrix::matrix A(2*H);
    A.make_zero();
    A.Q = Q;
    
    // even blocks dp
    for (int h=1; h<=H; h++) {
        for (int k=h; k<=H; k++) A[h-1][k-1] = 1;
        for (int k=1; h-2*k>0; k++) A[h-1][h-2*k-1] = 1;
        for (int k=1; h+1-2*k>0; k++) A[h-1][H+h-2*k] = 1;
    }
    // odd blocks dp
    for (int h=1; h<=H; h++) {
        for (int k=h; k<=H; k++) A[H+h-1][H+k-1] = 1;
        for (int k=1; h-2*k>0; k++) A[H+h-1][H+h-2*k-1] = 1;
        for (int k=1; h+1-2*k>0; k++) A[H+h-1][h-2*k] = 1;
    }
    
    Matrix::matrix B = A^(N-1);
    for (int n=0; n<H; n++) { // even blocks cells
        for (int i=0; i<2*H; i++) {
            int h = i%H + 1;
            bool even = i < H;
            if (h%2 == !even) {
                res += B[n][i]; if (res >= Q) res -= Q;
            }
        }
    }
    
    return res;
}

// F1(4,2) = 10 - confirmed
// F1(13,10) = 3729050610636 % Q = 50584533 - confirmed
// F1(10,13) = 37959702514 % Q = 959702255 - confirmed
// F1(100,100) = 841913936 - confirmed
// F1(10^12,100) = 364553235 (7.705s)
ll F1 (ll N, int H)
{
    ll res = G1(N,H) - G1(N,H-1);
    return res >= 0 ? res : res + Q;
}

ll G2 (int N, int H)
{
    ll res = 0;
    
    vector<vector<ll>> dp_even(N+1);
    vector<vector<ll>> dp_odd(N+1);
    vector<vector<ll>> sum_dp_even(N+1); // dp_even[n][1] + dp_even[n][2] + ... + dp_even[n][h]
    vector<vector<ll>> sum_dp_odd(N+1); // dp_odd[n][1] + dp_odd[n][2] + ... + dp_odd[n][h]
    vector<vector<ll>> sum_dp_even_only_for_even(N+1); // dp_even[n][2,4,...,h-h%2]
    vector<vector<ll>> sum_dp_odd_only_for_even(N+1); // dp_odd[n][2,4,...,h-h%2]
    for (int i=1; i<=N; i++) {
        dp_even[i] = dp_odd[i] = sum_dp_even[i] = sum_dp_odd[i] = vector<ll>(H+1);
        sum_dp_even_only_for_even[i] = sum_dp_odd_only_for_even[i] = vector<ll>(H+1);
    }
    
    for (int h=1; h<=H; h++) {
        if (h % 2 == 0) dp_even[1][h] = 1;
        else dp_odd[1][h] = 1;
        
        sum_dp_even[1][h] = sum_dp_even[1][h-1] + dp_even[1][h];
        sum_dp_odd[1][h] = sum_dp_odd[1][h-1] + dp_odd[1][h];
        sum_dp_even_only_for_even[1][h] = h%2 == 0
        ? sum_dp_even_only_for_even[1][h-2] + dp_even[1][h]
        : sum_dp_even_only_for_even[1][h-1];
        sum_dp_odd_only_for_even[1][h] = h%2 == 0
        ? sum_dp_odd_only_for_even[1][h-2] + dp_odd[1][h]
        : sum_dp_odd_only_for_even[1][h-1];
    }
    
    for (int n=2; n<=N; n++) {
        for (int h=1; h<=H; h++) {
            if (h % 2 == 0) {
                dp_even[n][h] = sum_dp_even[n-1][H] - sum_dp_even[n-1][h-1];
                dp_even[n][h] += sum_dp_even_only_for_even[n-1][h-2];
                dp_even[n][h] += sum_dp_odd[n-1][h-1] - sum_dp_odd_only_for_even[n-1][h-2];
                
                dp_odd[n][h] = sum_dp_odd[n-1][H] - sum_dp_odd[n-1][h-1];
                dp_odd[n][h] += sum_dp_odd_only_for_even[n-1][h-2];
                dp_odd[n][h] += sum_dp_even[n-1][h-1] - sum_dp_even_only_for_even[n-1][h-2];
            } else {
                dp_even[n][h] = sum_dp_even[n-1][H] - sum_dp_even[n-1][h-1];
                if (h > 1) dp_even[n][h] += sum_dp_even[n-1][h-2] - sum_dp_even_only_for_even[n-1][h-3];
                dp_even[n][h] += sum_dp_odd_only_for_even[n-1][h-1];
                
                dp_odd[n][h] = sum_dp_odd[n-1][H] - sum_dp_odd[n-1][h-1];
                if (h > 1) dp_odd[n][h] += sum_dp_odd[n-1][h-2] - sum_dp_odd_only_for_even[n-1][h-3];
                dp_odd[n][h] += sum_dp_even_only_for_even[n-1][h-1];
            }
            
            dp_even[n][h] %= Q; if (dp_even[n][h] < 0) dp_even[n][h] += Q;
            dp_odd[n][h] %= Q; if (dp_odd[n][h] < 0) dp_odd[n][h] += Q;
            
            sum_dp_even[n][h] = (sum_dp_even[n][h-1] + dp_even[n][h]) % Q;
            sum_dp_odd[n][h] = (sum_dp_odd[n][h-1] + dp_odd[n][h]) % Q;
            sum_dp_even_only_for_even[n][h] = h%2 == 0
            ? (sum_dp_even_only_for_even[n][h-2] + dp_even[n][h]) % Q
            : sum_dp_even_only_for_even[n][h-1];
            sum_dp_odd_only_for_even[n][h] = h%2 == 0
            ? (sum_dp_odd_only_for_even[n][h-2] + dp_odd[n][h]) % Q
            : sum_dp_odd_only_for_even[n][h-1];
        }
    }
    
    for (int h=1; h<=H; h++) {
        res += dp_even[N][h];
        if (res >= Q) res -= Q;
    }
    
    return res;
}

// F2(4,2) = 10 - confirmed
// F2(13,10) = 3729050610636 % Q = 50584533 - confirmed
// F2(10,13) = 37959702514 % Q = 959702255 - confirmed
// F2(100,100) = 841913936 - confirmed
// F2(10000,10000) = 749784357 (36.993s)
ll F2 (int N, int H)
{
    ll res = G2(N,H) - G2(N,H-1);
    return res >= 0 ? res : res + Q;
}

// F3(4,2) = 10 - confirmed
// F3(13,10) = 3729050610636 % Q = 50584533 - confirmed
// F3(10,13) = 37959702514 % Q = 959702255 - confirmed
// F3(100,100) = 841913936 - confirmed
// F3(100,10^12) = 635147632 (0.397s)
ll F3 (int N, ll H)
{
    ll res = 0;
    
    const int CNT = N; // it is enough, initially I thought we need greater polynom degree :)
    
    vvll C1, C2; // even and odd coefficients correspondingly, CNTx(CNT+1)
    vll G(2*CNT+1), F(2*CNT+1);
    for (int h=1; h<=2*CNT; h++) {
        G[h] = G2(N,h);
        F[h] = G[h] - G[h-1];
        if (F[h] < 0) F[h] += Q;
    }
    
    for (int h=1; h<=2*CNT; h++) {
        ll s = 1;
        vll coeffs;
        for (int deg=0; deg<CNT; deg++) {
            coeffs.push_back(s);
            s = s*h % Q;
        }
        coeffs.push_back(F[h]); // free column
        if (h % 2) C1.push_back(coeffs);
        else C2.push_back(coeffs);
    }
    
    vll sol1, sol2;
    Gauss(C1,sol1,Q);
    Gauss(C2,sol2,Q);
    
    ll K = H % Q;
    ll s = 1;
    for (int deg=0; deg<CNT; deg++) {
        if (H % 2) res += sol1[deg]*s;
        else res += sol2[deg]*s;
        res %= Q;
        s = s*K % Q;
    }
    
    return res;
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
    
    ans = ( F1(powmod(10,12),100) + F2(10000,10000) + F3(100,powmod(10,12)) ) % Q;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
