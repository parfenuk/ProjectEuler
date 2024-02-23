//
//  Generator.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 20.02.24.
//  Copyright © 2024 Miraslau Parafeniuk. All rights reserved.
//

#include <stdio.h>
#include "Solver.cpp"

namespace Generator {

int N, M;
vbool F; // own field
vvint G; // own graph

void set_dimensions (int n, int m)
{
    N = n; M = m;
    F = vbool(N*M);
    Grid::set_dimensions(n,m);
    G = Grid::G;
}

vvsint find_connected_components (const vsint &initial_component) // using Field
{
    vvsint w;
    
    vbool used(N*M, true);
    for (int i=0; i<(int)initial_component.size(); i++) {
        int v = initial_component[i];
        if (F[v] == false) used[v] = false;
    }
    
    for (int v=0; v<N*M; v++) {
        
        if (used[v]) continue;
        
        vsint component;
        int h = 0, t = 0;
        component.push_back(v);
        t++;
        used[v] = true;
        while (h < t) {
            int cur = component[h++];
            for (int i=0; i<(int)G[cur].size(); i++) {
                int to = G[cur][i];
                if (used[to]) continue;
                used[to] = true;
                component.push_back(to);
                t++;
            }
        }
        w.push_back(component);
    }
    
    return w;
}

vsint get_random_path (sint from, sint to) // using field
{
    vint p = {0,1,2,3};
    queue<sint> q;
    q.push(from);
    vsint prev(N*M,-1);
    for (int i=0; i<N*M; i++) {
        if (F[i]) prev[i] = -2; // inaccessible
    } prev[from] = -3; // start point
    
    while (!q.empty()) {
        sint v = q.front();
        if (v == to) break;
        q.pop();
        
        shuffle(p.begin(), p.end(), Utils::rd);
        for (int i=0; i<4; i++) {
            if (G[v].size() <= p[i]) continue;
            int u = G[v][p[i]];
            if (prev[u] != -1) continue;
            prev[u] = v;
            q.push(u);
        }
    }
    
    vsint path(1,to);
    do {
        path.push_back(prev[path.back()]);
    } while (path.back() != from);
    
    return path;
}

vector<psii> generate_single_points_set (const vsint &component, int max_points_count)
{   // TODO: hardcode all values for small components
    if (component.size() <= 12) return Solver::any_solution_on_grid(component);
    int n = (int)component.size();
    
    vector<psii> points;
    
    vsint p;
    while (p.empty()) {
        int id1 = (int)Utils::random_integer(0,n-1);
        int id2 = (int)Utils::random_integer(0,n-1);
        if (id1 == id2 || Grid::is_neighbours(component[id1], component[id2])) continue;
        p = get_random_path(component[id1], component[id2]);
        points.push_back(mp(component[id1], component[id2]));
    }
    
    for (int i=0; i<(int)p.size(); i++) F[p[i]] = 1; // draw path
    
    vvsint w = find_connected_components(component);
    for (int i=0; i<(int)w.size(); i++) {
        vector<psii> a = generate_single_points_set(w[i], max_points_count - (int)points.size());
        if (a.empty()) return vector<psii>();
        Containers::append_to(points,a);
        if (points.size() > max_points_count) return vector<psii>();
    }
    
    return points;
}

vector<pair<vector<psii>,vsint>> generate_levels (int desired_levels_count, int max_points_count = 19)
{
    vector<pair<vector<psii>,vsint>> levels;
    vector<sint> full_component;
    for (int i=0; i<N*M; i++) full_component.push_back(i);
    int cnt = 0;
    ll cnt_generation_success = 0, cnt_total = 0;
    
    while (cnt != desired_levels_count) {
        cnt_total++;
        for (int i=0; i<N*M; i++) F[i] = 0;
        
        vector<psii> pts = generate_single_points_set(full_component, max_points_count);
        if (!pts.empty()) {
            cnt_generation_success++;
            vsint solution = Solver::solve_graph(N,M,pts);
            if (!solution.empty()) {
                levels.push_back(mp(pts,solution));
                cnt++;
            }
        }
    }
    
    cout << "Total: " << cnt_total << ", succeed: " << cnt_generation_success << ", ";
    return levels;
}

}
