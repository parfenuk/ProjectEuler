//
//  Archivator.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 22.03.22.
//  Copyright © 2022 Miraslau Parafeniuk. All rights reserved.
//

#include "Archivator.hpp"

namespace Archivator
{

template <class Trzx>
void show (const vector<Trzx> &a, bool show_endl=true, bool show_size=false)
{
    if (show_size) cout << a.size() << endl;
    for (int i=0; i<(int)a.size(); i++) cout << a[i] << " ";
    if (show_endl) cout << endl;
}

sint N;
const int INF = 19;

sint fc (char c) { if ('0' <= c && c <= '9') return c-'0'; return c-'A'+10; } // 'A' -> 10
char tc (sint n) { if (n < 10) return '0'+n; return 'A'+n-10; }               // 12 -> 'C'
string ts (sint cnt, sint n) { string s; for (int i=0; i<cnt; i++) s += tc(n); return s; }

struct SP { // simple pattern
    vector<sint> s;   // symbols
    vector<sint> cnt; // count of corresponding symbol in s, INF if *
    vector<sint> repeats; // if non-empty - corresponds to patterns family
    sint len; // number of symbols in string form, ex. len(13*4*) = 5
    bool has_star;
    bool full;
    SP() { len = has_star = full = 0; }
    SP(const string &S) {
        len = (sint)S.length();
        has_star = full = false;
        for (int i=0; i<len; i++) {
            if (S[i] == '*') { cnt.back() = INF; has_star = true; }
            else {
                sint n = fc(S[i]);
                if (!s.empty() && n == s.back()) cnt.back()++;
                else { s.push_back(n); cnt.push_back(1); }
            }
        }
    }
    bool flat() { // repeats has form { k k+1 ... k+p }
        for (int i=1; i<(int)repeats.size(); i++) {
            if (repeats[i] != repeats[i-1]+1) return false;
        } return true;
    }
    string string_form() {
        string F;
        for (int i=0; i<(int)s.size(); i++) {
            char c = tc(s[i]);
            F += c;
            if (cnt[i] == INF) F += '*';
            else for (int j=1; j<cnt[i]; j++) F += c;
        } return F;
    }
};

bool SP_basic_sort (const SP &a, const SP &b)
{
    return a.s.size() < b.s.size() || (a.s.size() == b.s.size() && a.len < b.len);
}

sint dif (const SP &a, const SP &b) // dif(112*3*,445*6*) = 3, other cases = -1
{
    if (a.s.size() != b.s.size() || a.len != b.len) return -1;
    sint d = -1;
    for (int i=0; i<(int)a.s.size(); i++) {
        if (a.cnt[i] != b.cnt[i]) return -1;
        sint e = b.s[i] - a.s[i];
        if (d == -1) d = e;
        else if (d != e) return -1;
    } return d;
}

bool is_dominated_by (const SP &a, const SP &b) // a < b ? 1 : 0
{
    if (a.s.size() > b.s.size()) return false;
    int pa = 0, pb = 0;
    while (pa != (int)a.s.size() || pb != (int)b.s.size()) {
        if (pa == (int)a.s.size()) return true;
        if (pb == (int)b.s.size()) return false;
        if (a.s[pa] < b.s[pb]) return false;
        if (a.s[pa] > b.s[pb]) pb++;
        else {
            if ((a.cnt[pa] == INF) ^ (b.cnt[pb] == INF)) return false;
            if (a.cnt[pa] > b.cnt[pb]) return false;
            pa++; pb++;
        }
    } return true;
}

struct FP { // Family Pattern
    vsint s; // symbols
    vector<set<sint>> r; // ranges. r.fs == r.sc if only one count is possible
    vsint repeats;
    bool full;
    FP() {}
    FP(const SP &a) {
        s = a.s;
        r = vector<set<sint>>(a.s.size());
        for (int i=0; i<(int)a.cnt.size(); i++) r[i].insert(a.cnt[i]);
        repeats = a.repeats;
        full = a.full;
    }
    bool nest (const SP &a) { // true if succeeds
        if (full != a.full) return false;
        if (repeats != a.repeats) return false;
        if (s != a.s) return false;
        sint dif_digit = 0, dif_index = 0, dif_value = 0;
        for (int i=0; i<(int)s.size(); i++) {
            if (r[i].size() != 1) {
                if (dif_digit) return false;
                if (a.cnt[i] == INF) return false;
                dif_digit = s[i];
                dif_index = i;
                dif_value = a.cnt[i];
            }
            else {
                int c = *r[i].begin();
                if (c == a.cnt[i]) continue;
                //if (c < a.cnt[i]) return false;
                if (dif_digit) return false;
                dif_digit = s[i];
                dif_index = i;
                dif_value = a.cnt[i];
            }
        }
        r[dif_index].insert(dif_value);
        return true;
    }
    bool nest (const FP &a) {
        if (full != a.full) return false;
        if (repeats != a.repeats) return false;
        if (s != a.s) return false;
        sint dif_digit = 0, dif_index = 0;
        for (int i=0; i<(int)s.size(); i++) {
            if (r == a.r) continue;
            if (dif_digit) return false;
            dif_digit = s[i];
            dif_index = i;
        }
        r[dif_index].insert(a.r[dif_index].begin(),a.r[dif_index].end());
        return true;
    }
};



vector<string> get_archived_patterns (const vector<string> &S)
{
    N = fc(S.back()[2])+1; // 2*3* -> 4
    vector<SP> v;
    for (int i=0; i<(int)S.size(); i++) v.push_back(SP(S[i]));
    
    vector<SP> indecreasables;
    for (int i=0; i<(int)v.size(); i++) {
        bool ok = true;
        for (int j=0; j<(int)indecreasables.size(); j++) {
            sint d = dif(indecreasables[j],v[i]);
            if (d == -1) continue;
            indecreasables[j].repeats.push_back(d);
            ok = false;
            break;
        }
        if (ok) indecreasables.push_back(v[i]);
    }
    cout << "Indecreasables: " << indecreasables.size() << endl;
    vector<SP> full_indecreasables, not_full_indecs;
    for (int i=0; i<(int)indecreasables.size(); i++) {
        if (indecreasables[i].s.back() + indecreasables[i].repeats.size() + 1 == N) {
            indecreasables[i].full = true;
            full_indecreasables.push_back(indecreasables[i]);
        }
        else not_full_indecs.push_back(indecreasables[i]);
        //else if (!indecreasables[i].flat()) { cout << indecreasables[i].string_form() << " "; show(indecreasables[i].repeats); }
    }
    cout << "Full indecreasables: " << full_indecreasables.size() << endl;
    
    vector<FP> F;
    int iteration = 1;
    while (true) {
        bool changed = false;
        if (F.empty()) { // SP nest
            changed = true;
            for (int i=0; i<(int)full_indecreasables.size(); i++) {
                bool nested = false;
                for (int j=0; j<(int)F.size(); j++) {
                    if (F[j].nest(full_indecreasables[i])) { nested = true; break; }
                }
                if (!nested) F.push_back(FP(full_indecreasables[i]));
            }
            for (int i=0; i<(int)not_full_indecs.size(); i++) {
                bool nested = false;
                for (int j=0; j<(int)F.size(); j++) {
                    if (F[j].nest(not_full_indecs[i])) { nested = true; break; }
                }
                if (!nested) F.push_back(FP(not_full_indecs[i]));
            }
        }
        else { // FP nest
            vector<bool> erased(F.size());
            for (int i=0; i<(int)F.size(); i++) {
                if (erased[i]) continue;
                for (int j=0; j<(int)F.size(); j++) {
                    if (i == j) continue;
                    if (erased[j]) continue;
                    if (F[j].nest(F[i])) { erased[i] = true; changed = true; break; }
                }
            }
            vector<FP> G;
            for (int i=0; i<(int)F.size(); i++) {
                if (!erased[i]) G.push_back(F[i]);
            } F.swap(G);
        }
        cout << "Iteration #" << iteration << ": " << F.size() << " families\n";
        iteration++;
        if (!changed) break;
    }
    
//    sort(v.begin(),v.end(),SP_basic_sort);
//    vector<int> canonic_patterns_ids;
//    //for (int i=0; i<(int)v.size(); i++) cout << v[i].string_form() << endl;
//    for (int i=0; i<(int)v.size(); i++) {
//        bool reducable = false;
//        for (int j=0; j<i; j++) {
//            if (is_dominated_by(v[j],v[i])) { reducable = true; break; }
//        }
//        if (!reducable) canonic_patterns_ids.push_back(i);
//    }
//
//    vector<string> ret;
//    cout << "Irreducible: " << canonic_patterns_ids.size() << endl;
//    for (int i=0; i<(int)canonic_patterns_ids.size(); i++) {
//        int id = canonic_patterns_ids[i];
//        ret.push_back(v[id].string_form());
//        //cout << ret.back() << endl;
//    }
//    return ret;
    
    return {};
}

}
