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

int N; // total amount of tubes, usually 14 or 11
#define TUBE_SIZE 4
#define EMPTY_TUBES 2
#define pic pair<int,char>

string A[19], S[19]; // S is initial configuration, is never changed

// TODO: can be calculated dynamically
int depth (int k) // returns how many identical blocks of water from the top have the same type
{
    //if (A[k].empty()) return 0; // it is never called for empty tubes only, so could be removed
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
    //if (A[from].empty()) return 0; // nothing to pour (never called for empty, same as for depth)
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
            if (A[i].size() != TUBE_SIZE && A[i].back() == cf) return (i+1)*10;
        }
    }
    
    return 0; // otherwise move makes no sense
}

vector<pair<pii,int>> moves; // < <from, to>, how many >
int one_color_count = 0, empty_count = EMPTY_TUBES;
vector<bool> OC(N); // one color tubes storage
bool solution_found = false;

void reset()
{
    moves.clear();
    one_color_count = 0;
    empty_count = EMPTY_TUBES;
    OC = vector<bool>(N);
    solution_found = false;
    for (int i=0; i<N; i++) A[i] = S[i];
}

void move_back()
{
    int from = moves.back().fs.fs, to = moves.back().fs.sc, wt = A[to].back();
    for (int i=0; i<(int)moves.back().sc; i++) {
        A[from].push_back(wt);
        A[to].pop_back();
    }
    moves.pop_back();
}

void dfs (int from, int to, int max_moves = 50) // initial pouring is guaranteed to be valid
{
    if (moves.size()+1 >= max_moves) return;
    
    // check if pouring is valid
    int cnt = pouring_count(from,to);
    if (cnt == 0) return; // failed move
    char wt = A[from].back(); // water type
    int to2 = -1;
    
    // perform pouring
    if (cnt > 4) {
        to2 = cnt/10 - 1; // decode it
        A[from].resize(A[from].size()-2);
        A[to].push_back(wt);
        A[to2].push_back(wt);
        moves.push_back(mp(mp(from,to),1));
        moves.push_back(mp(mp(from,to2),1));
    }
    else {
        for (int i=0; i<cnt; i++) { A[from].pop_back(); A[to].push_back(wt); }
        moves.push_back(mp(mp(from,to),cnt));
    }
    
    // update values
    int oc_change = 0, emp_change = 0;
    bool from_reverse = false, to_reverse = false;
    if (OC[from]) { // one colored tube becomes empty
        if (A[from].empty()) { OC[from] = false; from_reverse = true; oc_change--; emp_change++; }
    }
    else { // multi colored tube becomes one colored
        if (one_color(from)) { OC[from] = true; from_reverse = true; oc_change++; }
    }
    // empty tube becomes one colored (case cnt > 4 is impossible here)
    if (A[to].size() == cnt) { OC[to] = true; to_reverse = true; oc_change++; emp_change--; }
    one_color_count += oc_change;
    empty_count += emp_change;
    
    // check for solution
    if (one_color_count == N-EMPTY_TUBES && empty_count == EMPTY_TUBES) { solution_found = true; return; }
    
    // go deep
    for (int i=0; i<N; i++) {
        if (A[i].empty()) continue;
        if (i == to || i == to2) continue; // makes no sense
        for (int j=0; j<N; j++) {
            if (i == j) continue;
            if (i < from && j != from) continue; // two independent pourings, wrong order
            dfs(i,j,max_moves);
            if (solution_found) return;
        }
    }
    
    // reverse move back
    if (from_reverse) OC[from] = !OC[from];
    if (to_reverse)   OC[to]   = !OC[to];
    one_color_count -= oc_change;
    empty_count    -= emp_change;
    move_back();
    if (cnt > 4) move_back(); // one more time in this case
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
     c: cyan (light green)
     b: blue
     i: indigo
     v: violet
     p: pink
     s: swamp (toxic green)
     w: brown
     g: grey
     */
    
    cin >> N;
    for (int i=0; i<N-EMPTY_TUBES; i++) cin >> S[i];
    vector<pair<pii,int>> final_moves; // optimal among all dfs start moves
    
    for (int k=0; k<N-EMPTY_TUBES; k++) {
        reset();
        dfs(k,N-EMPTY_TUBES,final_moves.empty() ? 50 : (int)final_moves.size());
        if (solution_found) {
            cout << k+1 << ": SOLUTION IN " << moves.size() << " MOVES\n";
            if (final_moves.empty() || moves.size() < final_moves.size()) final_moves = moves;
        }
        else {
            cout << k+1 << ": NO SOLUTION\n";
        }
    }
    
    // print final answer
    for (int i=0; i<(int)final_moves.size(); i++) cout << final_moves[i].fs.fs+1 << " -> " << final_moves[i].fs.sc+1 << endl;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
