#include "Definitions.hpp"

#include "Algebra.hpp"
#include "Combinatorics.cpp"
#include "Complex.cpp"
// That's why we include .cpp here despite of having header. Ugly workaround
// https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
#include "Containers.cpp"
#include "Geometry.cpp"
#include "Geometry3D.cpp"
#include "FFT.cpp"
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
    
    const int N = 20;
    int M = 10000000;
    
    vector<int> mark(M); // 1 or 2, depending on edges to lower level
    vector<pii> levels(N/2+1); // level 0: 0-0, level 1: 1-1, level 2: 2-4 etc.
    vector<vector<int>> G(M);
    // level 0
    levels[0] = mp(0,0);
    mark[0] = 7;
    for (int i=0; i<7; i++) G[0].push_back(1);
    // level 1
    levels[1] = mp(1,1);
    mark[1] = 1;
    G[1].push_back(0);
    for (int i=0; i<2; i++) G[1].push_back(1);
    
    for (int level=2; level<=N/2; level++) {
        const int from = levels[level-1].fs, to = levels[level-1].sc;
        int L = to + 1, R = L; // current level borders
        for (int i=from; i<=to; i++) {
            // add vertices for the current level
            mark[R++] = 2;
            for (int z=0; z<3-mark[i]; z++) mark[R++] = 1;
        }
        levels[level].fs = L;
        levels[level].sc = --R;
        
        // build edges with previous level
        int K = L;
        for (int i=from; i<=to; i++) {
            do {
                G[i].push_back(K);
                G[K].push_back(i);
                K++;
            } while (K <= R && mark[K] != 2);
            if (K <= R) {
                G[i].push_back(K);
                G[K].push_back(i);
            }
        }
        G[to].push_back(L);
        G[L].push_back(to);
        
        // build edges inside level
        for (int i=L; i<=R; i++) {
            int next = i+1 > R ? L : i+1;
            G[i].push_back(next);
            G[next].push_back(i);
        }
    }
    
//    for (int i=0; i<=N/2; i++) cout << i << ": " << levels[i].fs << " " << levels[i].sc << endl;
//    for (int i=0; i<=levels[N/2].sc; i++) {
//        cout << i << ": ";
//        for (int j=0; j<(int)G[i].size(); j++) cout << G[i][j] << " ";
//        cout << endl;
//    }
    
    M = levels[N/2].sc + 1;
    vector<Lnum> dp(M);
    dp[0] = 1;
    for (int z=1; z<=N; z++) {
        vector<Lnum> D(M);
        for (int i=0; i<M; i++) for (int j=0; j<(int)G[i].size(); j++) {
            int to = G[i][j];
            D[to] += dp[i];
        }
        dp.swap(D);
    }
    
    cout << M << endl << dp[0] << " ";
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
