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

#define N 14
#define TUBE_SIZE 4
#define EMPTY_TUBES 2
#define tube vchar
#define pic pair<int,char>

tube A[N];
void fill_tube (int k, const string &S)
{
    A[k].clear();
    for (int i=0; i<(int)S.length()/*=4*/; i++) A[k].push_back(S[i]);
}

// TODO: can be calculated dynamically
int depth (int k) // returns how many identical blocks of water from the top have the same type
{
    if (A[k].empty()) return 0; // possibly will be called for non-empty tubes only, so could be removed
    int cnt = 1, id = (int)A[k].size()-2;
    while (id >= 0 && A[k][id] == A[k].back()) { cnt++; id--; }
    return cnt;
}

bool one_color (int k) // checks if tube A[k] consists of only one water type
{
    return depth(k) == (int)A[k].size();
}

int pouring_count (int from, int to) // returns how many cells we can pour, if it makes any sense
{
    if (A[from].empty()) return 0; // nothing to pour
    if (A[to].size() == TUBE_SIZE) return 0; // nowhere to pour
    
    char cf = A[from].back(), ct = '#'; // color from, color to (is unknown yet)
    if (!A[to].empty()) ct = A[to].back();
    
    if (ct == '#') { // pouring to empty tube
        if (one_color(from)) return 0; // move makes no sense
        return depth(from);
    }
    
    if (cf != ct) return 0; // we can't mix up different water types
    
    int d = depth(from), free = TUBE_SIZE - (int)A[to].size();
    if (d <= free) return d; // correct and meaningful move
    
    if (d == 2) { // still one complex move can make sense
        for (int i=0; i<N; i++) {
            if (i == from || i == to) continue;
            if (A[i].size() != TUBE_SIZE && A[i].back() == cf) return 5;
        }
    }
    
    return 0; // otherwise move makes no sense
}

vector<pair<pii,pic>> moves; // < <from, to>, <how many, water type> >
int one_color_count = 0, empty_count = EMPTY_TUBES;
vector<bool> OC(N); // one color tubes storage
bool solution_found = false;

void dfs (int from, int to) // initial pouring is guaranteed to be valid
{
    if (solution_found) return;
    
    int cnt = pouring_count(from,to);
    if (cnt == 0) return; // failed move
    char wt = A[from].back(); // water type
    int to2 = -1;
    
    // perform pouring
    if (cnt == 5) {
        for (int i=0; i<N; i++) { // set additional tube we pour to
            if (i == from || i == to) continue;
            if (A[i].size() != TUBE_SIZE && A[i].back() == wt) { to2 = i; break; }
        }
        A[from].pop_back();
        A[from].pop_back();
        A[to].push_back(wt);
        A[to2].push_back(wt);
        moves.push_back(mp(mp(from,to),mp(1,wt)));
        moves.push_back(mp(mp(from,to2),mp(1,wt)));
    }
    else {
        for (int i=0; i<cnt; i++) { A[from].pop_back(); A[to].push_back(wt); }
        moves.push_back(mp(mp(from,to),mp(cnt,wt)));
    }
    
    // update_values
    int oc_change = 0, emp_change = 0;
    bool from_reverse = false, to_reverse = false;
    if (OC[from]) { // one colored tube becomes empty
        if (A[from].empty()) { OC[from] = false; from_reverse = true; oc_change--; emp_change++; }
    }
    else { // multi colored tube becomes one colored
        if (one_color(from)) { OC[from] = true; from_reverse = true; oc_change++; }
    }
    // empty tube becomes one colored (case cnt == 5 is impossible here)
    if (A[to].size() == cnt) { OC[to] = true; to_reverse = true; oc_change++; emp_change--; }
    one_color_count += oc_change;
    empty_count += emp_change;
    
    // check for solution
    if (one_color_count == N-EMPTY_TUBES && empty_count == EMPTY_TUBES) { solution_found = true; return; }
    
    // go deep
    for (int i=0; i<N; i++) for (int j=0; j<N; j++) {
        if (i != j) dfs(i,j);
    }
    
    if (solution_found) return;
    
    // reverse move back
    if (from_reverse) OC[from] = !OC[from];
    if (to_reverse)   OC[to]   = !OC[to];
    one_color_count -= oc_change;
    empty_count -= emp_change;
    for (int i=0; i<(int)moves.back().sc.fs; i++) {
        A[from].push_back(wt);
        A[to].pop_back();
    }
    moves.pop_back();
    if (cnt == 5) { // do it one more time
        for (int i=0; i<(int)moves.back().sc.fs; i++) {
            A[from].push_back(wt);
            A[to].pop_back();
        }
        moves.pop_back();
    }
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    /* colors:
     r: red
     o: orange
     y: yellow
     d: dark green
     l: light green (salad)
     b: blue
     i: indigo
     v: violet
     p: pink
     s: swamp (toxic green)
     w: brown
     g: grey
     */
    
    for (int i=0; i<N-EMPTY_TUBES; i++) {
        string S; cin >> S;
        fill_tube(i,S);
    }
    
    for (int k=0; k<N-EMPTY_TUBES; k++) {
        dfs(k,N-EMPTY_TUBES);
        if (solution_found) {
            cout << "SOLUTION:\n";
            for (int i=0; i<(int)moves.size(); i++) cout << moves[i].fs.fs+1 << " -> " << moves[i].fs.sc+1 << endl;
            break;
        }
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
