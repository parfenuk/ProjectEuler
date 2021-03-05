#include "Definitions.hpp"

#include "Algebra.hpp"
#include "Combinatorics.cpp"
#include "Complex.cpp"
// That's why we include .cpp here despite of having header
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

// TODO: Add definitions file

// TODO: write operators for pairs (with templates ofc, in utils)
// TODO: write map and set search: find minimum greater / maximum less

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
    
    string S;
    
    for (int i=0; i<30; i++) {
        Lnum A(i);
        cout << i << " " << Lnum::sqrt(A) << endl;
    }
    
    cout << Matrix::linear_recurrence_value({1,1}, {0,1}, 5, 1000000007) << endl;
    cout << Matrix::linear_recurrence_value({1,1}, {0,1}, 10, 1000000007) << endl;
    cout << Matrix::linear_recurrence_value({1,1}, {0,1}, 150000000, 1000000009) << endl;
            
    vector<dd> A;
    for (dd i=0.1; i<1; i+=0.1) A.push_back(cos(i));
    Containers::show(A);
    
    cout << Containers::total_vector_sum(A) << endl;
    
    Eratosthenes_sieve(100,true);
    vector<int> P = Containers::subvector(primes,5,9);
    Containers::show(P);
    cout << Containers::count_less_than(P,19) << endl;
    cout << Containers::count_greater_than(P,23) << endl;
    cout << Containers::count_greater_than(P,7) << endl;
    
    vector<ull> a = Combinatorics::get_powers(3,6);
    vector<ull> b = Combinatorics::get_factorials(6);
    a = a + b;
    Containers::show(a+b);
    
    Geometry::point p(1,2);
    Geometry3D::point q(1,2,3);
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
