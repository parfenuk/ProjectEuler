//
//  Solver.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 18.02.24.
//  Copyright © 2024 Miraslau Parafeniuk. All rights reserved.
//

#include <stdio.h>
#include "Grid.cpp"

namespace Solver {

using namespace Containers;

int N;
vvint G;
vsint path;
vsint path_indeces; // index of corresponsing edge in the path from G
int current_path_id;
vsint color_map; // which path id occupies current cell
vsint focus_point_type; // +n: begin, -n: end, 0: ordinary
int total_focus_points_count;
int focus_points_visited = 0;

bool step_up (int id_from)
{
    int v = path.back();
    for (int i=id_from; i<(int)G[v].size(); i++) {
        int to = G[v][i];
        int tv = focus_point_type[v];
        int tt = focus_point_type[to];
        
        if (color_map[to] == color_map[v] && (path.size() < 2 || path[path.size()-2] != to) && tv == 0) return false;
        
        if (color_map[to]) continue;
        
        if (tv >= 0 && tt != 0 && current_path_id+tt) continue;
        
        path.push_back(to);
        //path_indeces.push_back(i);
        if (tt) { focus_points_visited++; }
        if (tt > 0) current_path_id = tt;
        color_map[to] = current_path_id;
        return true;
    }
    return false;
}

int step_back()
{
    if (path.size() == 1) { return -1; }
    int v = path[path.size()-2], to = path.back();//, id = path_indeces.back();
    path.pop_back();
    //path_indeces.pop_back();
    color_map[to] = 0;
    if (focus_point_type[to]) { focus_points_visited--; }
    if (focus_point_type[v] < 0) { current_path_id = -focus_point_type[v]; }
    return index_of_object(G[v], to);
}

vsint backtrack (int initial_vertice, bool debug = false) // find Hamiltonian cycle
{
    path.clear();
    path.reserve(N);
    path_indeces.reserve(N);
    path.push_back(initial_vertice);
    color_map[initial_vertice] = focus_point_type[initial_vertice];
    focus_points_visited = 1;
    current_path_id = focus_point_type[initial_vertice];
    
    bool non_full_solution_found = false;
    int id_from = 0;
    vsint solution;
    
    clock_t Time = clock();
    while (true) {
        if (clock() - Time > 60000000) return vsint();
        
        bool q = step_up(id_from);
        if (q) {
            //cout << "Up: "; show(path);
            id_from = 0;
            if (path.size() == N) {
                if (debug) Containers::show(path);
                if (!solution.empty()) return vsint(); //Ambiguity
                solution = path;
            }
            else if (focus_points_visited == total_focus_points_count && !non_full_solution_found) {
                non_full_solution_found = true;
                if (debug) Containers::show(path);
                return vsint(); //Ambiguity
            }
        }
        else {
            id_from = step_back();
            //cout << "Back: "; show(path);
            if (id_from == -1) break;
            id_from++;
        }
    }
    
    return solution.empty() ? vsint() : solution; // No solution or Unique
}

vsint solve_graph (int n, int m, vector<psii> points) // n - rows, m - columns
{
    N = n*m;
    color_map = vsint(N);
    focus_point_type = vsint(N);
    total_focus_points_count = 2*(int)points.size();
    
    Grid::assign_field();
    Grid::build_graph();
    G = Grid::G;
    
    for (int i=0; i<(int)points.size(); i++) {
        focus_point_type[points[i].fs] = i+1;
        focus_point_type[points[i].sc] = -i-1;
        int k = i+1; if (k == (int)points.size()) k = 0;
        G[points[i].sc] = { points[k].fs };
    }
    
    return backtrack(points[0].fs);
}

vsint solve_graph (int n, int m, vvsint points)
{
    vector<psii> pts;
    for (int i=0; i<points.size(); i++) {
        int r1 = points[i][0], c1 = points[i][1];
        int r2 = points[i][2], c2 = points[i][3];
        pts.push_back(mp(r1*m+c1,r2*m+c2));
    }
    return solve_graph(n, m, pts);
}

vsint solve_grid_graph (vsint free_points, vector<psii> points)
{
    N = (int)free_points.size();
    
    Grid::build_graph();
    G = Grid::G;
    
    color_map = vsint(Grid::N*Grid::M);
    focus_point_type = vsint(Grid::N*Grid::M);
    total_focus_points_count = 2*(int)points.size();
    
    for (int i=0; i<(int)points.size(); i++) {
        focus_point_type[points[i].fs] = i+1;
        focus_point_type[points[i].sc] = -i-1;
        int k = i+1; if (k == (int)points.size()) k = 0;
        G[points[i].sc] = { points[k].fs };
    }
    
    return backtrack(points[0].fs);
}

vector<psii> any_solution_on_grid (vsint free_points) // always connected
{
    int n = (int)free_points.size();
    if (n < 3) return vector<psii>();
    int kMax = n < 6 ? 2 : (n < 9 ? 4 : 6);
    
    Grid::assign_field(Grid::INACCESSIBLE);
    for (int i=0; i<n; i++) {
        Grid::F[free_points[i]] = 0;
    }
    
    for (int k=2; k<=kMax; k+=2) {
        
        vvint pairs;
        vector<vvchar> P = Combinatorics::fill_partitions(k);
        for (int i=0; i<(int)P.size(); i++) {
            vvchar partition = P[i];
    
            bool all_pairs = true;
            for (int j=0; j<(int)partition.size(); j++) {
                if (partition[j].size() != 2) { all_pairs = false; break; }
            } if (!all_pairs) continue;
    
            vint new_pair;
            for (int j=0; j<(int)partition.size(); j++) {
                new_pair.push_back(partition[j][0]-1);
                new_pair.push_back(partition[j][1]-1);
            } pairs.push_back(new_pair);
        } P.clear(); P.shrink_to_fit();
        
        vvsint A = Combinatorics::get_combinations(n,k);
        shuffle(A.begin(), A.end(), Utils::rd);
        
        for (int i=0; i<(int)A.size(); i++) {
            for (int j=0; j<(int)pairs.size(); j++) {
                vsint B;
                for (int m=0; m<k; m++) B.push_back(free_points[A[i][pairs[j][m]]-1]);
                
                if (!Grid::is_good_pts_set(B)) continue;
                vsint solution = solve_grid_graph(free_points, Containers::to_pairs(B));
    
                if (!solution.empty()) {
                    return Containers::to_pairs(B);
                }
            }
        }
    }
    
    return vector<psii>();
}

}
