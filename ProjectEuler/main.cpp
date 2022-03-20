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

#define DN vector<psii> // < digit, count >
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
        s1 += A[i].fs*A[i].sc;
        s2 += A[i].fs*A[i].fs*A[i].sc;
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
    const int L = MAX_LENGTH[N];
    // 462, 15246 max - but check it then
    // TODO: maybe, only last digit should be stored
    map<psii,pair<DN,sint>> M; // key: <s1,s2>, value: < min_number, last(max) digit >
    vector<DN> v;
    for (int i=1; i<N; i++) { // 1-len fill
        v.push_back(DN(1,mp(i,1)));
        M[mp(i,i*i)] = mp(v[i-1],i);
    }
    
    vector<pair<DN,sint>> R; // sint - count how many times it can be increased in family
    for (int len=2; len<=L; len++) { // fix length
        vector<DN> w;
        map<psii,sint> uLD; // updated last digits
        for (int i=0; i<(int)v.size(); i++) { // fix current number with (length-1) digits
            psii s = get_sums(v[i]);
            for (int k=v[i].back().fs; k<N; k++) { // fix current digit to append
                sint s1 = s.fs + k, s2 = s.sc + k*k;
                append_digit(v[i],k);
                if (M.find(mp(s1,s2)) == M.end()) { // brand new pair (s1,s2)
                    M[mp(s1,s2)] = mp(v[i],k);
                    w.push_back(v[i]);
                }
                else {
                    pair<DN,sint> p = M[mp(s1,s2)];
                    if (!no_common_digit(p.fs,v[i])) { pop_digit(v[i]); continue; }
                    p.fs = join_numbers(p.fs,v[i]);
                    M[mp(s1,s2)] = p;
                    
                    bool dominated = false;
                    for (int j=0; j<(int)R.size(); j++) {
                        if (nested(R[j].fs,v[i])) { dominated = true; break; }
                    }
                    if (!dominated) {
                        R.push_back(mp(v[i],N-1-p.fs.back().fs)); // added to answer
                    }
                    
                    sint last_digit = p.fs.back().fs;
                    if (last_digit < p.sc) {
                        sint d = uLD[mp(s1,s2)];
                        // this means we have number with len digits and given last_digit
                        if (d == 0 || d > last_digit) uLD[mp(s1,s2)] = last_digit;
                    }
                }
                pop_digit(v[i]);
            }
        }
        for (map<psii,sint>::iterator it=uLD.begin(); it!=uLD.end(); it++) {
            M[it->fs].sc = it->sc;
        }
        w.swap(v);
    }
    
    set<string> S;
    for (int i=0; i<(int)R.size(); i++) {
        S.insert(stringValue(R[i].fs));
        for (int j=0; j<R[i].sc; j++) {
            shift(R[i].fs);
            S.insert(stringValue(R[i].fs));
        }
    }
    
    return vector<string>(S.begin(),S.end());
}

vvsint LD;

DN get_DN (sint s1, sint s2, sint k)
{
    DN R;
    do {
        s1 -= k;
        s2 -= k*k;
        if (R.empty() || R.back().fs != k) R.push_back(mp(k,1));
        else R.back().sc++;
        k = LD[s1][s2];
    } while (s1);
    reverse(R.begin(),R.end());
    return R;
}

vector<string> get_reducables3 (int base) // slightly wrong
{
    N = base;
    const int L = 10;
    sint S1 = (N-1)*L, S2 = S1*(N-1);
    LD = vvsint(S1+1);
    for (int i=0; i<=S1; i++) LD[i] = vector<sint>(S2+1);
    map<psii,sint> M; // current last digits
    
    vector<pair<psii,sint>> v; // < <s1,s2>, last_digit >
    for (int i=1; i<N; i++) { // 1-len fill
        LD[i][i*i] = i;
        v.push_back(mp(mp(i,i*i),i));
    }
    
    vector<pair<DN,sint>> R;
    
    for (int len=2; len<=L; len++) { // fix length
        M.clear();
        vector<pair<psii,sint>> w;
        for (int i=0; i<(int)v.size(); i++) { // fix current number with (length-1) digits
            for (sint k=v[i].sc; k<N; k++) {
                sint s1 = v[i].fs.fs + k, s2 = v[i].fs.sc + k*k;
                if (LD[s1][s2] == 0) {
                    LD[s1][s2] = k;
                    w.push_back(mp(mp(s1,s2),k));
                }
                else {
                    if (k < LD[s1][s2]) { // update last digit if necessary
                        int d = M[mp(s1,s2)];
                        if (d == 0 || k < d) M[mp(s1,s2)] = k;
                    }
                    
                    DN r = get_DN(s1,s2,k);
                    bool dominated = false;
                    for (int j=0; j<(int)R.size(); j++) {
                        if (nested(R[j].fs,r)) { dominated = true; break; }
                    }
                    if (!dominated) {
                        R.push_back(mp(r,N-1-max(LD[s1][s2],k))); // added to answer
                    }
                }
            }
        }
        w.swap(v);
        for (map<psii,sint>::iterator it=M.begin(); it!=M.end(); it++) {
            if (it->sc < LD[it->fs.fs][it->fs.sc]) LD[it->fs.fs][it->fs.sc] = it->sc;
        }
    }
    
    vector<string> S;
    for (int i=0; i<(int)R.size(); i++) {
        S.push_back(stringValue(R[i].fs));
        for (int j=0; j<R[i].sc; j++) {
            shift(R[i].fs);
            S.push_back(stringValue(R[i].fs));
        }
    }
    
    return S;
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
    
    N = 27;
    vector<string> S = get_reducables(N);
    //for (int i=0; i<(int)S.size(); i++) cout << S[i] << endl;
    Total_Time = clock() - Total_Time;
    cout << "Obtaining reducables time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    Total_Time = clock();
    vector<string> s1 = Reducables::calculate_patterns(N,S);
    Total_Time = clock() - Total_Time;
    cout << "Generate patterns time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    Total_Time = clock();
    
    vector<string> s2 = RealTest::real_solution(N);
    cout << "Expected Count: " << s2.size() << endl;
    sort(s1.begin(), s1.end());
    int p1 = 0, p2 = 0;
    while (p1 != (int)s1.size() || p2 != (int)s2.size()) {
        if (p1 == (int)s1.size()) {
            cout << "Your array doesn't contain " << s2[p2] << endl;
            p2++;
        }
        else if (p2 == (int)s2.size()) {
            cout << "Real array doesn't contain " << s1[p1] << endl;
            p1++;
        }
        else if (s1[p1] < s2[p2]) {
            cout << "Real array doesn't contain " << s1[p1] << endl;
            p1++;
        }
        else if (s1[p1] > s2[p2]) {
            cout << "Your array doesn't contain " << s2[p2] << endl;
            p2++;
        }
        else { p1++; p2++; }
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
