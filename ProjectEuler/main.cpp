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
using namespace Combinatorics;
using namespace Containers;
using namespace NumberUtils;
using point = Geometry3D::point;
using matrix = Matrix::matrix;

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
    
    vvsint A = sum_partitions(19);
    for (int i=0; i<(int)A.size(); i++) show(A[i]);
    
    const ull P = 109;
    vull a = generate_fixed_powers(P-1,3,P);
    set<ull> s;
    for (int i=0; i<(int)a.size(); i++) s.insert(a[i]);
    
    for (set<ull>::iterator it=s.begin(); it!=s.end(); it++)
    for (set<ull>::iterator kt=s.begin(); kt!=s.end(); kt++)
    {
        ull sum = (*it + *kt) % P;
        if (s.find(sum) != s.end()) {
            cout << *it << " + " << *kt << " = " << sum << endl;
            ans++;
        }
    }
    cout << ans << endl;
    ans = 0;
    for (ull i=1; i<P; i++) for (ull j=1; j<P; j++) {
        if ((powmod(i,3,P)+P-powmod(j,3,P)) % P == 1) {
            ans++;
        }
    } ans /= 9;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
