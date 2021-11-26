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
    
    vector<dd> A = { 1.0 };
    int k = 0;
    int n = 1; // current length of A
    
    while (true) {
        k++;
        vector<dd> B(n+2);
        B[0] = A[0] / 2;
        B[n+1] = 1.0;
        int mean = n/2 + 1;
        for (int i=1; i<mean; i++) B[i] = (B[i-1]+A[i]) / 2;
        for (int i=n; i>mean; i--) B[i] = (B[i+1]+A[i-2]) / 2;
        B[mean] = (B[mean-1] + B[mean+1]) / 2;
        if (B[mean] < 0.5000250012500626) {
            cout << k << " " << fixed << B[mean] << " " << 1.0/B[mean];
            break;
        }
        
        n += 2;
        A.swap(B);
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
