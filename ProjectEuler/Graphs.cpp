//
//  Graphs.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/3/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

typedef vector<vector<int>> graph; // represented by adjancecy matrix
typedef vector<vector<pii>> weightedGraph;

namespace Graphs
{

vvint find_connected_components (const graph &g, int N)
{
    vvint w;
    
    vbool used(N);
    for (int v=0; v<N; v++) {
        
        if (used[v]) continue;
            
        vint component;
        int h = 0, t = 0;
        component.push_back(v);
        t++;
        used[v] = true;
        while (h < t) {
            int cur = component[h++];
            for (int i=0; i<(int)g[cur].size(); i++) {
                int to = g[cur][i];
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

}
