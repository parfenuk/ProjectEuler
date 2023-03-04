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
        "Trio","Mashups","Minuses",  //  0  1  2
        "ELE","Memes","Rock",        //  3  4  5
        "DIS","4Letters","Pop",      //  6  7  8
        "Demons","SingRus","SingEng",//  9 10 11
        "Plays","Sasha","Alice",     // 12 13 14
        "1eq2","Ends","Body"         // 15 16 17
    };
    const vector<int> cats = { 5,6,7,8,9 };
    const vector<int> auctions = { 0,1,2,6,7,9,10,12 };
    const vector<int> cats_distribution = { 1,2,2 }; // cats in each round
    const vector<int> auctions_distribution = { 2,1,2 }; // same for auctions
    
    vpii F = {mp(3,6),mp(3,15),mp(6,15),mp(10,12),mp(13,14),mp(1,4),mp(9,17)}; // forbidden pairs, should be in different trios
    vpii R = {mp(5,8),mp(10,11)}; // required pairs, should be together
    vpii C = {mp(0,3),mp(10,1),mp(14,1),mp(16,3),mp(6,1)}; // some elements must be in specific trio
    vpii D = {mp(1,3),mp(2,1),mp(3,1),mp(7,1),mp(1,1)}; // some elements shouldn't be in specific trio
    
    vint v; for (int i=0; i<18; i++) v.push_back(i);
    random_device rd;
    mt19937 g(rd());
    
    vint A(18);
    bool ok = false;
    
    while (!ok) {
        ans++;
        shuffle(v.begin(), v.end(), g);
        vector<int> catscnt(3), auccnt(3);
        for (int i=0; i<18; i++) {
            A[v[i]] = i/6;
            if (contains(cats,v[i])) catscnt[i/6]++;
            if (contains(auctions,v[i])) auccnt[i/6]++;
        }
        if (catscnt[0] != cats_distribution[0] ||
            catscnt[1] != cats_distribution[1]) continue;
        if (auccnt[0] < auctions_distribution[0] ||
            auccnt[1] < auctions_distribution[1] ||
            auccnt[2] < auctions_distribution[2]) continue;
        
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
