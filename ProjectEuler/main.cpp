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

string to_json (int N, int M, vector<psii> pts)
{
    string S = "{ \"rows\": "; S += to_string(N);
    S += ", \"columns\": "; S += to_string(M);
    S += ", \"pairs\": [";
    for (int i=0; i<(int)pts.size(); i++) {
        S += "[";
        int r1 = pts[i].fs/M, c1 = pts[i].fs%M;
        int r2 = pts[i].sc/M, c2 = pts[i].sc%M;
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
        int n = (int)Utils::random_integer(1,12);
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

//pair<bool, vector<psii>> try_get_pts (int N, int M, const vsint &a)
//{
//    vector<psii> pts;
//    sint total_dist = 0;
//    for (int i=0; i<(int)a.size(); i+=2) {
//        if (is_neighbours(a[i],a[i+1])) return mp(false, pts);
//        total_dist += abs(a[i]/M - a[i+1]/M);
//        total_dist += abs(a[i]%M - a[i+1]%M);
//        if (total_dist > N*M) return mp(false, pts);
//        pts.push_back(mp(a[i],a[i+1]));
//    } return mp(true, pts);
//}

//void generate_all_levels (int N, int M, int k)
//{
//    ofstream out("output.txt");
//    
//    Grid::set_dimensions(N,M);
//    
//    vvint pairs;
//    vector<vvchar> P = Combinatorics::fill_partitions(k);
//    for (int i=0; i<(int)P.size(); i++) {
//        vvchar partition = P[i];
//        
//        bool all_pairs = true;
//        for (int j=0; j<(int)partition.size(); j++) {
//            if (partition[j].size() != 2) { all_pairs = false; break; }
//        } if (!all_pairs) continue;
//        
//        vint new_pair;
//        for (int j=0; j<(int)partition.size(); j++) {
//            new_pair.push_back(partition[j][0]-1);
//            new_pair.push_back(partition[j][1]-1);
//        } pairs.push_back(new_pair);
//    } P.clear(); P.shrink_to_fit();
//        
//    int cnt = 0;
//    vsint a;
//    for (sint i=1; i<=k; i++) a.push_back(i);
//    while (Combinatorics::next_combination(a,N*M,k)) {
//        
//        vsint A(k); for (int i=0; i<k; i++) A[i] = a[i]-1;
//        
//        if (!is_normal_form(A)) continue;
//        
//        for (int j=0; j<(int)pairs.size(); j++) {
//            vsint B;
//            for (int m=0; m<k; m++) B.push_back(A[pairs[j][m]]);
//            pair<bool, vector<psii>> p = try_get_pts(N,M,B);
//            if (!p.fs) continue;
//            string S = Solver::solve_graph(N,M,p.sc);
//            
//            if (S == "Unique") {
//                cnt++;
//                cout << cnt << " ";
//                out << to_json(N,M,p.sc) << endl;
//            }
//        }
//    }
//    cout << endl << "Total: " << cnt << endl;
//}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
//    string S;
//    while (getline(cin,S)) {
//        if (S[0] != '{') continue;
//        if (S.back() == ',') S.pop_back();
//        cout << "<string>" << S << "</string>" << endl;
//    } return 0;
    
    ull ans = 0;
    
    Generator::set_dimensions(10,10);
    vector<vector<psii>> levels = Generator::generate_levels(10);
    for (int i=0; i<(int)levels.size(); i++) {
        show_pair(levels[i]);
    }
    
//    Grid::set_dimensions(6,6);
//    vsint pts = { 0,1,2,6,7,8,12,13,14 };
//    vector<psii> a = Solver::any_solution_on_grid(pts);
//    show_pair(a);
    
//    Grid::set_dimensions(10,10);
//    cout << Solver::solve_graph(10,10, {{1,1,6,8},{2,6,8,8},{2,7,6,7},{3,0,9,8},{3,3,5,1},{3,4,6,5},{5,0,5,4},{6,2,9,7},{8,1,8,7},{8,2,8,4}});
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
