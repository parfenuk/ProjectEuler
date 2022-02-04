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
    
    vpii F = {mp(0,1),mp(0,2),mp(1,2),mp(3,4),mp(3,5),mp(4,5),mp(6,7),mp(6,8),mp(7,8),mp(10,11),mp(10,12),mp(11,12),mp(13,14),mp(15,16),mp(13,17)}; // forbidden pairs, should be in different trios
    vpii R = {mp(9,12)}; // required pairs, should be together
    vpii C = {mp(1,0),mp(8,2)}; // some elements must be in specific trio
    vpii D = {mp(4,0),mp(9,0)}; // some elements shouldn't be in specific trio
    
    vint v; for (int i=0; i<18; i++) v.push_back(i);
    random_device rd;
    mt19937 g(rd());
    
    vint A(18);
    bool ok = false;
    
    while (!ok) {
        ans++;
        shuffle(v.begin(), v.end(), g);
        for (int i=0; i<18; i++) A[v[i]] = i/6;
        
        ok = true;
        // F check
        for (int i=0; i<(int)F.size(); i++) {
            if (A[F[i].fs] == A[F[i].sc]) { ok = false; break; }
        } if (!ok) continue;
        // R check
        for (int i=0; i<(int)R.size(); i++) {
            if (A[R[i].fs] != A[R[i].sc]) { ok = false; break; }
        } if (!ok) continue;
        // C check
        for (int i=0; i<(int)C.size(); i++) {
            if (A[C[i].fs] != C[i].sc) { ok = false; break; }
        } if (!ok) continue;
        // D check
        for (int i=0; i<(int)D.size(); i++) {
            if (A[D[i].fs] == D[i].sc) { ok = false; break; }
        }
    }
    
    sort(v.begin(), v.begin()+6);
    sort(v.begin()+6, v.begin()+12);
    sort(v.begin()+12, v.end());
    
    const vector<string> w = {"90s","00s","10s","Dud","Mladenci","Kino","Numbers","3Letters","PRO","Covers","Solo","Siri","Minuses","Eating","Colors","FRock","RRock","Blogers"};
    
    cout << "FOUND after " << ans << " iterations\n";
    for (int i=0; i<18; i++) {
        if (i%6 == 0) cout << endl;
        cout << w[v[i]] << " ";
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
