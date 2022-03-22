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
#include "RealTest.hpp"
#include "Reducables.hpp"

using namespace Algebra;
using namespace Containers;

#define num vector<char>
#define pnn pair<num,num>

const vll POWER = Combinatorics::generate_powers(2ll,36);
const vsint MAX_LENGTH = { 0,0,0,0,4,4,6,7,7,7,8,9,8,9,9,11,10,11,10,10,12,11,13,13,13,13,13,13,14,14,14,14,14,14,14,15,15};
const vsint MAX_FIRST_DIGIT = { 0,0,0,0,1,1,1,2,2,2,2,2,3,4,4,4,4,4,4,4,4,5,5,6,6,8,8,8,8,8,8,8,8,9,9,9,9 };

int N;

// *** FIRST PART - PRIMITIVE REDUCABLES OBTAINING ***

#define DN vector<pcc> // < digit, count >
sint fc (char c) { if ('0' <= c && c <= '9') return c-'0'; return c-'A'+10; } // 'A' -> 10
char tc (sint n) { if (n < 10) return '0'+n; return 'A'+n-10; }               // 12 -> 'C'
string ts (sint cnt, sint n) { string s; for (int i=0; i<cnt; i++) s += tc(n); return s; }

string stringValue (const DN &A)
{
    string S;
    for (int i=0; i<(int)A.size(); i++) {
        char c = tc(A[i].fs);
        for (int j=0; j<A[i].sc; j++) S += c;
    } return S;
}

void append_digit (DN &A, sint k)
{
    if (k == A.back().fs) A.back().sc++;
    else A.push_back(mp(k,1));
}

void pop_digit (DN &A)
{
    if (A.back().sc == 1) A.pop_back();
    else A.back().sc--;
}

void shift (DN &A)
{
    for (int i=0; i<(int)A.size(); i++) A[i].fs++;
}

psii get_sums (const DN &A) // TODO: make static variables?
{
    sint s1 = 0, s2 = 0;
    for (int i=0; i<(int)A.size(); i++) {
        s1 += A[i].fs*1ll*A[i].sc;
        s2 += A[i].fs*1ll*A[i].fs*A[i].sc;
    } return mp(s1,s2);
}

bool nested (const DN &A, const DN &B) // A < B ?
{
    int pa = 0, pb = 0;
    while (pa != (int)A.size() || pb != (int)B.size()) {
        if (pa == (int)A.size()) return true;
        else if (pb == (int)B.size()) return false;
        else if (A[pa].fs < B[pb].fs) return false;
        else if (A[pa].fs > B[pb].fs) pb++;
        else { if (A[pa].sc > B[pb].sc) return false; pa++; pb++; }
    } return true;
}

DN join_numbers (const DN &A, const DN &B)
{
    DN C;
    int pa = 0, pb = 0;
    while (pa != (int)A.size() || pb != (int)B.size()) {
        if (pa == (int)A.size()) C.push_back(B[pb++]);
        else if (pb == (int)B.size()) C.push_back(A[pa++]);
        else if (A[pa].fs < B[pb].fs) C.push_back(A[pa++]);
        else if (A[pa].fs > B[pb].fs) C.push_back(B[pb++]);
        else { C.push_back(mp(A[pa].fs,A[pa].sc+B[pb].sc)); pa++; pb++; }
    } return C;
}

bool no_common_digit (const DN &A, const DN &B)
{
    int pa = 0, pb = 0;
    while (pa != (int)A.size() && pb != (int)B.size()) {
        if (A[pa].fs < B[pb].fs) pa++;
        else if (A[pa].fs > B[pb].fs) pb++;
        else return false;
    } return true;
}

vector<string> get_reducables (int base) // TODO: think about naive way of obtaining small PRs
{
    N = base;
    const sint L = MAX_LENGTH[N];
    
    // TODO: maybe, only last digit should be stored
    map<psii,pair<DN,char>> M; // key: <s1,s2>, value: < min_number, last(max) digit >
    vector<DN> v;
    for (int i=1; i<N; i++) { // 1-len fill
        v.push_back(DN(1,mp(i,1)));
        M[mp(i,i*i)] = mp(v[i-1],i);
    }
    
    vector<DN> R; // reducable numbers
    for (sint len=2; len<=L; len++) { // fix length
        vector<DN> w;
        map<psii,char> uLD; // updated last digits
        for (int i=0; i<(int)v.size(); i++) { // fix current number with (length-1) digits
            psii s = get_sums(v[i]);
            for (char k=v[i].back().fs; k<N; k++) { // fix current digit to append
                sint s1 = s.fs + k, s2 = s.sc + k*1ll*k;
                append_digit(v[i],k);
                if (M.find(mp(s1,s2)) == M.end()) { // brand new pair (s1,s2)
                    M[mp(s1,s2)] = mp(v[i],k);
                    w.push_back(v[i]);
                }
                else {
                    pair<DN,char> p = M[mp(s1,s2)];
                    if (!no_common_digit(p.fs,v[i])) { pop_digit(v[i]); continue; }
                    p.fs = join_numbers(p.fs,v[i]);
                    M[mp(s1,s2)] = p;
                    
                    bool dominated = false;
                    for (int j=0; j<(int)R.size(); j++) {
                        if (nested(R[j],v[i])) { dominated = true; break; }
                    }
                    if (!dominated) {
                        R.push_back(v[i]); // added to answer
                    }
                    
                    char last_digit = p.fs.back().fs;
                    if (last_digit < p.sc) {
                        sint d = uLD[mp(s1,s2)];
                        // this means we have number with len digits and given last_digit
                        if (d == 0 || d > last_digit) uLD[mp(s1,s2)] = last_digit;
                    }
                }
                pop_digit(v[i]);
            }
        }
        for (map<psii,char>::iterator it=uLD.begin(); it!=uLD.end(); it++) {
            M[it->fs].sc = it->sc;
        }
        w.swap(v);
    }
    
    cout << "Reducables count: " << R.size() << endl;
    vector<string> S;
    for (int i=0; i<(int)R.size(); i++) {
        S.push_back(stringValue(R[i]));
    }
    
    return S;
}

void take_microscope (const vector<string> &R)
{
    cout << "N = " << N << endl;
    int primitive_cnt = 0;
    sint max_length = 0;
    sint max_S1 = 0, max_S2 = 0;
    int max_different_digits = 1;
    int max_first_digit = 1; // in primitives
    set<string> S(R.begin(),R.end());
    for (set<string>::const_iterator it=S.begin(); it!=S.end(); it++) {
        string U = *it;
        
        // 2. count max_length
        if (U.length() > max_length) max_length = (sint)U.length();
        
        // 3. count max_S1,S2
        sint s1 = 0, s2 = 0;
        for (int i=0; i<(int)U.length(); i++) { s1 += fc(U[i]); s2 += fc(U[i])*fc(U[i]); }
        if (s1 > max_S1) max_S1 = s1;
        if (s2 > max_S2) max_S2 = s2;
        
        // 4. count max_different_digits
        int dif = 1;
        for (int i=1; i<(int)U.length(); i++) {
            if (U[i] != U[i-1]) dif++;
        } if (dif > max_different_digits) max_different_digits = dif;
        
        // 1. count primitive_cnt
        bool is_primitive = true;
        while (U[0] != '1') { // reduce string
            for (int i=0; i<(int)U.length(); i++) {
                if (U[i] == 'A') U[i] = '9';
                else U[i]--;
            }
            if (S.find(U) != S.end()) { is_primitive = false; break; }
        }
        if (is_primitive) {
            primitive_cnt++;
            // 5. count max_first_digit
            sint f = fc((*it)[0]);
            if (f > max_first_digit) max_first_digit = f;
        }
    }
    cout << primitive_cnt << " " << max_length << " " << max_S1 << " " << max_S2 << " " << max_different_digits << " " << max_first_digit << endl;
}

bool test_correctness (const vector<string> &s1, const vector<string> &s2)
{
    bool correct = true;
    if (s1.size() != s2.size()) correct = false;
    int p1 = 0, p2 = 0;
    while (p1 != (int)s1.size() || p2 != (int)s2.size()) {
        if (p1 == (int)s1.size()) {
            cout << "Your array doesn't contain " << s2[p2] << endl;
            correct = false;
            p2++;
        }
        else if (p2 == (int)s2.size()) {
            cout << "Real array doesn't contain " << s1[p1] << endl;
            correct = false;
            p1++;
        }
        else if (s1[p1] < s2[p2]) {
            cout << "Real array doesn't contain " << s1[p1] << endl;
            correct = false;
            p1++;
        }
        else if (s1[p1] > s2[p2]) {
            cout << "Your array doesn't contain " << s2[p2] << endl;
            correct = false;
            p2++;
        }
        else { p1++; p2++; }
    }
    return correct;
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    // obtaining hard-coded reducables
    vector<vector<string>> R(37);
    string T; int K = 0;
    while (getline(cin,T)) {
        if (T.length() < 3) K = atoi(T.c_str());
        else R[K].push_back(T);
    }
    
//    for (int b=4; b<=36; b++) {
//        vector<string> S = get_reducables(b);
//        take_microscope(S);
//    }
    
    for (int base=4; base<=36; base++) {
        cout << "N = " << base << endl;
        vector<string> S = R[base];//get_reducables(base);
        //for (int i=0; i<(int)S.size(); i++) cout << S[i] << endl;
        // Total_Time = clock() - Total_Time;
        // cout << "Obtaining reducables time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
        Total_Time = clock();
        vector<string> s1 = Reducables::calculate_patterns(base,S);
        Total_Time = clock() - Total_Time;
        cout << "Generate patterns time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
        Total_Time = clock();
        
        vector<string> s2 = RealTest::real_solution(base);
        cout << "Expected Count: " << s2.size() << endl;
        sort(s1.begin(), s1.end());
        bool ok = test_correctness(s1,s2);
        if (!ok) break;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
