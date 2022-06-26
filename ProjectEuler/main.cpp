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
    
    random_device r2d2;
    mt19937 g2(r2d2());
    vector<int> a;
    for (int i=0; i<30; i++) a.push_back(i);
    shuffle(a.begin(),a.end(),g2);
    for (int i=0; i<30; i++) {
        cout << a[i]/5 + 1 << " " << (a[i]%5+1)*300 << endl;
    }
    
    const vector<string> w = {
        "Hard Rock","Hard Pop","Hard Rap", //  0  1  2
        "PRO","Colors","Sergey",           //  3  4  5
        "NO","Dance","Birds",              //  6  7  8
        "Mash-ups","N.N.","Weapons",       //  9 10 11
        "CHBD","Miroslav","Minuses",       // 12 13 14
        "AA","4 Letters","E"               // 15 16 17
    };
    const vector<int> cats = { 3,4,5,10,11 };
    
    vpii F = {mp(3,15),mp(6,17),mp(8,11),mp(12,13)}; // forbidden pairs, should be in different trios
    vpii R = {}; // required pairs, should be together
    vpii C = {mp(0,1),mp(1,2),mp(2,3)}; // some elements must be in specific trio
    vpii D = {mp(17,1)}; // some elements shouldn't be in specific trio
    
    vint v; for (int i=0; i<18; i++) v.push_back(i);
    random_device rd;
    mt19937 g(rd());
    
    vint A(18);
    bool ok = false;
    
    while (!ok) {
        ans++;
        shuffle(v.begin(), v.end(), g);
        vector<int> catscnt(3);
        for (int i=0; i<18; i++) {
            A[v[i]] = i/6;
            if (contains(cats,v[i])) catscnt[i/6]++;
        } if (catscnt[0] != 2 || catscnt[1] != 1) continue;
        
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
            if (A[C[i].fs]+1 != C[i].sc) { ok = false; break; }
        } if (!ok) continue;
        // D check
        for (int i=0; i<(int)D.size(); i++) {
            if (A[D[i].fs]+1 == D[i].sc) { ok = false; break; }
        }
    }
    
    sort(v.begin(), v.begin()+6);
    sort(v.begin()+6, v.begin()+12);
    sort(v.begin()+12, v.end());
    
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
