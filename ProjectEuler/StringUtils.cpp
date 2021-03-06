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

}
