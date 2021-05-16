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

int N, M, A[20][20];

void print_answer()
{
    vector<pii> cells;
    while (cells.size() != N*M) {
        bool found = false;
        for (int i=0; i<N; i++) for (int j=0; j<M; j++) {
            if (A[i][j] != 1) continue;
            found = true;
            A[i][j] = 0;
            if (i && A[i-1][j]) A[i-1][j]--;
            if (j && A[i][j-1]) A[i][j-1]--;
            if (i != N-1 && A[i+1][j]) A[i+1][j]--;
            if (j != M-1 && A[i][j+1]) A[i][j+1]--;
            cells.push_back(mp(i,j));
        }
        if (!found) { cout << "No"; return; }
    }
    cout << "Yes\n";
    for (int i=(int)cells.size()-1; i>=0; i--)
        cout << cells[i].fs+1 << " " << cells[i].sc+1 << endl;
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
    
    cin >> N >> M;
    for (int i=0; i<N; i++) for (int j=0; j<M; j++) cin >> A[i][j];
    if (N <= 2 || M <= 2) { print_answer(); return 0; }
    int P = 1; for (int i=0; i<N-2; i++) P *= 2;
    vector<vector<int>> G(262144);
    vector<pair<int,vint>> valid_numbers;
    for (int i=0; i<262144; i++) {
        vint d = NumberUtils::digits(i,2,18);
        bool ok = true;
        for (int j=1; j<(int)d.size(); j++) {
            if (d[j] && d[j-1]) { ok = false; break; }
        }
        if (ok) valid_numbers.push_back(mp(i,d));
    }
    for (int i=0; i<(int)valid_numbers.size(); i++)
    for (int j=i+1; j<(int)valid_numbers.size(); j++) {
        if ((valid_numbers[i].fs & valid_numbers[j].fs) == 0) {
            G[valid_numbers[i].fs].push_back(valid_numbers[j].fs);
            G[valid_numbers[j].fs].push_back(valid_numbers[i].fs);
        }
    }
    int cnt = 0;
    for (int i=0; i<262144; i++) cnt += G[i].size();
    cout << cnt;
    
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
