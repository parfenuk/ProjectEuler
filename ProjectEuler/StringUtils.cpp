//
//  StringUtils.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/3/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

// ********** STRING METHODS **********

namespace StringUtils
{

// TODO: z-function, KMP-function, some other stuff...

// simpler version works 1.5 times slower...
vstring parse_by_symbol (const string &S, char p)
{
    vstring v;
    size_t found = 0, first = 0;
    while (first != S.length() && S[first] == p) first++;
    if (first == S.length()) return v;
    
    while (true) {
        
        found = S.find(p,first);
        if (found == string::npos) { v.push_back(S.substr(first, S.length()-first)); break; }
        v.push_back(S.substr(first, found-first));
        while (found != S.length() && S[found] == p) found++;
        if (found == S.length()) break;
        first = found;
    }
    return v;
}

void trimSymbol (string &S, char p) {
    
    // remove trailing symbols
    while (!S.empty() && S.back() == p) S.pop_back();
    // remove leading symbols
    int i = 0; while (i < (int)S.length() && S[i] == p) i++;
    S.erase(0,i);
}

bool hasPrefix (const string &S, const string &prefix) {
    return S.length() >= prefix.length() && S.substr(0,prefix.length()) == prefix;
}

bool hasSuffix (const string &S, const string &suffix) {
    return S.length() >= suffix.length() && S.substr(S.length()-suffix.length(),suffix.length()) == suffix;
}

bool isNumericalString (const string &S) {
    int dots = 0;
    for (int i=0; i<(int)S.length(); i++) {
        if (i == 0 && S[i] == '-') continue;
        if (S[i] == '.') { dots++; if (dots > 1) return false; }
        else if (S[i] < '0' || S[i] > '9') return false;
    }
    return true;
}

bool same_letters (string s, string u)
{
    if (s.length() != u.length()) return false;
    
    sort(s.begin(), s.end());
    sort(u.begin(), u.end());
    
    for (int i=0; i<(int)s.length(); i++) if (s[i] != u[i]) return false;
    return true;
}

vector<pii> Manacher (const string &s)
{
    int n = (int)s.length();
    
    vector<pii> d(n);
    
    int l = 0, r = -1;
    for (int i=0; i<n; i++) {
        int k = i > r ? 1 : min(d[l+r-i].fs, r-i+1);
        while (i+k < n && i-k >= 0 && s[i+k] == s[i-k]) k++;
        d[i].fs = k;
        if (i+k-1 > r) {
            l = i-k+1;
            r = i+k-1;
        }
    }
    
    l = 0; r = -1;
    for (int i=0; i<n; i++) {
        int k = i > r ? 0 : min(d[l+r-i+1].sc, r-i+1);
        while (i+k < n && i-k-1 >= 0 && s[i+k] == s[i-k-1]) k++;
        d[i].sc = k;
        if (i+k-1 > r) {
            l = i-k;
            r = i+k-1;
        }
    }
    
    return d;
}


vint prefix_function (const string &s)
{
    int n = (int)s.length();
    vint pi(n);
    for (int i=1; i<n; i++) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j]) j = pi[j-1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

vint suffix_array (const string& s, const int alphabet_size = 26, bool reversed = false)
{
    int n0 = (int)s.length();
    if (n0 == 0) return {};
    
    int n = n0 + 1;                         // + sentinel
    const int alphabet = alphabet_size + 1; // by default: 0 = sentinel, 1..26 = 'a'..'z'
    
    vint p(n), cnt(max(n, alphabet)), c(n);
    vint a(n); // here char to int mapping is changing depending on alphabet
    for (int i=0; i<n0; i++) a[i] = reversed ? 26-s[i]+'a' : (s[i]-'a') + 1;
    a[n0] = 0;
    
    for (int i=0; i<n; i++) cnt[a[i]]++;
    for (int i=1; i<alphabet; i++) cnt[i] += cnt[i-1];
    for (int i=0; i<n; i++) p[--cnt[a[i]]] = i;
    
    c[p[0]] = 0;
    int classes = 1;
    for (int i=1; i<n; i++) {
        if (a[p[i]] != a[p[i-1]]) classes++;
        c[p[i]] = classes-1;
    }
    
    vint pn(n), cn(n);
    for (int h=0; (1<<h)<n; h++) {
        for (int i=0; i<n; i++) {
            pn[i] = p[i] - (1<<h);
            if (pn[i] < 0)  pn[i] += n;
        }
        
        cnt.assign(classes,0);
        for (int i=0; i<n; i++) cnt[c[pn[i]]]++;
        for (int i=1; i<classes; i++) cnt[i] += cnt[i-1];
        for (int i=n-1; i>=0; i--) p[--cnt[c[pn[i]]]] = pn[i];
        
        cn[p[0]] = 0;
        classes = 1;
        for (int i=1; i<n; i++) {
            int mid1 = p[i] + (1<<h);
            if (mid1 >= n) mid1 -= n;
            int mid2 = p[i-1] + (1<<h);
            if (mid2 >= n) mid2 -= n;
            if (c[p[i]] != c[p[i-1]] || c[mid1] != c[mid2])
                ++classes;
            cn[p[i]] = classes-1;
        }
        c.swap(cn);
        if (classes == n) break;
    }
    
    // p[0] == n0 (sentinel), remove it
    p.erase(p.begin());
    return p;
}

}
