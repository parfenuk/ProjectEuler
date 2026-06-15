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
    
    const int Q = 1000000007;
    const int N = 1000;
    
    vll M(N+1);
    
    // ###### I ######
    vint I(10);
    for (int i=1; i<=N; i++) {
        vint d = NumberUtils::digits(i,2,10);
        for (int j=0; j<10; j++) {
            if (d[9-j]) I[j]++;
        }
    }
    for (int i=0; i<10; i++) {
        M[0] += powmod(2,i)*(I[i]*I[i]/4);
    }
    
    // ###### X ######
    vvll A(N+1);
    for (int i=0; i<=N; i++) A[i] = vll(N+1);
    vector<vpii> B(1048576);
    
    for (int i=1; i<=N; i++) for (int j=1; j<=N; j++) {
        B[i*i ^ j*j].push_back(mp(i,j));
    }
    for (int x=0; x<1048576; x++) {
        for (int i=0; i<(int)B[x].size(); i++) {
            pii p = B[x][i];
            int a = p.fs, b = p.sc;
            for (int c=1; c<=N; c++) {
                if ((a*a ^ b*b) >= (b*b ^ c*c)) continue;
                A[b][c] = max(A[b][c], A[a][b] + (b*b ^ c*c));
                if (A[b][c] > M[1]) M[1] = A[b][c];
            }
        }
    }
    
    // ###### C ######
    for (int a=0; a<N; a++) for (int b=a; b<N; b++) for (int c=b; c<N; c++) {
        if ((a^b^c) == 0) continue;
        if ((a^b) > c) continue;
        if ((a^c) > b) continue;
        if ((b^c) > a) continue;
        if (c == a) M[2]++;
        else if (c == b || b == a) M[2] += 3;
        else M[2] += 6;
    }
    
    // Answer!
    for (int n=3; n<=N; n++) {
        M[n] = M[n-1]*M[n-2]%Q*M[n-3]%Q;
    } ans = M[1000];
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
