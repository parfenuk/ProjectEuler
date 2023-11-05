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

void generate_from_single_element_in_list (int n)
{
    vector<string> ret(n);
    string S;
    for (int i=0; i<n; i++) {
        getline(cin,S);
        ret[i] = "\"" + S + "\" = ";
    }
    for (int i=0; i<n; i++) {
        getline(cin,S);
        add_escapings(S);
        ret[i] += "\"" + S + "\";";
    }
    for (int i=0; i<n; i++) cout << ret[i] << endl;
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

    string S;
    while (getline(cin,S)) {
        size_t found = S.find("?");
        if (found == string::npos) { cout << S << endl; continue; }
        string Q = S.substr(0,found+1);
        string A = S.substr(found+1,S.length()-found-1);
        if (A[0] == ' ') A.erase(0,1);
        add_escapings(Q);
        cout << "[\"" << Q << "\"," << "\"" << A << "\"],\n";
    }
    
//    string S;
//    map<string,string> M;
//    int K = 1;
//    while (getline(cin,S)) {
//        size_t br = S.find("\" = \"");
//        if (br == string::npos) {
//            if ((StringUtils::hasPrefix(S, "//") && K == 2)) cout << S << endl;
//            K++;
//            continue;
//        }
//        
//        string key = S.substr(1,br-1);
//        string value = S.substr(br+5,S.length()-br-7);
//        
//        if (M.find(key) == M.end()) {
//            M[key] = value;
//            add_escapings(value);
//            cout << "\"" << key << "\" = \"" << value << "\";\n";
//        }
        
        //cout << key << " = " << value << endl;
//        if (M.find(key) != M.end()) {
//            if (M[key] != value) cout << K << ": " << key << " - same value\n";
//            else cout << K << ": old: " << M[key] << ", new: " << value << endl;
//        } else {
//            M[key] = value;
//        }
        
        //K++;
//        for (int i=0; i<(int)keys.size(); i++) {
//            if (!StringUtils::hasPrefix(S,keys[i]+'"')) continue;
//            size_t c = S.find("\"");
//            c = S.find("\"", c+1);
//            string s1 = keys[i];
//            string s2 = S.substr(c+1,S.length()-c-1);
//            add_escapings(s2);
//            cout << "\"" << keys[i] << "\" = \"" << s2 << "\";\n";
//        }
    //}
    
    //Total_Time = clock() - Total_Time;
    //cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
