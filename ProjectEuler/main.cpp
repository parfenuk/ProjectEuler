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

string sForm (const num &a)
{
    string S;
    for (int i=0; i<min(N,10); i++) {
        if (a[i] == -1) { S += ('0'+i); S += '*'; }
        else for (int j=0; j<a[i]; j++) S += ('0'+i);
    }
    for (int i=10; i<N; i++) {
        if (a[i] == -1) { S += ('A'+i-10); S += '*'; }
        else for (int j=0; j<a[i]; j++) S += ('A'+i-10);
    }
    return S;
}

pii get_sums (const num &a)
{
    int s1 = 0, s2 = 0;
    for (int i=1; i<N; i++) {
        s1 += i*a[i];
        s2 += i*i*a[i];
    }
    return mp(s1,s2);
}

sint get_different_digits (const num &a)
{
    sint cnt = 0;
    for (int i=1; i<N; i++) if (a[i]) cnt++;
    return cnt;
}

char max_digit (const num &a)
{
    for (int i=N-1; i>=0; i--) if (a[i]) return i;
    return -1;
}

bool operator< (const num &a, const num &b) // no patterns
{
    int sa = 0, sb = 0;
    for (int i=0; i<N; i++) { sa += a[i]; sb += b[i]; }
    if (sa < sb) return true;
    if (sb > sa) return false;
    
    for (int i=0; i<N; i++) {
        if (a[i] < b[i]) return true;
        if (a[i] > b[i]) return false;
    } return false;
}

bool nested (const num &a, const num &b) // true if a is fully contained in b
{
    for (int i=0; i<N; i++) {
        if (a[i] > b[i]) return false;
    } return true;
}

void add (vector<pnn> &a, num n, num s) {
    do {
        a.push_back(mp(n,s));
        if (n[N-1] || s[N-1]) break;
        for (int i=N-1; i>0; i--) {
            n[i] = n[i-1];
            s[i] = s[i-1];
        }
    } while (true);
}

struct DPState {
    sint len;
    sint last_digit;
    ll mask;
    DPState() { len = last_digit = mask = 0; }
    DPState(sint l, sint d, ll m) { len = l; last_digit = d; mask = m; }
    void show() { cout << len << " " << last_digit << " " << mask << endl; }
}initial_state;

vector<vector<DPState>> dp; // dp[s1][s2] = <<len, last_digit>, mask>

num get_subtractive (int s1, int s2, sint last_digit)
{
    num A(N);
    A[last_digit]++;
    while (true) {
        s1 -= last_digit;
        s2 -= last_digit*last_digit;
        if (!s1 || !s2) break;
        last_digit = dp[s1][s2].last_digit;
        A[last_digit]++;
    }
    return A;
}

vector<string> get_reducables (int base)
{
    N = base;
    const sint MAX_L = MAX_LENGTH[N];
    int S1 = (N-1)*MAX_L;
    int S2 = S1*(N-1);
    
    //S1 = 462; S2 = 15246;
    
    vector<pnn> primitive_reducables;
    vector<pnn> reducables;
    
    dp = vector<vector<DPState>>(S1+1);
    for (int i=0; i<=S1; i++) dp[i] = vector<DPState>(S2+1);
    
    queue<pair<pii,DPState>> Q;
    Q.push(mp(mp(0,0),DPState()));
    
    int push_iterations = 1;
    int mask_update_iterations = 0;
    int nested_check_iterations = 0;
    int total_iterations = 1;
    while (!Q.empty()) {
        pair<pii,DPState> p = Q.front(); // < <s1,s2>, <<len,last_digit>,mask> >
        Q.pop();
        pii sums = p.fs;
        DPState D = p.sc;
        //cout << "Processing " << p.fs.fs << " " << p.fs.sc << " "; D.show();
        if (D.len == MAX_L) continue;
        
        for (int k=max((int)D.last_digit,1); k<N; k++) {
            int s1 = sums.fs + k;
            int s2 = sums.sc + k*k;
            if (s1 > S1 || s2 > S2) break;
            ll old_mask = dp[s1][s2].mask;
            ll new_mask = D.mask | POWER[k];
            total_iterations++;
            if (dp[s1][s2].len == 0) {
                push_iterations++;
                dp[s1][s2] = DPState(D.len+1,k,new_mask); // just add
                Q.push(mp(mp(s1,s2),dp[s1][s2]));
                //cout << "Added " << s1 << " " << s2 << " "; dp[s1][s2].show();
            }
            else { // elem (s1,s2) exists
                if (dp[s1][s2].len == D.len+1 || // equal lengths
                    (old_mask | new_mask) != old_mask + new_mask) { // same digit is used
                    dp[s1][s2].mask |= new_mask;
                    mask_update_iterations++;
                    //cout << "Set new mask length " << s1 << " " << s2 << " " << dp[s1][s2].mask << endl;
                }
                else {
                    num a = get_subtractive(s1,s2,k);
                    num b = get_subtractive(s1,s2,dp[s1][s2].last_digit);
                    bool ok = true;
                    for (int i=0; i<(int)reducables.size(); i++) {
                        nested_check_iterations++;
                        if (nested(reducables[i].fs,a)) { ok = false; break; }
                    } if (!ok) continue;
                    primitive_reducables.push_back(mp(a,b));
                    add(reducables,a,b);
                    //cout << "New primitive: " << sForm(a) << " -> " << sForm(b) << endl;
                }
            }
        }
    }
    
    cout << "Primitive reducables count: " << primitive_reducables.size() << endl;
    
    char c = '0';
    S1 = S2 = 0;
    sint dr = 0, ds = 0;
    int len = 0;
    vector<int> cnt_length(MAX_L+1);
    vector<string> res;
//    ofstream out("output.txt");
//    for (int i=0; i<(int)primitive_reducables.size(); i++) {
//        string S = sForm(primitive_reducables[i].fs);
//        string T = sForm(primitive_reducables[i].sc);
//        //out << S << " -> " << T << endl;
//        //out << S << endl;
//        //res.push_back(S);
//
//        // auxiliary calculations
//        cnt_length[S.length()]++;
//        if (S.length() > len) len = (int)S.length();
//        if (S.length() <= 7 && S[0] > c) c = S[0];
//        pii p = get_sums(primitive_reducables[i].fs);
//        if (p.fs > S1) S1 = p.fs;
//        if (p.sc > S2) S2 = p.sc;
//        int d1 = get_different_digits(primitive_reducables[i].fs);
//        int d2 = get_different_digits(primitive_reducables[i].sc);
//        if (d1 > dr) dr = d1;
//        if (d2 > ds) ds = d2;
//    }
//    cout << "\nTotal iterations: " << total_iterations << endl;
//    cout << "Push iterations: " << push_iterations << endl;
//    cout << "Mask iterations: " << mask_update_iterations << endl;
//    cout << "Nested iterations: " << nested_check_iterations << endl;
//    cout << "Count: " << primitive_reducables.size() << " " << reducables.size() << endl;
//    cout << "Max S1 = " << S1 << ", S2 = " << S2 << endl;
//    cout << "Max Digits R: " << dr << ", S: " << ds << endl;
//    cout << "Max symbol: " << c << endl;
    //cout << "Lengths: "; show(cnt_length);
    //cout << "Max Length: " << len;
    
    for (int i=0; i<(int)primitive_reducables.size(); i++) cout << sForm(primitive_reducables[i].fs) << " -> " << sForm(primitive_reducables[i].sc) << endl;
    //for (int i=0; i<(int)reducables.size(); i++) cout << sForm(reducables[i].fs) << " -> " << sForm(reducables[i].sc) << endl;
    for (int i=0; i<(int)reducables.size(); i++) res.push_back(sForm(reducables[i].fs));
    
    return res;
}

string get_num (const vsint &a)
{
    string S;
    for (int i=0; i<(int)a.size(); i++) {
        if (a[i] == 0) continue;
        if (a[i] < 10) S += ('0'+a[i]);
        else S += ('A'+a[i]-10);
    } return S;
}

inline void inc (sint &s1, sint &s2, const sint a) { s1 += a; s2 += a*a; }
inline void dec (sint &s1, sint &s2, const sint a) { s1 -= a; s2 -= a*a; }

// *** SECOND PART - PATTERNS OBTAINING ***

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
    
//    for (ull n=1; n<powmod(8,6); n++) {
//        vector<int> d = NumberUtils::digits(n,8,6);
//        bool sorted = true;
//        for (int i=1; i<6; i++) if (d[i] < d[i-1]) { sorted = false; break; }
//        if (!sorted) continue;
//        if (total_vector_sum(d) == 31) {
//            int s2 = 0;
//            for (int i=0; i<6; i++) s2 += d[i]*d[i];
//            if (s2 == 181) show(d);
//        }
//    }
    
    N = 8;
    vector<string> S = get_reducables(N);
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    Total_Time = clock();
    vector<string> s1 = Reducables::calculate_patterns(N,S);
    
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
