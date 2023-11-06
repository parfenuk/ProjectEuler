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

const vull F = Combinatorics::generate_factorials(12);

ull process_vector (vsint a, bool is_final = false)
{
    ull k = F[12-is_final];
    for (int i=0; i<(int)a.size(); i++) k /= F[a[i]];
    if (is_final || a[0] == 0) return k;
    a[0]--;
    return k - process_vector(a,true);
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
    
    vvsint a = Combinatorics::get_repeated_combinations(10,12);
    for (int i=0; i<(int)a.size(); i++) {
        ull k = process_vector(a[i]);
        ans += k*(k-1)/2;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
