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
using namespace Containers;

const int N = 360, K = 20;
//const int N = 36, K = 6;
vector<int> C;
//const vector<int> C = {1,0,15,10,105,126,525,780,2055,3060,5955,8010,12285,14190,17715,17190,17715,14190,12285,8010,5955}; // precalculated for N = 360, K = 20
//const vector<int> C = {1,0,3,2,3,0,1}; // precalculated for N = 36, K = 6
vector<pdd> D(N);

bool isBalanced (const vsint &a)
{
    pdd p = mp(0,0);
    for (int i=0; i<(int)a.size(); i++) {
        p.fs += D[a[i]-1].fs;
        p.sc += D[a[i]-1].sc;
    }
    return fabs(p.fs) < EPS && fabs(p.sc) < EPS;
}

ull count (const vsint &a)
{
    vint A(a[0]+1);
    for (int i=0; i<(int)a.size(); i++) A[a[i]]++;
    
    ull cnt = 1;
    ull free_groups = N/rad(N);
    
    for (int i=1; i<(int)A.size(); i++) {
        // if A[i] equals 0, nothing changes
        cnt *= Combinatorics::Binomial(free_groups, A[i]);
        cnt *= powmod(C[i],A[i]);
        free_groups -= A[i];
    }
    
    return cnt;
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

    // *** PRECALC CODE FOR OBTANING ARRAY 'C' ***
    int n = (int)rad(N);
    for (int i=0; i<n; i++) D[i] = mp(cos(2*PI*i/n),sin(2*PI*i/n));

    C = vector<int>(K+1);

    for (int k=1; k<=K; k++) {
        vsint a;
        for (int i=1; i<=k; i++) a.push_back(i);
        if (isBalanced(a)) C[k]++;
        while (Combinatorics::next_combination(a,n,k)) {
            if (isBalanced(a)) C[k]++;
        }
        cout << "K = " << k << ": " << C[k] << " ways\n";
    }
    
    // *** MAIN CODE ***
    vvsint A = Combinatorics::sum_partitions(K);
    for (int i=0; i<(int)A.size(); i++) ans += count(A[i]);
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
