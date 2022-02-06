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
    
    const int N = 20, NN = N*N/2;
    vector<sint> A(NN+1,4);
    
    // 3-set
    for (int a=1; a<=N; a++) for (int b=1; b<=a; b++) {
        int k = a*a + b*b;
        if (k <= NN) A[k] = 3;
        k = a*a - b*b;
        if (k <= NN) A[k] = 3;
    }
    for (int a=1; a<=N; a++) for (int b=1; b<a; b++) {
        int k = a*b;
        if (k <= NN) A[k] = 3;
    }
    // 2-set
    for (int a=1; a*a<=NN; a++) {
        A[a*a] = 2;
    }
    // 1-set
    A[0] = 1;
    
    for (int i=0; i<=NN; i++) cout << i << ": " << A[i] << endl;
    
    // answer calcualtion
    for (int i=0; i<NN; i++) ans += A[i]*2;
    ans += A[NN];
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
