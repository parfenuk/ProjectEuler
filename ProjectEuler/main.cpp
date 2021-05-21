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

void research_names_and_lastnames()
{
    ull ans = 0;
    
    msi names, lastnames;
    
    for (int i=0; i<17686; i++) {
        string S;
        getline(cin,S);
        vector<string> v = StringUtils::parse_by_symbol(S, ' ');
        if (i < 30) {
            cout << v[0] << endl;
            for (int j=0; j<(int)v[0].size(); j++) cout << v[0][j] << " "; cout << endl;
        }
        if (v.size() == 1) names[v[0]]++;
        if (v.size() == 2) { names[v[0]]++; lastnames[v[1]]++; }
        if (v.size() == 3) { names[v[1]]++; lastnames[v[2]]++; }
    }
    
    cout << names.size() << " " << lastnames.size() << endl;
    int k = 1;
    for (msi::iterator it=names.begin(); it!=names.end(); it++) {
        // 44 most frequent names, 85.4% of all
        if (it->sc >= 100) { cout << k++ << ". " << it->fs << " " << it->sc << endl; }
    }
    k = 1;
    for (msi::iterator it=lastnames.begin(); it!=lastnames.end(); it++) {
        // 40 most frequent lastnames, 82.3% of all
        if (it->sc >= 70) { cout << k++ << ". " << it->fs << " " << it->sc << endl; ans += it->sc; }
    }
    
    cout << ans << endl;
}

// *** NAIVE DETECTION PHONE NUMBERS ***
//        for (int i=0; i<=(int)S.length()-17; i++) {
//            string T = S.substr(i,17);
//            if (isPhoneNumber(T)) pn.push_back(T);
//        }

bool isDigit (const char &p) { return p >= '0' && p <= '9'; }
int digitsCount (const string &S) {
    int d = 0; for (int i=0; i<(int)S.length(); i++) if (isDigit(S[i])) d++; return d;
}
bool allSymbolsOK (const string &S, const vint &poses, char p) {
    for (int i=0; i<(int)poses.size(); i++) {
        if (S.length() <= poses[i] || S[poses[i]] != p) return false;
    } return true;
}
void erasePositions (string &S, const vint &poses) {
    for (int i=(int)poses.size()-1; i>=0; i--) S.erase(poses[i],1);
}
void eraseAll (string &S, char p) { // all 'p' are erased
    size_t found = 0;
    while (true) {
        found = S.find(p,found);
        if (found == string::npos) break;
        S.erase(found,1);
    }
}
void replaceAll (string &S, char p, char q) { // all 'p' are replaced by q
    size_t found = 0;
    while (true) {
        found = S.find(p,found);
        if (found == string::npos) break;
        S.replace(found,1,1,q);
    }
}
bool to_pn_format (string &S) // to phone number format
{
    // can be 12 digits - starts with 375 (375291234567)
    // can be 11 digits - starts with 80  (80291234567)
    // can be 10 digits - starts with 0   (0291234567)
    // can be 9 digits - starts with 29 or 33 or 25 or 17 (291234567)
    string T;
    for (int i=0; i<(int)S.length(); i++) if (isDigit(S[i])) T += S[i];
    if (T.length() == 12) {
        if (T.substr(0,3) == "375") { S = "+" + T; return true; }
        return false;
    }
    if (T.length() == 11) {
        if (T.substr(0,2) == "80") { S = "+375" + T.substr(2,9); return true; }
        return false;
    }
    if (T.length() == 10) {
        string code = T.substr(0,3);
        if (code=="017" || code=="025" || code=="029" || code=="033") {
            S = "+375" + T.substr(1,9); return true;
        }
        return false;
    }
    if (T.length() == 9) {
        string code = T.substr(0,2);
        if (code=="17" || code=="25" || code=="29" || code=="33") {
            S = "+375" + T; return true;
        }
        return false;
    }
    return false;
}

bool isPhoneNumber (string &S) // return value has correct format +375291234567
{
    // let's try to organize common pattern
    size_t fd = S.find_first_of("0123456789"); // first digit
    int digits_count = digitsCount(S);
    if (fd != string::npos &&
        (digits_count >= 9 && digits_count <= 12) &&
        S.find_first_not_of("+()-0123456789 \t\n") == string::npos &&
        (S[0] == '+' || isDigit(S[0])) &&
        isDigit(S.back())) {
        
        return to_pn_format(S);
    }
    
    return false;
}

set<string> FinalPhoneNumbers; // only in format +375291234567

void process_doc1() // Генеральная прокуратура2
{
    ifstream in("Enforcers/doc1.txt");
    string S;
    
    int k = 0;
    while (getline(in,S)) {
        k++;
        replaceAll(S,'\t',' ');
        replaceAll(S,',',' ');
        vector<string> v = StringUtils::parse_by_symbol(S,' ');
    }
}

void extract_phone_numbers (const string &from_file, const string &to_file, int &k)
{
    ifstream in(from_file);
    ofstream out(to_file);
    set<string> phone_numbers;
    string S, name;
    getline(in,name);
    
    while (getline(in,S)) {
        for (int len=9; len<=19; len++) for (int i=0; i<=(int)S.length()-len; i++) {
            string T = S.substr(i,len);
            if (isPhoneNumber(T)) {
                phone_numbers.insert(T);
            }
        }
    }
    FinalPhoneNumbers.insert(phone_numbers.begin(),phone_numbers.end());
    
    for (set<string>::iterator it=phone_numbers.begin(); it!=phone_numbers.end(); it++) {
        out << *it << endl;
        k++;
    }
    cout << name << ": ";
    cout << "obtained " << k << " numbers\n";
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
        
    for (int i=1; i<=27; i++) {
        int k = 0;
        string file_input = "Enforcers/doc" + to_string(i) + ".txt";
        string file_output = "Enforcers/phone numbers " + to_string(i) + ".txt";
        extract_phone_numbers(file_input, file_output, k);
        ans += k;
    }
    
    cout << ans << endl;
    
    ans = (ull)FinalPhoneNumbers.size();
    ofstream phoneOut("Enforcers/all phone numbers.txt");
    for (set<string>::iterator it=FinalPhoneNumbers.begin(); it!=FinalPhoneNumbers.end(); it++) {
        //cout << *it << endl;
        phoneOut << *it << endl;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
