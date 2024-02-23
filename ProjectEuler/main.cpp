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

struct LevelBeauty {
    sint points_count;
    dd av_dist;
    sint rotations_count;
    
    LevelBeauty() { points_count = av_dist = rotations_count = 0; }
    LevelBeauty(sint pc, dd av, sint rc) {
        points_count = pc;
        av_dist = av;
        rotations_count = rc;
    }
};

LevelBeauty get_beauty (const vector<psii> &pts, const vsint &solution)
{
    LevelBeauty beauty;
    beauty.points_count = pts.size();
    
    for (int i=0; i<(int)pts.size(); i++) {
        psii cell1 = Grid::cell[pts[i].fs];
        psii cell2 = Grid::cell[pts[i].sc];
        beauty.av_dist += fabs(cell1.fs-cell2.fs) + fabs(cell1.sc-cell2.sc);
    }
    beauty.av_dist /= pts.size();
    
    sint prev_direction = -1; // 0 - up, 1 - down, 2 - left, 3 - right
    for (int i=1; i<(int)solution.size(); i++) {
        bool is_prev_pair = contains_pair(pts, solution[i-1]);
        bool is_cur_pair = contains_pair(pts, solution[i]);
        if (is_prev_pair && is_cur_pair) continue; // it's path jump change
        
        psii prev = Grid::cell[solution[i-1]];
        psii cur = Grid::cell[solution[i]];
        sint cur_direction = -1;
        if (prev.fs == cur.fs + 1) cur_direction = 0;
        else if (prev.fs == cur.fs - 1) cur_direction = 1;
        else if (prev.sc == cur.sc + 1) cur_direction = 2;
        else if (prev.sc == cur.sc - 1) cur_direction = 3;
        
        if (is_prev_pair) prev_direction = cur_direction; // path begin, ignore it
        else {
            if (cur_direction != prev_direction) beauty.rotations_count++;
            prev_direction = cur_direction;
        }
    }
    
    return beauty;
}

bool beauty_sort (const pair<vector<psii>,vsint> &a, const pair<vector<psii>,vsint> &b) {
    LevelBeauty b1 = get_beauty(a.fs, a.sc);
    LevelBeauty b2 = get_beauty(b.fs, b.sc);
    
    if (b1.points_count < b2.points_count) return true;
    if (b1.points_count > b2.points_count) return false;
    if (b1.av_dist < b2.av_dist) return true;
    if (b1.av_dist > b2.av_dist) return false;
    return b1.rotations_count < b2.rotations_count;
}

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

ofstream out("output.txt");
// desired_config[i] determines how many levels with exactly i points is needed
void generate_for_dimensions (int N, int M, vector<int> desired_config)
{
    Generator::set_dimensions(N,M);
    
    int first_non_zero = 0;
    while (desired_config[first_non_zero] == 0) { first_non_zero++; }
    int last_non_zero = (int)desired_config.size() - 1;
    while (desired_config[last_non_zero] == 0) { last_non_zero--; }
    int sum = total_vector_sum(desired_config);
    const int F = first_non_zero;
    
    vector<pair<vector<psii>,vsint>> obtained_levels;
    
    cout << "Work for N = " << N << ", M = " << M << " has been started\n";
    clock_t tt = clock();
    while (sum) {
        vector<pair<vector<psii>,vsint>> levels = Generator::generate_levels(1,last_non_zero);
        int n = (int)levels[0].fs.size();
        if (n < first_non_zero) {
            if (n < F) cout << "Generated Super!!! " << n << endl;
            else cout << "Generated extra " << n << endl;
            desired_config[first_non_zero]--;
            sum--;
            while (first_non_zero < (int)desired_config.size() && desired_config[first_non_zero] == 0) { first_non_zero++; }
            obtained_levels.push_back(levels[0]);
        }
        if (n < (int)desired_config.size() && desired_config[n]) {
            cout << "Generated " << n << endl;
            desired_config[n]--;
            sum--;
            while (last_non_zero >= 0 && desired_config[last_non_zero] == 0) { last_non_zero--; }
            obtained_levels.push_back(levels[0]);
        }
    }
    
    sort(obtained_levels.begin(), obtained_levels.end(), beauty_sort);
    for (int i=0; i<(int)obtained_levels.size(); i++) {
        out << to_json(obtained_levels[i].fs) << endl;
    }
    
    tt = clock() - tt;
    cout << "Work for N = " << N << ", M = " << M << " has been finished in ";
    cout << ((float)tt)/CLOCKS_PER_SEC << " seconds\n";
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(3);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
//    string S;
//    while (getline(cin,S)) {
//        if (S[0] != '{') continue;
//        cout << json_to_plist(S) << endl;
//    } return 0;
    
    ull ans = 0;
    
    generate_for_dimensions(4, 5,   {0,0,0,6,6,3});
    generate_for_dimensions(5, 5,   {0,0,0,2,5,5,3});
    generate_for_dimensions(5, 6,   {0,0,0,0,3,5,5,2});
    generate_for_dimensions(6, 6,   {0,0,0,0,1,3,6,5});
    generate_for_dimensions(6, 7,   {0,0,0,0,0,1,3,6,4});
    generate_for_dimensions(7, 7,   {0,0,0,0,0,0,1,3,5,5});
    generate_for_dimensions(7, 8,   {0,0,0,0,0,0,0,3,3,3,3});
    generate_for_dimensions(8, 8,   {0,0,0,0,0,0,0,0,2,3,4,1});
    generate_for_dimensions(8, 9,   {0,0,0,0,0,0,0,0,1,2,3,4});
    generate_for_dimensions(9, 9,   {0,0,0,0,0,0,0,0,0,1,2,3,4});
    generate_for_dimensions(9, 10,  {0,0,0,0,0,0,0,0,0,0,0,3,7});
    generate_for_dimensions(10, 10, {0,0,0,0,0,0,0,0,0,0,0,0,10});
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
