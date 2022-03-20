//
//  Brus.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 20.03.22.
//  Copyright © 2022 Miraslau Parafeniuk. All rights reserved.
//

#include "Brus.hpp"
#include "Algebra.hpp"
#include "NumberUtils.hpp"

namespace Brus
{

int N;

sint fc (char c) { if ('0' <= c && c <= '9') return c-'0'; return c-'A'+10; } // 'A' -> 10
char tc (sint n) { if (n < 10) return '0'+n; return 'A'+n-10; }               // 12 -> 'C'
string ts (sint cnt, sint n) { string s; for (int i=0; i<cnt; i++) s += tc(n); return s; }

bool next_string (string &S)
{
    if (S.back() == tc(N-1)) return false;
    for (int i=0; i<(int)S.length(); i++) {
        if (S[i] != '9') S[i]++;
        else S[i] = 'A';
    } return true;
}

struct FN { // Forbidden number (vpsii form)
    vector<psii> A; // < digit, count_it_appears > - size is at most 6
    psii operator[] (int k) { return A[k]; }
    int size() const { return (int)A.size(); }
    FN() {}
    FN(const vector<psii> &B) { A = B; }
    FN(const string &S) { // S is non-empty
        A.push_back(mp(fc(S[0]),1));
        for (int i=1; i<(int)S.length(); i++) {
            if (S[i] == S[i-1]) A.back().sc++;
            else A.push_back(mp(fc(S[i]),1));
        }
    }
    void show() {
        for (int i=0; i<size(); i++) for (int j=0; j<A[i].sc; j++) cout << tc(A[i].fs); cout << endl;
    }
};

const sint inf = 15;

struct Brus { // size is N always, maybe reduce it in future
    vsint low;
    vsint high;
    Brus() { low = vsint(N); high = vsint(N,inf); }
    void show() {
        cout << "("; Containers::show(low,0); cout << ") - (";
        Containers::show(high,0); cout << ")\n";
    }
};

//bool Dominating (const Brus &a, const Brus &b) // true if a >> b
//{
//    for (int i=0; i<N; i++) {
//        if (a[i] < b[i]) return false;
//    } return true;
//}
//
//bool Dominating (const list<Brus> &L, const Brus &b) // true if a >> b
//{
//    for (list<Brus>::const_iterator it=L.begin(); it!=L.end(); it++) {
//        if (Dominating(*it,b)) return true;
//    } return false;
//}
//
//void clean_dominated (list<Brus> &L, const Brus &b)
//{
//    for (list<Brus>::const_iterator it=L.begin(); it!=L.end(); ) {
//        if (Dominating(b,*it)) it = L.erase(it);
//        else it++;
//    }
//}

bool relation_FN (const Brus &B, FN &a)
{
    for (int i=0; i<a.size(); i++) {
        if (B.high[a[i].fs] < a[i].sc) return false; // outside
    } return true; // has common points inside
}

vector<Brus> split (const Brus &B, FN &a) // a-spray and B have common points here
{
    vector<Brus> res;
    Brus br = B;
    for (int i=0; i<a.size(); i++) {
        if (br.low[a[i].fs] < a[i].sc) {
            br.high[a[i].fs] = a[i].sc-1;
            res.push_back(br);
            br.low[a[i].fs] = a[i].sc;
            br.high[a[i].fs] = B.high[a[i].fs];
        }
    }
    return res;
}

vector<string> get_patterns (const Brus &B, psii range)
{
    if (range.fs == range.sc) {
        if (B.high[range.fs] == inf) { string s; s = s + tc(range.fs) + '*'; return {s}; }
        vector<string> S(1,ts(B.low[range.fs],range.fs));
        for (int i=B.low[range.fs]+1; i<=B.high[range.fs]; i++) S.push_back(S.back() + tc(range.fs));
        return S;
    }
    sint M = (range.fs + range.sc) / 2;
    vector<string> pr = get_patterns(B,mp(range.fs,M));
    vector<string> sf = get_patterns(B,mp(M+1,range.sc));
    vector<string> res;
    for (int i=0; i<(int)pr.size(); i++) for (int j=0; j<(int)sf.size(); j++) {
        res.push_back(pr[i] + sf[j]);
    }
    return res;
}

bool contains_point (const Brus &B, const vint &a)
{
    for (int i=1; i<N; i++) {
        if (a[i] < B.low[i] || a[i] > B.high[i]) return false;
    } return true;
}

bool check_intersections (const list<Brus> &L) {
    for (ull n=1; n<Algebra::powmod(inf+1,N-1); n++) {
        vector<int> d = NumberUtils::digits(n,inf+1,N-1);
        int contain_count = 0;
        for (list<Brus>::const_iterator it=L.begin(); it!=L.end(); it++) {
            if (contains_point(*it,d)) contain_count++;
        }
        if (contain_count > 1) return false;
    } return true;
}

void calculate_patterns (int base, const vector<pair<string,sint>> &r)
{
//    vector<FN> R;
//    for (int i=0; i<(int)S.size(); i++) {
//        do {
//            R.push_back(FN(S[i]));
//        } while (next_string(S[i]));
//    } // TODO: experiment with sorting if too slow
    
    vector<FN> R;
    for (int i=0; i<(int)r.size(); i++) {
        string S = r[0].fs;
        R.push_back(FN(S));
        for (int j=1; j<r[0].sc; j++) {
            next_string(S);
            R.push_back(FN(S));
        }
    }
    
    list<Brus> L;
    L.push_back(Brus());
    
    for (int n=0; n<(int)R.size(); n++) {
        
        vector<Brus> splitted;
        for (list<Brus>::iterator it=L.begin(); it!=L.end(); ) {
            if (!relation_FN(*it,R[n])) { it++; continue; }
            // R[n] is inside current Brus, perform split operation
            vector<Brus> a = split(*it,R[n]);
            splitted.insert(splitted.end(),a.begin(),a.end()); // direct insertion faster/slower?
            it = L.erase(it);
        }
        
        for (int i=0; i<(int)splitted.size(); i++) L.push_back(splitted[i]);
//        R[n].show();
//        for (list<Brus>::iterator it=L.begin(); it!=L.end(); it++) it->show();
//        if (!check_intersections(L)) {
//            cout << "ALARM!\n";
//        }
    }
    
    int patterns = 0;
    cout << "\nTotal bruses: " << L.size() << endl;
    //for (list<Brus>::iterator it=L.begin(); it!=L.end(); it++) it->show();
    
    //vector<string> ANS;
    for (list<Brus>::iterator it=L.begin(); it!=L.end(); it++) {
        vector<string> S = get_patterns(*it,mp(1,N-1));
        patterns += (int)S.size();
        //for (int i=0; i<(int)S.size(); i++) cout << S[i] << endl;
    }
    cout << "Total patterns: " << patterns << endl;
}

}
