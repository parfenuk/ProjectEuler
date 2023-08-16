#include "Definitions.hpp"

#include "Algebra.hpp"
#include "Combinatorics.cpp"
#include "Complex.cpp"
// That's why we include .cpp here despite of having header. Ugly workaround
// https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
#include "Containers.cpp"
#include "Geometry.cpp"
#include "Geometry3D.cpp"
#include "Fractions.cpp"
#include "Lnum.hpp"
#include "Matrix.cpp"
#include "NumberUtils.hpp"
#include "Pell_Equation.cpp"
#include "StringUtils.cpp"
#include "Utils.cpp"

using namespace Algebra;
using namespace Containers;

u32string utf8ToUtf32 (const string &utf8) {
    
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cnv;
    u32string utf32 = cnv.from_bytes(utf8);
    if (cnv.converted() < utf8.size())
        throw std::runtime_error("utf8ToUtf32: Could not convert the following string: \"" + utf8 + "\"\n");
    return utf32;
}

string utf32ToUtf8 (const u32string &utf32) {
    
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cnv;
    string utf8 = cnv.to_bytes(utf32);
    if (cnv.converted() < utf32.size())
        throw std::runtime_error("utf32ToUtf8: could not convert string from utf32 to utf8\n");
    return utf8;
}

void print32 (const u32string &str) {
    
    wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
    cout << converter.to_bytes(str) << endl;
}

void add_escapings (string &S) {
    for (int i=(int)S.length()-1; i>=0; i--) {
        if (S[i] == '"') {
            S.insert(i,"\\");
        }
    }
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","rt",stdin);
    freopen("output.txt","wt",stdout);
#endif
        
    //ifstream in("Hints.csv");
    string S;
    while (getline(cin,S)) {
        size_t hint = S.find("_hint");
        if (hint == string::npos || S[0] != 'q') { cout << "ALARM!"; return 0; }
        int id = atoi(S.substr(1,hint-1).c_str());
        string s1 = S.substr(0,hint+5);
        string s2 = S.substr(hint+9,S.length()-hint-9);
        add_escapings(s2);
        cout << "\"q" << id << "_hint\" = \"" << s2 << "\";\n";
    }
    
    //Total_Time = clock() - Total_Time;
    //cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
