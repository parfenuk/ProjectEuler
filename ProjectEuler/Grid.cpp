//
//  Grid.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 18.02.24.
//  Copyright © 2024 Miraslau Parafeniuk. All rights reserved.
//

#include <stdio.h>
#include "Definitions.hpp"

namespace Grid {

const sint INACCESSIBLE = -100;

int N, M;
int cellID[10][10];
vector<psii> cell; // from row and column
vint F; // field
vvint G; // neighbours graph

void assign_field (int k = 0)
{
    for (int i=0; i<N*M; i++) F[i] = k;
}

void build_graph()
{
    G = vvint(N*M);
    for (int i=0; i<N; i++) for (int j=0; j<M; j++) {
        int id = cellID[i][j];
        if (F[id] == INACCESSIBLE) continue;
        if (i != 0 && F[id-M] == 0) G[id].push_back(id-M); // upper cell
        if (i != N-1 && F[id+M] == 0) G[id].push_back(id+M); // lower cell
        if (j != 0 && F[id-1] == 0) G[id].push_back(id-1); // left cell
        if (j != M-1 && F[id+1] == 0) G[id].push_back(id+1); // right cell
    }
}

void set_dimensions (int n, int m)
{
    N = n; M = m;
    cell = vector<psii>(N*M);
    for (int i=0; i<n; i++) for (int j=0; j<m; j++) {
        int id = i*m + j;
        cellID[i][j] = id;
        cell[id] = mp(i,j);
    }
    F = vint(N*M);
    build_graph();
}

vector<psii> ids_to_cells (const vint &a)
{
    vector<psii> ret;
    for (int i=0; i<(int)a.size(); i++) ret.push_back(cell[a[i]]);
    return ret;
}

vsint cells_to_ids (vector<psii> &a)
{
    vsint ret;
    for (int i=0; i<(int)a.size(); i++) ret.push_back(cellID[a[i].fs][a[i].sc]);
    return ret;
}

bool is_neighbours (int a, int b)
{
    return abs(cell[a].fs - cell[b].fs) + abs(cell[a].sc - cell[b].sc) == 1;
}

// here N and M are 10
vint get_random_path (int s, int e)
{
    vsint p = {0,1,2,3};
    vint path;
    path.push_back(s);
    
    while (true) {
        
        shuffle(p.begin(), p.end(), Utils::rd);
        bool ok = false;
        
        for (int z=0; z<(int)p.size(); z++) {
            
            if (G[path.back()].size() <= p[z]) continue;
            int next = G[path.back()][p[z]];
            
            bool good = true;
            for (int i=0; i<(int)path.size()-1; i++) {
                if (is_neighbours(next, path[i])) { good = false; break; }
            }
            
            if (good) { path.push_back(next); ok = true; break; }
        }
        
        if (!ok) break;
        
        if (is_neighbours(path.back(), e)) { path.push_back(e); return path; }
    }
    
    return {};
}

// but we have inaccessible cells here
vint try_get_path (int s, int e)
{
    vbool used(N*M);
    vint from(N*M, -1);
    used[s] = true;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int v = q.front();
        if (v == e) break;
        q.pop();
        
        for (int i=0; i<(int)G[v].size(); i++) {
            int to = G[v][i];
            if (F[to] == INACCESSIBLE || used[to]) continue;
            
            used[to] = true;
            from[to] = v;
            q.push(to);
        }
    }
    
    if (from[e] == -1) return {};
    vint path(1,e);
    while (path.back() != s) {
        path.push_back(from[path.back()]);
    }
    return path; // reversed, but it's ok
}

// here N and M are already determined
vint generate_forbidden_cells (const vint &path)
{
    for (int i=1; i<(int)path.size()-1; i++) F[path[i]] = 1; // original path id
    for (int i=1; i<(int)path.size()-1; i++) {
        F[path[i]] = INACCESSIBLE;
        while (true) {
            vint p = try_get_path(path[0], path.back());
            if (p.empty()) break;
            vint filtered;
            for (int i=1; i<(int)p.size()-1; i++) {
                if (F[p[i]] != 1) filtered.push_back(p[i]);
            }
            F[Utils::random_element(filtered)] = INACCESSIBLE;
        }
        F[path[i]] = 1;
    }
    
    vint cells;
    for (int i=0; i<(int)F.size(); i++) {
        if (F[i] == INACCESSIBLE) cells.push_back(i);
    }
    return cells;
}

vsint flipH (const vsint &a)
{
    vsint b(a.size());
    for (int i=0; i<(int)a.size(); i++) {
        sint r = cell[a[i]].fs, c = cell[a[i]].sc;
        r = N-1-r;
        b[i] = (r*M+c);
    } sort(b.begin(), b.end()); return b;
}

vsint flipV (const vsint &a)
{
    vsint b(a.size());
    for (int i=0; i<(int)a.size(); i++) {
        sint r = cell[a[i]].fs, c = cell[a[i]].sc;
        c = M-1-c;
        b[i] = (r*M+c);
    } sort(b.begin(), b.end()); return b;
}

vsint mirror (const vsint &a)
{
    vsint b(a.size());
    for (int i=0; i<(int)a.size(); i++) {
        sint r = cell[a[i]].fs, c = cell[a[i]].sc;
        r = N-1-r; c = M-1-c;
        b[i] = (r*M+c);
    } sort(b.begin(), b.end()); return b;
}

vsint cw90 (const vsint &a)
{
    vsint b(a.size());
    for (int i=0; i<(int)a.size(); i++) {
        sint r = cell[a[i]].fs, c = cell[a[i]].sc;
        sint r1 = N-1-c, c1 = r;
        b[i] = (r1*N+c1);
    } sort(b.begin(), b.end()); return b;
}

vsint ccw90 (const vsint &a)
{
    vsint b(a.size());
    for (int i=0; i<(int)a.size(); i++) {
        sint r = cell[a[i]].fs, c = cell[a[i]].sc;
        sint r1 = N-1-c, c1 = r;
        b[i] = (r1*N+c1);
    } sort(b.begin(), b.end()); return b;
}

vsint transpose (const vsint &a)
{
    vsint b(a.size());
    for (int i=0; i<(int)a.size(); i++) {
        sint r = cell[a[i]].fs, c = cell[a[i]].sc;
        sint r1 = c, c1 = r;
        b[i] = (r1*N+c1);
    } sort(b.begin(), b.end()); return b;
}

bool is_less_or_equal (const vsint &a, const vsint &b)
{
    for (int i=0; i<(int)a.size(); i++) {
        if (a[i] < b[i]) return true;
        if (a[i] > b[i]) return false;
    } return true;
}

bool is_normal_form (const vsint &a)
{
    if (!is_less_or_equal(a, mirror(a))) return false;
    if (!is_less_or_equal(a, flipV(a))) return false;
    if (!is_less_or_equal(a, flipH(a))) return false;
    
    if (N == M) {
        if (!is_less_or_equal(a, cw90(a))) return false;
        if (!is_less_or_equal(a, ccw90(a))) return false;
        if (!is_less_or_equal(a, transpose(a))) return false;
    }
    
    return true;
}

bool is_good_pts_set (const vsint &a)
{
    for (int i=0; i<(int)a.size(); i+=2) {
        if (is_neighbours(a[i],a[i+1])) return false;
    } return true;
}

}
