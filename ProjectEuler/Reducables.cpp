//
//  Reducables.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 20.03.22.
//  Copyright © 2022 Miraslau Parafeniuk. All rights reserved.
//

#include "Reducables.hpp"

namespace Reducables
{

int N;
const sint INF = 2022;
vector<string> Prefs[36];

sint fc (char c) { if ('0' <= c && c <= '9') return c-'0'; return c-'A'+10; } // 'A' -> 10
char tc (sint n) { if (n < 10) return '0'+n; return 'A'+n-10; }               // 12 -> 'C'
string ts (sint cnt, sint n) { string s; for (int i=0; i<cnt; i++) s += tc(n); return s; }

void fill_prefs()
{
    for (int i=1; i<N; i++) {
        Prefs[i].clear();
        Prefs[i].push_back("");
        for (int j=1; j<=15; j++) Prefs[i].push_back(Prefs[i].back()+tc(i));
        Prefs[i].push_back(Prefs[i][0]+tc(i)+'*');
    }
}

struct FN { // Forbidden number
    vsint A;
    sint last_non_zero;
    sint operator[] (int k) const { return A[k]; }
    FN() { A = vsint(N); }
    FN(const vsint &B) { A = B; }
    FN(const string &S) {
        A = vsint(N);
        for (int i=0; i<(int)S.length(); i++) A[fc(S[i])]++;
        last_non_zero = fc(S.back());
    }
};

bool operator< (const FN &a, const FN &b)
{
    for (int i=0; i<N; i++) {
        if (a[i] < b[i]) return true;
        if (a[i] > b[i]) return false;
    } return false;
}

vector<FN> R;

vector<string> merge_strings_array (pii r, int n, const vector<string> &s)
{
    vector<string> w;
    if (r.sc == INF) r = mp(16,16);
    for (int i=r.fs; i<=r.sc; i++) for (int j=0; j<(int)s.size(); j++) {
        w.push_back(Prefs[n][i] + s[j]);
    } return w;
}

vector<string> minus_set_strings (const vector<string> &S, const vector<string> &v) // S - v
{
    vector<string> res;
    
    int ps = 0, pv = 0;
    while (ps != (int)S.size() || pv != (int)v.size()) {
        if (ps == (int)S.size()) break;
        if (pv == (int)v.size()) { res.push_back(S[ps++]); continue; }
        if (S[ps] > v[pv]) pv++;
        else if (S[ps] < v[pv]) { res.push_back(S[ps++]); }
        else { ps++; pv++; }
    }
    return res;
}

ll id_check = 0;            // 1274645636
ll id_insertion = 0;        //  488350079
int pattern_insertion = 0;  //    1972430

vvint used_cnt_indeces[37];
vint cnt_indeces[37];
vector<sint> max_digits_count(37,INF);

void clearance (int n)
{
    for (int i=0; i<15; i++) used_cnt_indeces[n][i].clear();
    cnt_indeces[n].clear();
    max_digits_count[n] = INF;
}

// n - first digit, ids - subset of FN, 'last' if no further dfs
vector<string> get_final_patterns (int n, bool empty_ids, bool last=false, bool should_return_empty = false)
{
    if (last || n == N-1) { // no deeper dfs
        if (should_return_empty) return {};
        string ret;
        if (max_digits_count[n] == INF) { ret += tc(n); ret += '*'; return { ret }; }
        else return vector<string>(Prefs[n].begin(),Prefs[n].begin()+max_digits_count[n]);
    }
    if (empty_ids) { // no any restrictions
        string ret;
        for (int i=n; i<N; i++) {
            ret += tc(i);
            ret += '*';
        } return { ret }; // n*(n+1)*...(N-1)*
    }
    if (should_return_empty) return {};

    vector<psii> ranges;
    if (cnt_indeces[n].size() == 1) {
        if (cnt_indeces[n][0] == 0) ranges = { mp(0,INF) };
        else ranges = { mp(0,cnt_indeces[n][0]-1), mp(cnt_indeces[n][0],INF) };
    }
    else {
        if (cnt_indeces[n][0]) ranges.push_back(mp(0,cnt_indeces[n][0]-1));
        for (int i=1; i<(int)cnt_indeces[n].size(); i++) ranges.push_back(mp(cnt_indeces[n][i-1],cnt_indeces[n][i]-1));
        ranges.push_back(mp(cnt_indeces[n].back(),INF));
    }
    
    if (ranges.size() == 1) { // partial case, (0,INF) range
        // fill n+1 info
        clearance(n+1);
        empty_ids = true;
        for (int j=ranges[0].fs; j<=14; j++) {
            //id_insertion += (int)used_cnt_indeces[n][j].size();
            for (int i=0; i<(int)used_cnt_indeces[n][j].size(); i++) {
                int id = used_cnt_indeces[n][j][i];
                if (R[id].last_non_zero < n+1) { should_return_empty = true; break; }
                used_cnt_indeces[n+1][R[id][n+1]].push_back(id);
                empty_ids = false;
                if (R[id].last_non_zero == n+1 &&
                    R[id][n+1] < max_digits_count[n+1]) max_digits_count[n+1] = R[id][n+1];
            }
            if (should_return_empty) break;
        }
        if (!should_return_empty) for (int i=0; i<15; i++) if (!used_cnt_indeces[n+1][i].empty()) cnt_indeces[n+1].push_back(i);
        
        vector<string> suffixes = get_final_patterns(n+1,empty_ids,last,should_return_empty);
        return merge_strings_array(ranges[0],n,suffixes);
    }
    
    vector<string> res;
    max_digits_count[n+1] = INF;
    for (int j=0; j<=14; j++) {
        //id_insertion += (int)used_cnt_indeces[n][j].size();
        for (int i=0; i<(int)used_cnt_indeces[n][j].size(); i++) {
            int id = used_cnt_indeces[n][j][i];
            if (R[id].last_non_zero < n+1) { should_return_empty = true; break; }
            if (R[id].last_non_zero == n+1 &&
                R[id][n+1] < max_digits_count[n+1]) max_digits_count[n+1] = R[id][n+1];
        }
        if (should_return_empty) break;
    }
    vector<string> star_suffixes = get_final_patterns(n+1,empty_ids,true,should_return_empty);
    bool star = (star_suffixes.size()==1 && star_suffixes[0][1]=='*');
    bool empty = false;
    bool any_id_added = false;
    should_return_empty = false;
    clearance(n+1);
    
    for (int r=0; r<(int)ranges.size(); r++) {
        vector<string> w;
        if (r+1 == (int)ranges.size()) w = merge_strings_array(ranges[r],n,star_suffixes);
        else {
            if (empty) { if (star_suffixes.empty()) break; continue; }
            for (int j=ranges[r].fs; j<=ranges[r].sc && j<=14; j++) {
                //id_insertion += (int)used_cnt_indeces[n][j].size();
                // TODO: use the fact R is sorted
                for (int i=0; i<(int)used_cnt_indeces[n][j].size(); i++) {
                    int id = used_cnt_indeces[n][j][i];
                    if (R[id].last_non_zero < n+1) { should_return_empty = true; break; }
                    used_cnt_indeces[n+1][R[id][n+1]].push_back(id);
                    any_id_added = true;
                    if (R[id].last_non_zero == n+1 &&
                        R[id][n+1] < max_digits_count[n+1]) max_digits_count[n+1] = R[id][n+1];
                }
                if (should_return_empty) break;
            }
            if (!should_return_empty) {
                cnt_indeces[n+1].clear(); // TODO: alternative option: use set
                for (int i=0; i<15; i++) if (!used_cnt_indeces[n+1][i].empty()) cnt_indeces[n+1].push_back(i);
            }
            
            vector<string> suffixes = get_final_patterns(n+1,!any_id_added,last,should_return_empty);
            if (suffixes.empty()) { empty = true; continue; }
            if (star_suffixes.empty()) {
                w = merge_strings_array(ranges[r],n,suffixes);
            }
            else { // erase strings that contained in star_suffixes
                vector<string> real_suffixes;
                for (int j=0; j<(int)suffixes.size(); j++) {
                    if (star && suffixes[j] == star_suffixes[0]) continue;
                    if (!star && ((suffixes[j].empty() || suffixes[j].back() == tc(n+1)) && suffixes[j].length() < star_suffixes.size())) continue;
                    real_suffixes.push_back(suffixes[j]);
                }
                w = merge_strings_array(ranges[r],n,real_suffixes);
            }
        }
        //pattern_insertion += (int)w.size();
        res.insert(res.end(),w.begin(),w.end());
    }
    
    return res;
}

// OPTIMIZATION 1: dfs from 1 to N-1 manually
// OPTIMIZATION 2: restructured FN, only 6 different digits max
// OPTIMIZATION 3: take into account *, i.e. no many digits after it
// OPTIMIZATION 4: look attentively at FNs with only 2 different digits
// OPTIMIZATION 5: only patterns with * can be ambiguos
vector<string> calculate_patterns (int base, const vector<string> &r)
{
    N = base;
    fill_prefs();
    for (int i=1; i<=N; i++) used_cnt_indeces[i] = vvint(15);

    R.clear();
    for (int i=0; i<(int)r.size(); i++) R.push_back(FN(r[i]));
    sort(R.begin(), R.end());

    cout << "Reducables count: " << R.size() << endl;
    
    //vint ids; for (int i=0; i<(int)R.size(); i++) ids.push_back(i);
    for (int i=0; i<(int)R.size(); i++) {
        used_cnt_indeces[1][R[i][1]].push_back(i);
    }
    for (int i=0; i<15; i++) if (!used_cnt_indeces[1][i].empty()) cnt_indeces[1].push_back(i);
    vector<string> ans = get_final_patterns(1,false);
    reverse(ans.begin(), ans.end());
//    ofstream out("output.txt");
//    for (int i=0; i<(int)ans.size(); i++) out << ans[i] << endl;
    cout << "Patterns Count: " << ans.size() << endl;
    
//    cout << "ID check: " << id_check << endl;
//    cout << "ID insertion: " << id_insertion << endl;
//    cout << "Pattern insertion: " << pattern_insertion << endl;
    
    return ans;
}

}
