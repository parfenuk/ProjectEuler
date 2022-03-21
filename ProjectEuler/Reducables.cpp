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

bool next_string (string &S)
{
    if (S.back() == tc(N-1)) return false;
    for (int i=0; i<(int)S.length(); i++) {
        if (S[i] != '9') S[i]++;
        else S[i] = 'A';
    } return true;
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

vector<string> merge_strings_array (const vector<string> &p, const vector<string> &s)
{
    vector<string> w;
    for (int i=0; i<(int)p.size(); i++) for (int j=0; j<(int)s.size(); j++) {
        w.push_back(p[i] + s[j]);
    } return w;
}

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
//        if (S[ps] > v[pv]) { res.push_back(S[ps++]); }
//        else if (S[ps] < v[pv]) pv++;
        if (S[ps] > v[pv]) pv++;
        else if (S[ps] < v[pv]) { res.push_back(S[ps++]); }
        else { ps++; pv++; }
    }
    return res;
}

// n - first digit, ids - subset of FN, digit must be non-empty, no further dfs
vector<string> get_final_patterns (int n, vector<int> ids, bool first=true, bool last=false)
{
    if (ids.empty()) { // no any restrictions
        string ret;
        for (int i=n; i<N; i++) {
            ret += tc(i);
            ret += '*';
        } return { ret }; // n*(n+1)*...(N-1)*
    }
    
    if (last || n == N-1) { // no deeper dfs
        sint max_digits_count = INF;
        for (int i=0; i<(int)ids.size(); i++) {
            if (R[ids[i]].last_non_zero < n) return {};
            if (R[ids[i]].last_non_zero == n &&
                R[ids[i]][n] < max_digits_count) max_digits_count = R[ids[i]][n];
        }
        string ret;
        if (max_digits_count == INF) { ret += tc(n); ret += '*'; return { ret }; }
        else return vector<string>(Prefs[n].begin(),Prefs[n].begin()+max_digits_count);
    }
    
    vector<vector<int>> used_cnt_indeces(15); // auxiliary
    for (int i=0; i<(int)ids.size(); i++) {
        if (R[ids[i]].last_non_zero < n) return {};
        used_cnt_indeces[R[ids[i]][n]].push_back(ids[i]);
    }
    vector<int> cnt_indeces;
    for (int i=0; i<15; i++) if (!used_cnt_indeces[i].empty()) cnt_indeces.push_back(i);

    vector<pii> ranges;
    if (cnt_indeces.size() == 1) {
        if (cnt_indeces[0] == 0) ranges = { mp(0,INF) };
        else ranges = { mp(0,cnt_indeces[0]-1), mp(cnt_indeces[0],INF) };
    }
    else {
        if (cnt_indeces[0]) ranges.push_back(mp(0,cnt_indeces[0]-1));
        for (int i=1; i<(int)cnt_indeces.size(); i++) ranges.push_back(mp(cnt_indeces[i-1],cnt_indeces[i]-1));
        ranges.push_back(mp(cnt_indeces.back(),INF));
    }
    
    if (ranges.size() == 1) { // partial case
        vector<string> suffixes = get_final_patterns(n+1,ids);
        return merge_strings_array(ranges[0],n,suffixes);
    }
    
    vector<string> res;
    vector<int> new_ids;
    vector<string> star_suffixes = get_final_patterns(n+1,ids,0,1), suffixes;
    //bool star = (star_suffixes.size()==1 && star_suffixes[0][1]=='*');
    //sint max_len = (sint)star_suffixes.size()-1;
    for (int i=0; i<(int)ranges.size(); i++) {
        for (int j=ranges[i].fs; j<=min(ranges[i].sc,14); j++) {
            new_ids.insert(new_ids.end(),used_cnt_indeces[j].begin(),used_cnt_indeces[j].end());
        }
        vector<string> w;
        if (i+1 == (int)ranges.size()) w = merge_strings_array(ranges[i],n,star_suffixes);
        else {
            // HOW THIS SHIT WORKS! prefixes are sorted randomly!!! :D
            suffixes = get_final_patterns(n+1,new_ids);
            sort(suffixes.begin(), suffixes.end());
            suffixes = minus_set_strings(suffixes,star_suffixes);
            w = merge_strings_array(ranges[i],n,suffixes);
            // WHY THIS SHIT DOESN'T WORK?!
//            suffixes = get_final_patterns(n+1,new_ids);
//            vector<string> real_suffixes;
//            for (int j=0; j<(int)suffixes.size(); j++) {
//                if (star && suffixes[j] == star_suffixes[0]) continue;
//                if (!star && (suffixes[j].empty() || (suffixes[j].back() == tc(n+1) && suffixes[j].length() <= max_len))) continue;
//                real_suffixes.push_back(suffixes[j]);
//            }
//            w = merge_strings_array(prefixes, real_suffixes);
        }
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

    R.clear();
    for (int i=0; i<(int)r.size(); i++) R.push_back(FN(r[i]));
    sort(R.begin(), R.end());

    cout << "Reducables count: " << R.size() << endl;
    
    vint ids; for (int i=0; i<(int)R.size(); i++) ids.push_back(i);
    vector<string> ans = get_final_patterns(1,ids);
    reverse(ans.begin(), ans.end());
//    ofstream out("output.txt");
//    for (int i=0; i<(int)ans.size(); i++) out << ans[i] << endl;
    cout << "Patterns Count: " << ans.size() << endl;
    
    return ans;
}

}
