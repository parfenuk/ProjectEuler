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
#define EMPTY_TUBES 2
#define TUBE_SIZE 4
#define pic pair<int,char>

map<string,sint> D; // dist from initial position
map<string,pair<string,psii>> From; // < previous state, <from-to pouring> >

string normalized_form (vector<string> w) {
    
    string ret;
    sort(w.begin(), w.end());
    for (int i=0; i<(int)w.size(); i++) {
        ret += w[i];
        if (w[i].length() < TUBE_SIZE) ret += '$';
    }
    return ret;
}

string A[19], S[19]; // S is initial configuration, is never changed

// TODO: can be calculated dynamically
sint depth (sint k) // returns how many identical blocks of water from the top have the same type
{
    sint cnt = 1, id = (int)A[k].size()-2;
    while (id >= 0 && A[k][id] == A[k].back()) { cnt++; id--; }
    return cnt;
}

bool one_color (int k) // checks if tube A[k] consists of only one water type
{
    return depth(k) == (int)A[k].size();
}

// TODO: maybe, it's worth to use auxiliary arrays like for one_colored and empty like for dfs algo
bool is_solved_position()
{
    sint empty = 0, one_colored = 0;
    for (int i=0; i<N; i++) {
        if (A[i].empty()) empty++;
        else if (one_color(i)) one_colored++;
        else return false;
    }
    return empty == EMPTY_TUBES;
}

sint bfs_pouring_count (sint from, sint to) // returns how many cells we can pour, if it makes any sense
{
    if (A[from].empty()) return 0; // nothing to pour
    if (A[to].size() == TUBE_SIZE) return 0; // nowhere to pour
    char cf = A[from].back(), ct = '#'; // color from, color to (is unknown yet)
    if (!A[to].empty()) ct = A[to].back();
    if (ct == '#') return depth(from); // pouring to empty tube
    if (cf != ct) return 0; // we can't mix up different water types
    int d = depth(from), free = TUBE_SIZE - (int)A[to].size();
    return min(d,free);
}

vector<psii> bfs_answer (string f)
{
    vector<psii> bfs_moves;
    do {
        pair<string,psii> p = From[f];
        bfs_moves.push_back(p.sc);
        f = p.fs;
    } while (!f.empty());
    
    bfs_moves.pop_back();
    reverse(bfs_moves.begin(), bfs_moves.end());
    return bfs_moves;
}

vector<psii> bfs()
{
    for (int i=0; i<N; i++) A[i] = S[i];
    vector<string> w(A,A+N);
    string NF = normalized_form(w);
    D[NF] = 0;
    From[NF] = mp("",mp(0,0));
    queue<vector<string>> q;
    q.push(w);
    
    while (!q.empty()) {
        w = q.front();
        q.pop();
        for (int i=0; i<N; i++) A[i] = w[i];
        string F = normalized_form(w);
        sint d = D[F]+1;
        
        if (is_solved_position()) return bfs_answer(F);
        
        for (sint from=0; from<N; from++) for (sint to=0; to<N; to++) {
            if (from == to) continue;
            int cnt = bfs_pouring_count(from,to);
            if (cnt == 0) continue;
            char wt = w[from].back();
            for (int i=0; i<cnt; i++) { w[from].pop_back(); w[to].push_back(wt); }
            NF = normalized_form(w);
            if (D.find(NF) == D.end()) {
                D[NF] = d+1;
                From[NF] = mp(F,mp(from,to));
                q.push(w);
            }
            for (int i=0; i<cnt; i++) { w[from].push_back(wt); w[to].pop_back(); }
        }
    }
            
    return vector<psii>();
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

void dfs (int from, int to, int max_moves = 60) // initial pouring is guaranteed to be valid
{
    if (moves.size() >= max_moves) return;
    
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

void print_dfs_solution()
{
    vector<pair<pii,int>> final_moves; // optimal among all dfs start moves
    
    for (int k=0; k<N-EMPTY_TUBES; k++) {
        reset();
        dfs(k,N-EMPTY_TUBES);
        if (solution_found) {
            cout << k+1 << ": SOLUTION IN " << moves.size() << " MOVES\n";
            if (final_moves.empty() || moves.size() < final_moves.size()) final_moves = moves;
        }
        else {
            cout << k+1 << ": NO SOLUTION\n";
        }
    }
    
    // print final answer
    //for (int i=0; i<(int)final_moves.size(); i++) cout << final_moves[i].fs.fs+1 << " -> " << final_moves[i].fs.sc+1 << endl;
}

void print_bfs_solution()
{
    vector<psii> final_moves = bfs();
    cout << "OPTIMAL SOLUTION IN " << final_moves.size() << " MOVES\n";
    for (int i=0; i<(int)final_moves.size(); i++) cout << final_moves[i].fs+1 << " -> " << final_moves[i].sc+1 << endl;
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
    int initial_empty_tubes; cin >> initial_empty_tubes;
    for (int i=0; i<N-initial_empty_tubes; i++) cin >> S[i];
    
    //print_dfs_solution();
    print_bfs_solution();
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
