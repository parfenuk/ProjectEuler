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

#include "Generator.cpp"

using namespace Algebra;
using namespace Containers;

string to_json (const vector<psii> &pts)
{
    string S = "{ \"rows\": "; S += to_string(Grid::N);
    S += ", \"columns\": "; S += to_string(Grid::M);
    S += ", \"pairs\": [";
    for (int i=0; i<(int)pts.size(); i++) {
        S += "[";
        int r1 = Grid::cell[pts[i].fs].fs, c1 = Grid::cell[pts[i].fs].sc;
        int r2 = Grid::cell[pts[i].sc].fs, c2 = Grid::cell[pts[i].sc].sc;
        S += to_string(r1); S += ",";
        S += to_string(c1); S += ",";
        S += to_string(r2); S += ",";
        S += to_string(c2); S += "]";
        if (i+1 == (int)pts.size()) S += "]";
        else S += ",";
    }
    
    S += ", \"colorIds\": [";
    vector<int> clrs;
    while (clrs.size() < pts.size()) {
        int n = (int)Utils::random_integer(1,max(12,(int)pts.size()));
        if (contains(clrs,n)) continue;
        clrs.push_back(n);
    }
    
    for (int i=0; i<(int)clrs.size(); i++) {
        S += to_string(clrs[i]);
        if (i+1 == (int)pts.size()) S += "]";
        else S += ",";
    }
    
    S += " },";
    
    return S;
}

string json_to_plist (string S)
{
    if (S.back() == ',') S.pop_back();
    return "<string>" + S + "</string>";
}

// path is on 10x10 board
pair<pii,vint> get_enclosing_rect (const vint &path) // < <N,M>, new_path >
{
    int min_row = 10, max_row = -1;
    int min_col = 10, max_col = -1;
    
    for (int i=0; i<(int)path.size(); i++) {
        int r = path[i] / 10, c = path[i] % 10;
        if (r < min_row) min_row = r;
        if (r > max_row) max_row = r;
        if (c < min_col) min_col = c;
        if (c > max_col) max_col = c;
    }
    
    int n = max_row - min_row + 1;
    int m = max_col - min_col + 1;
    vint a;
    for (int i=0; i<(int)path.size(); i++) {
        int r = path[i] / 10, c = path[i] % 10;
        r -= min_row; c -= min_col;
        a.push_back(r*m + c);
    }
    return mp(mp(n,m),a);
}

struct Level {
    int N, M;
    pii s, e;
    vector<psii> forbidden_cells;
    int difficulty;
    void show() {
        cout << N << " " << M << endl;
        cout << "(" << s.fs << " " << s.sc << ") -> (";
        cout << e.fs << " " << e.sc << ")";
        cout << "\nforbidden: ";
        for (int i=0; i<(int)forbidden_cells.size(); i++) {
            cout << "(" << forbidden_cells[i].fs << " " << forbidden_cells[i].sc << ") ";
        }
        cout << endl;
    }
    
    string plist_json_value() {
        string S = "{ \"rows\": "; S += to_string(N);
        S += ", \"columns\": "; S += to_string(M);
        S += ", \"pairs\": [[";
        S += to_string(s.fs); S += ",";
        S += to_string(s.sc); S += ",";
        S += to_string(e.fs); S += ",";
        S += to_string(e.sc); S += "]], \"colorIds\": [";
        S += to_string(Utils::random_integer(1,5));
        S += "], \"forbiddenCells\": [";
        for (int i=0; i<(int)forbidden_cells.size(); i++) {
            S += to_string(forbidden_cells[i].fs);
            S += ",";
            S += to_string(forbidden_cells[i].sc);
            S += ",";
        }
        S.pop_back();
        S += "]}";
        
        return S;
    }
};

Level generate_random_level()
{
    Grid::set_dimensions(10,10);
    
    int s, e;
    while (true) {
        s = (int)Utils::random_integer(0,99);
        e = (int)Utils::random_integer(0,99);
        psii c1 = Grid::cell[s], c2 = Grid::cell[e];
        if (abs(c1.fs - c2.fs) + abs(c1.sc - c2.sc) > 1) break;
    }
    
    vint p;
    while (true) {
        p = Grid::get_random_path(s,e);
        if (!p.empty()) break;
    }
    pair<pii,vint> R = get_enclosing_rect(p);
    if (R.fs.sc - R.fs.fs != 0 && R.fs.sc - R.fs.fs != 1) {
        Level l;
        l.N = l.M = 0;
        return l;
    }
    Grid::set_dimensions(R.fs.fs, R.fs.sc);
    vint ids = Grid::generate_forbidden_cells(R.sc);
    vector<psii> cells = Grid::ids_to_cells(ids);
    
    Level l;
    l.N = R.fs.fs;
    l.M = R.fs.sc;
    l.s = Grid::cell[R.sc[0]];
    l.e = Grid::cell[R.sc.back()];
    l.forbidden_cells = cells;
    l.difficulty = (int)p.size();
    
    return l;
}

bool dif_sort (const Level &a, const Level &b) { return a.difficulty < b.difficulty; }

int main() {
    clock_t Total_Time = clock();
    cout.precision(3);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    vector<Level> levels[13];
    vector<int> level_sizes = {12,12,12,12,12,12,12,11,11,11,11,11,11};
    vector<pii> level_dimensions = { mp(4,4), mp(4,5), mp(5,5), mp(5,6), mp(6,6), mp(6,7), mp(7,7), mp(7,8), mp(8,8), mp(8,9), mp(9,9), mp(9,10), mp(10,10) };
    
    while (ans != 150) {
        Level l = generate_random_level();
        for (int i=0; i<13; i++) {
            if (l.N == level_dimensions[i].fs && l.M == level_dimensions[i].sc && levels[i].size() < level_sizes[i]) {
                cout << l.N << " " << l.M << endl;
                levels[i].push_back(l);
                ans++;
                break;
            }
        }
        //cout << l.plist_json_value() << endl;
    }
    
    vector<Level> L;
    for (int i=0; i<13; i++) for (int j=0; j<level_sizes[i]; j++) {
        L.push_back(levels[i][j]);
    }
    sort(L.begin(), L.end(), dif_sort);
    
    ofstream out("output.txt");
    for (int i=0; i<ans; i++) {
        out << "<string>" << L[i].plist_json_value() << "</string>" << endl;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
