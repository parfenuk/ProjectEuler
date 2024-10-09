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

const int Q = 998244353;
const ll ORDER = powmod(2,23)*7*17;
const vint ROOTS = { 3,5,6,10,11,12,13,20,22,24,26,27,35,40,42,47,48,51 };
const int N = 900000;

vector<pair<pii,int>> min_orders(N+1, mp(mp(1000000000,-1),0)); // < min_order, primitive_root >

void fill_orders()
{
    for (int i=0; i<=N; i++) min_orders[i].sc = i;
    
    for (int i=0; i<1; i++) {
        const ll root = powmod(ROOTS[i], (Q-1)/ORDER, Q);
        ll s = 1;
        int cnt = 0;
        for (int j=1; j<=ORDER; j++) {
            s = s*root % Q;
            if (s <= N && j < min_orders[s].fs.fs) {
                if (min_orders[s].fs.fs > Q) cnt++;
                min_orders[s].fs = mp(j,root);
            }
        }
        cout << "Root " << i << ": " << cnt << " updates\n";
    }
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
    
    fill_orders();
    sort(min_orders.begin(), min_orders.end());
    reverse(min_orders.begin(), min_orders.end());
    for (int i=0; i<1000; i++) {
        //if (min_orders[i].fs.fs < Q) { cout << i << endl; break; }
        cout << min_orders[i].sc << " " << min_orders[i].fs.fs << " " << min_orders[i].fs.sc << endl;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
