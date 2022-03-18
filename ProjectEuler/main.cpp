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

#define num vector<char>
#define pnn pair<num,num>

const vll POWER = Combinatorics::generate_powers(2ll,36);

int N;

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
    
    int cnt = 0;
    vvsint a = Combinatorics::sum_partitions(13);
    for (int i=0; i<a.size(); i++) if (a[i].size() <= 6) cnt++;
    cout << cnt;
        
    N = 36;
    const sint MAX_L = 15;
    int S1 = (N-1)*MAX_L;
    int S2 = S1*(N-1);
    
    S1 = 462; S2 = 15246;
    
    vector<pnn> primitive_reducables;
    vector<pnn> reducables;
    
    dp = vector<vector<DPState>>(S1+1);
    for (int i=0; i<=S1; i++) dp[i] = vector<DPState>(S2+1);
    
    queue<pair<pii,DPState>> Q;
    Q.push(mp(mp(0,0),DPState()));
    
    int iterations = 1;
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
            iterations++;
            if (dp[s1][s2].len == 0) {
                dp[s1][s2] = DPState(D.len+1,k,new_mask); // just add
                Q.push(mp(mp(s1,s2),dp[s1][s2]));
                //cout << "Added " << s1 << " " << s2 << " "; dp[s1][s2].show();
            }
            else { // elem (s1,s2) exists
                if (dp[s1][s2].len == D.len+1 || // equal lengths
                    (old_mask | new_mask) != old_mask + new_mask) { // same digit is used
                    dp[s1][s2].mask |= new_mask;
                    //cout << "Set new mask length " << s1 << " " << s2 << " " << dp[s1][s2].mask << endl;
                }
                else {
                    num a = get_subtractive(s1,s2,k);
                    num b = get_subtractive(s1,s2,dp[s1][s2].last_digit);
                    bool ok = true;
                    for (int i=0; i<(int)reducables.size(); i++) {
                        if (nested(reducables[i].fs,a)) { ok = false; break; }
                    } if (!ok) continue;
                    primitive_reducables.push_back(mp(a,b));
                    add(reducables,a,b);
                    //cout << "New primitive: " << sForm(a) << " -> " << sForm(b) << endl;
                }
            }
        }
    }
    
    char c = '0';
    S1 = S2 = 0;
    sint dr = 0, ds = 0;
    for (int i=0; i<(int)primitive_reducables.size(); i++) {
        string S = sForm(primitive_reducables[i].fs);
        if (S.length() > ans) ans = S.length();
        if (S[0] > c) c = S[0];
        cout << sForm(primitive_reducables[i].fs) << " -> " << sForm(primitive_reducables[i].sc) << endl;
        pii p = get_sums(primitive_reducables[i].fs);
        if (p.fs > S1) S1 = p.fs;
        if (p.sc > S2) S2 = p.sc;
        int d1 = get_different_digits(primitive_reducables[i].fs);
        int d2 = get_different_digits(primitive_reducables[i].sc);
        if (d1 > dr) dr = d1;
        if (d2 > ds) ds = d2;
    }
    cout << "\nTotal iterations: " << iterations << endl;
    cout << "Count: " << primitive_reducables.size() << endl;
    cout << "Max S1 = " << S1 << ", S2 = " << S2 << endl;
    cout << "Max Digits R: " << dr << ", S: " << ds << endl;
    cout << "Max symbol: " << c;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
