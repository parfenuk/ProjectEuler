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

#define N 7
#define M 7
#define Q 7

const int SIZE = N*M;

const vector<psii> directions = {
    mp(1,1), mp(1,0), mp(1,-1), mp(-1,0),
    mp(-1,1), mp(0,1), mp(-1,-1), mp(0,-1)
};

sint F[N][M];
int cnt_attacked = 0;
int max_not_attacked = 0;

void update_field (sint row, sint col, bool add)
{
    if (add) { F[row][col]++; if (F[row][col] == 1) cnt_attacked++; }
    else { F[row][col]--; if (F[row][col] == 0) cnt_attacked--; }
}

psii cell (sint ID)
{
    ID--;
    return mp(ID/M,ID%M);
}

void show_field (const vsint &a)
{
    string S[N][M];
    for (int i=0; i<N; i++) for (int j=0; j<M; j++) {
        if (F[i][j]) S[i][j] = '*';
        else S[i][j] = 'o';
    }
    for (int i=0; i<(int)a.size(); i++) {
        psii p = cell(a[i]);
        S[p.fs][p.sc] = 'Q';
    }
    for (int i=0; i<N; i++) {
        for (int j=0; j<M; j++) cout << S[i][j] << " ";
        cout << endl;
    }
    cout << "Free cells: " << max_not_attacked << "\n";
    if (a[0] + a.back() == SIZE+1) cout << "Symmetric\n\n";
    else cout << "\n";
}

void update_queen (sint ID, bool add)
{
    psii p = cell(ID);
    sint row = p.fs, col = p.sc;
    update_field(row,col,add);
    
    for (int d=0; d<(int)directions.size(); d++) {
        sint r = row, c = col;
        for (int i=1;;i++) {
            r += directions[d].fs;
            if (r == -1 || r == N) break;
            c += directions[d].sc;
            if (c == -1 || c == M) break;
            update_field(r,c,add);
        }
    }
}

bool next_combination (vsint &a, int n, int k)
{
    for (int i=k-1; i>=0; i--) {
        //if (a[i] > n-k+i) { update_queen(a[i],false); continue; }
        if (a[i] > SIZE-k+i-a[0]+1) { update_queen(a[i],false); continue; }
        update_queen(a[i],false);
        a[i]++;
        update_queen(a[i],true);
        for (int j=i+1; j<k; j++) { a[j] = a[j-1] + 1; update_queen(a[j],true); }
        if (i <= 1) cout << a[0] << " " << a[1] << endl;
        return true;
    } return false;
}

void get_combinations (int n, int k) // numeration starts from 1
{
    vsint a;
    for (int i=1; i<=k; i++) { a.push_back(i); update_queen(i,true); }
    for (int i=0; i<N; i++) {
        for (int j=0; j<M; j++) cout << F[i][j] << " ";
        cout << endl;
    }
    while (next_combination(a,n,k)) {
//        for (int i=0; i<N; i++) {
//            for (int j=0; j<M; j++) cout << F[i][j] << " ";
//            cout << endl;
//        }
        if (SIZE - cnt_attacked >= max_not_attacked) {
            max_not_attacked = SIZE - cnt_attacked;
            show_field(a);
        }
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
    
    vector<string> A = { "Влад", "Маша", "Даша", "Диана", "Егор", "Юра", "Макс", "Вадим", "Атрур", "Амир", "Юля", "Настя" };
    
    vector<int> P; for (int i=0; i<12; i++) P.push_back(i);
    random_device rng;
    mt19937 urng(rng());
    shuffle(P.begin(), P.end(), urng);
    
    cout << "Мирослав ";
    for (int i=0; i<3; i++) {
        cout << A[P[i]] << " ";
    } cout << endl;
    
    cout << "Люба ";
    for (int i=3; i<6; i++) {
        cout << A[P[i]] << " ";
    } cout << endl;
    
    cout << "Артур ";
    for (int i=6; i<9; i++) {
        cout << A[P[i]] << " ";
    } cout << endl;
    
    cout << "Даша ";
    for (int i=9; i<12; i++) {
        cout << A[P[i]] << " ";
    } cout << endl;
    
    //for (int i=0; i<N; i++) for (int j=0; j<M; j++) F[i][j] = 0;
    
    //get_combinations(SIZE,Q);
    
//    vvsint a = get_combinations(10,5);
//    for (int i=0; i<(int)a.size(); i++) show(a[i]);
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
