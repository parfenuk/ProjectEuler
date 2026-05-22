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
#include "Polynom.cpp"
#include "StringUtils.cpp"
#include "Utils.cpp"

using namespace Algebra;
using namespace Containers;

// all arrays of length n where no consecutive terms are equal and each next term is not greater than 'max_previos_term'+1
// 1...COLORS are available colors
vector<pair<vsint,sint>> generate_tuples (int n, const int COLORS) // elem: <array, max_elem>
{
    vector<pair<vsint,sint>> A(1,mp(vsint(1,1),1));
    for (int zzz=2; zzz<=n; zzz++) {
        vector<pair<vsint,sint>> B;
        for (int p=0; p<(int)A.size(); p++) {
            pair<vsint,sint> a = A[p];
            for (int i=1; i<=a.sc+1 && i<=COLORS; i++) {
                if (i == a.fs.back()) continue;
                a.fs.push_back(i);
                sint max = a.sc; if (i > max) max = i;
                B.push_back(mp(a.fs,max));
                a.fs.pop_back();
            }
        }
        A.swap(B);
    }
    return A;
}

const int Q = 1000000007;
vvsint B2; // all bitmasks
vsint C[10][10]; // C[i][j] - numbers with i digits and j 1-bits
vvint forbidden_values;

// n - colors count in 1st column (prev)
// m - colors count in 2nd column (next)
// forbidden_values[i] - which colors in range 1...n are forbidden for new cell 'i' (deprecated, now access in O(1))
// return value: vll[i] - number of ways to fill m-array with 'i' free/new colors
vll transfer_count (int n, int m, int fb_value)
{
    const int P = 1 << n;
    // dp[i][j][k]: i - used numbers, j - free count, k - used mapping mask
    vector<vvll> dp(m+1);
    for (int i=0; i<=m; i++) {
        dp[i] = vvll(m+1);
        for (int j=0; j<=m; j++) dp[i][j] = vll(P);
    }
    
    dp[0][0][0] = 1;
    for (int len=0; len<m; len++) {
        for (int f=0; f<=len; f++) {
            for (int j=0; j<(int)C[n][len-f].size(); j++) {
                int k = C[n][len-f][j];
                if (dp[len][f][k] == 0) continue;
                int p2 = 1;
                for (int i=1; i<=n; i++, p2<<=1) { // try to map each value
                    if (B2[k][i-1] || forbidden_values[len+1][i] == fb_value) continue;
                    int mask = k|p2;
                    dp[len+1][f][mask] += dp[len][f][k];
                    if (dp[len+1][f][mask] >= Q) dp[len+1][f][mask] -= Q;
                }
                // map free value
                dp[len+1][f+1][k] += dp[len][f][k];
                if (dp[len+1][f+1][k] >= Q) dp[len+1][f+1][k] -= Q;
            }
        }
    }
    
    vll F(m+1);
    for (int f=0; f<=m; f++) for (int k=0; k<P; k++) {
        F[f] += dp[m][f][k];
        if (F[f] >= Q) F[f] -= Q;
    }
    return F;
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
    
    vull F = Combinatorics::generate_factorials(100,false,Q);
    vull FI = Combinatorics::generate_factorials(100,true,Q);
    
    const int N = 9, M = 10;
    B2 = Combinatorics::generate_bitmasks(N);
    for (int i=0; i<(int)B2.size(); i++) {
        int cnt = 0, max_one_bit = -1;
        for (int j=0; j<N; j++) {
            cnt += B2[i][j];
            if (B2[i][j]) max_one_bit = j;
        }
        for (int j=max_one_bit+1; j<=N; j++) C[j][cnt].push_back(i);
    }
    forbidden_values = vvint(N+1);
    for (int i=0; i<=N; i++) forbidden_values[i] = vint(N+1);
    
    vector<vvll> B(4140); // N = 9 Bell number
    for (int i=0; i<4140; i++) B[i] = vvll(4140);
    
    vector<pair<vsint,sint>> A = generate_tuples(N,N);
    int K = (int)A.size();
    
    int fb_value = 1;
    for (int i=0; i<K; i++) for (int j=0; j<K; j++) {
        for (int k=0; k<(int)A[j].fs.size(); k++) {
            forbidden_values[A[j].fs[k]][A[i].fs[k]] = fb_value;
        }
        B[i][j] = transfer_count(A[i].sc, A[j].sc, fb_value);
        fb_value++;
    }

    vpll coeffs;
    for (int COLORS = 1; COLORS <= N*M+1; COLORS++) {
        vvll D(K); for (int i=0; i<K; i++) D[i] = vll(K);
        
        for (int i=0; i<K; i++) {
            if (A[i].sc > COLORS) continue;
            for (int j=0; j<K; j++) {
                if (A[j].sc > COLORS) continue;
                vll v = B[i][j];
                for (int f=1; f<=A[j].sc; f++) {
                    if (COLORS-A[i].sc-f >= 0) {
                        v[f] = v[f]*F[COLORS-A[i].sc] % Q;
                        v[f] = v[f]*FI[COLORS-A[i].sc-f] % Q;
                    } else {
                        v[f] = 0;
                    }
                }
                D[i][j] = total_vector_sum(v);
            }
        }
        
        vvll DP(M); for (int i=0; i<M; i++) DP[i] = vll(K);
        
        for (int i=0; i<K; i++) {
            DP[0][i] = COLORS >= A[i].sc ? F[COLORS]*FI[COLORS-A[i].sc] % Q : 0;
        }
        for (int n=0; n<M-1; n++) {
            for (int i=0; i<K; i++) for (int j=0; j<K; j++) {
                DP[n+1][j] += DP[n][i]*D[i][j];
                DP[n+1][j] %= Q;
            }
        }
        
        ull res = 0;
        for (int i=0; i<K; i++) {
            res += DP[M-1][i];
            if (res >= Q) res -= Q;
        }
        
        cout << COLORS << ": " << res << endl;
        coeffs.push_back(mp(COLORS,res));
    }

    Polynom::Polynom P = Polynom::interpolating_polynom(coeffs,Q);
    ans = Polynom::sum_values_to(P,1112131415);
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
