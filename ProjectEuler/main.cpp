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

#define HOR false
#define VER true

vector<vector<sint>> Sums[40][7];

void fill_sums()
{
    for (int n=1; n<=6; n++) {
        vvsint C = Combinatorics::get_combinations(9,n);
        for (int k=0; k<(int)C.size(); k++) {
            vsint A = C[k];
            sint sum = 0;
            for (int i=0; i<n; i++) sum += A[i];
            do {
                Sums[sum][n].push_back(A);
            } while (next_permutation(A.begin(), A.end()));
        }
    }
}

string Field[7][7];

struct strip
{
    bool type;
    string cells;
    string head; // 1 or 2 symbols
    sint sum; // value depends on certain permutation
    psii pos; // row and column
    vector<pair<sint,psii>> crosses;
    strip() {}
    strip(const string &S) {
        if (S[1] == 'v') type = VER;
        else type = HOR;
        head = S.substr(2,S.length()-3);
    }
    void fill_cells() {
        cells.clear();
        psii inc = mp(type==VER,type==HOR);
        psii p = pos;
        do {
            p.fs += inc.fs;
            p.sc += inc.sc;
            if (p.fs > 6 || p.sc > 6 || Field[p.fs][p.sc].length() != 1 || Field[p.fs][p.sc] == "X") break;
            cells += Field[p.fs][p.sc];
        } while (true);
    }
};

bool strip_sort (const strip &a, const strip &b)
{
    return a.cells.length() < b.cells.length();
}

psii intersection (const strip &a, const strip &b)
{
    if (a.type == b.type) return mp(-1,-1);
    if (a.type == VER) {
        psii p = intersection(b,a);
        return mp(p.sc,p.fs);
    }
    if (a.pos.sc >= b.pos.sc) return mp(-1,-1);
    if (a.pos.fs <= b.pos.fs) return mp(-1,-1);
    sint r = a.pos.fs - b.pos.fs, c = b.pos.sc - a.pos.sc;
    if (b.cells.length() < r || a.cells.length() < c) return mp(-1,-1);
    return mp(c-1,r-1);
}
        
vector<strip> strips;
set<char> non_zero_letters;

void read_field_unit (sint r, sint c, const string &S)
{
    Field[r][c] = S;
    if (S[0] == '(') {
        size_t comma = S.find(",");
        if (comma == string::npos) {
            strip A(S);
            A.pos = mp(r,c);
            strips.push_back(A);
            non_zero_letters.insert(S[2]);
        }
        else {
            string S1 = S.substr(0,comma+1);
            string S2 = S.substr(comma,S.length()-comma);
            strip A(S1), B(S2);
            A.pos = B.pos = mp(r,c);
            strips.push_back(A);
            strips.push_back(B);
            non_zero_letters.insert(S1[2]);
            non_zero_letters.insert(S2[2]);
        }
    }
    else if (S[0] != 'O' && S[0] != 'X') non_zero_letters.insert(S[0]);
}

// sum and length are known and stored in 's'
bool ok_sum (const strip &s, sint k, const vsint &combs, const vint &P)
{
    for (int i=0; i<(int)s.cells.length(); i++) { // strip itself check
        if (s.sum >= 40) return false;
        if (s.cells[i] != 'O' && Sums[s.sum][s.cells.length()][k][i] != P[s.cells[i]-'A']) return false;
    }
    for (int i=0; i<(int)s.crosses.size(); i++) { // crossed strips check
        sint id = s.crosses[i].fs;
        sint a = s.crosses[i].sc.fs, b = s.crosses[i].sc.sc;
        if (combs[id] == -1) continue;
        if (Sums[s.sum][s.cells.length()][k][a] != Sums[strips[id].sum][strips[id].cells.length()][combs[id]][b]) return false;
    }
    return true;
}

int attempts = 0;

bool try_find_solution (const vint &P)
{
    for (set<char>::iterator it=non_zero_letters.begin(); it!=non_zero_letters.end(); it++) {
        if (P[*it-'A'] == 0) return false;
    }
    
    int M = (int)strips.size();
    sint vert_sum = 0, hor_sum = 0;
    for (int i=0; i<M; i++) { // sum set and basic validation check
        string S = strips[i].head;
        if (S.length() == 1) strips[i].sum = P[S[0]-'A'];
        else strips[i].sum = 10*P[S[0]-'A'] + P[S[1]-'A'];
        if (Sums[strips[i].sum][strips[i].cells.length()].empty()) return false;
        sint min_sum = 0;
        for (int j=0; j<(int)strips[i].cells.length(); j++) {
            char c = strips[i].cells[j];
            if (c != 'O') min_sum += P[c-'A'];
        }
        if (strips[i].sum < min_sum) return false;
        if (strips[i].type == VER) vert_sum += strips[i].sum;
        else hor_sum += strips[i].sum;
    } if (vert_sum != hor_sum) return false;
    
    attempts++;
    
    // backtrack algo
    vsint combs(M,-1);
    sint cur = 0, from = 0;
    while (true) {
        if (from >= Sums[strips[cur].sum][strips[cur].cells.length()].size()) { // step back
            combs[cur] = -1;
            cur--;
            if (cur == -1) return false; // no solution here
            from = combs[cur]+1;
        }
        else if (ok_sum(strips[cur],from,combs,P)) { // try set new sum
            combs[cur] = from;
            cur++;
            from = 0;
            if (cur == M) return true; // solution found
        }
        else from++;
    }
    
    return false; // never reaches here actually
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
    
    fill_sums();
    
    for (int T=0; T<200; T++) {
        
        strips.clear();
        non_zero_letters.clear();
        attempts = 0;
        
        string Kakuro; cin >> Kakuro;
        vector<string> S = StringUtils::parse_by_symbol(Kakuro, ',');
        int N = atoi(S[0].c_str());
        if (N == 6) for (int i=0; i<7; i++) Field[i][6] = Field[6][i] = "X";
        int id = 1;
        for (int i=0; i<N; i++) for (int j=0; j<N; j++) {
            if (S[id][0] == '(' && S[id].back() != ')') {
                id++;
                S[id] = S[id-1] + ',' + S[id];
            }
            read_field_unit(i,j,S[id++]);
        }
        
        int M = (int)strips.size();
        for (int i=0; i<M; i++) strips[i].fill_cells();
        sort(strips.begin(), strips.end(), strip_sort);
        for (int i=0; i<M; i++) for (int j=i+1; j<M; j++) {
            psii p = intersection(strips[i],strips[j]);
            if (p.fs != -1) {
                //strips[i].crosses.push_back(mp(j,p));
                strips[j].crosses.push_back(mp(i,mp(p.sc,p.fs)));
            }
        }
        
        vint P = {0,1,2,3,4,5,6,7,8,9};
        do {
            if (try_find_solution(P)) {
                ull n = NumberUtils::from_digits(P);
                cout << T << " " << n << " valid attempts: " << attempts << endl;
                ans += n;
                break;
            }
        } while (next_permutation(P.begin(),P.end()));
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
