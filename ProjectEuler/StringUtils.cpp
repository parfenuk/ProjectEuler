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

vstring parse_by_symbol2 (const string &S, char p)
{
    vstring v;
    string T;
    
    for (int i=0; i<(int)S.length(); i++) {
        if (S[i] != p) T += S[i];
        else {
            if (!T.empty()) v.push_back(T);
            T.clear();
        }
    } if (!T.empty()) v.push_back(T);
    
    return v;
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
