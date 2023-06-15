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
using namespace Geometry;
using namespace Fractions;

struct Info {
    vint answer; // sorted in decreasing order
    int lcm;
}I[151][51];

void fill_info()
{
    string S;
    while (getline(cin,S)) {
        size_t slash = S.find("/"), colon = S.find(":"), b1 = S.find("{"), b2 = S.find("}");
        int M = atoi(S.substr(0,slash).c_str());
        int N = atoi(S.substr(slash+1,colon-slash-1).c_str());
        vector<string> w = StringUtils::parse_by_symbol(S.substr(b1+2,b2-b1-3), ',');
        for (int i=0; i<(int)w.size(); i++) I[M][N].answer.push_back(atoi(w[i].c_str()));
    }
}

set<int> used_numbers (int N)
{
    set<int> s;
    for (int M=1; M<=50 && M <= 3*N; M++) s.insert(I[M][N].answer.begin(),I[M][N].answer.begin());
    return s;
}

void show_vector (const vector<int> &a)
{
    cout << "{ ";
    show(a,',',false);
    cout << " }\n";
}

ll LCM (const vector<int> &a)
{
    ll L = 1;
    for (int i=0; i<(int)a.size(); i++) L = LCM(L,a[i]);
    return L;
}

vint hard_coded (int M, int N)
{
    // 2: return { , };
    // 3: return { ,, };
    // 4: return { ,,, };
    // 5: return { ,,,, };
    
    if (N == 17) {
        if (M == 49) return { 189,136,135,30,9,8,7,6,5,4,3,2,1 };
    }
    if (N == 19) {
        if (M == 50) return { 190,140,38,7,6,5,4,3,2,1 };
    }
    if (N == 37) {
        if (M == 2) return { 222,111,74,37 }; // optimal
        if (M == 7) return { 111,74,6 }; // optimal
        if (M == 18) return { 296,222,148,74,8,3 }; // optimal
        if (M == 21) return { 296,280,259,148,20,2 }; // optimal
        if (M == 35) return { 148,111,74,12,3,2 }; // optimal
        if (M == 39) return { 222,111,74,37,1 }; // optimal
        if (M == 44) return { 111,74,6,1 }; // optimal
    }
    if (N == 41) {
        if (M == 10) return { 246,205,164,82,60,5 }; // optimal
        if (M == 13) return { 205,82,20,4 }; // optimal
        if (M == 17) return { 205,164,123,82,20,3 }; // optimal
        if (M == 18) return { 246,164,82,12,3 }; // optimal
        if (M == 25) return { 164,123,82,12,2 }; // optimal
        if (M == 35) return { 123,82,3,2 }; // optimal
    }
    if (N == 43) {
        if (M == 2) return { 215,172,129,86,60 }; // optimal
        if (M == 3) return { 215,172,129,86,60,43 }; // optimal
        if (M == 9) return { 129,86,43,6 }; // optimal
        if (M == 14) return { 344,301,258,56,21,4 }; // optimal as (4)430
        if (M == 16) return { 215,172,86,60,3 }; // optimal
        if (M == 18) return { 344,172,86,43,24,3 }; // optimal
        if (M == 21) return { 301,215,43,42,10,3 }; // optimal
        if (M == 23) return { 86,43,2 }; // optimal
        if (M == 25) return { 344,172,129,43,24,2 }; // optimal
        if (M == 28) return { 344,43,8,2 }; // optimal
        if (M == 31) return { 301,215,172,140,5,2 }; // optimal as (5)473
        if (M == 32) return { 344,215,86,40,5,2 }; // optimal as (4)1677, (5)1032
        if (M == 34) return { 172,86,43,4,2 }; // optimal
        if (M == 41) return { 172,129,43,12,3,2 }; // optiomal as (5)645
        if (M == 45) return { 215,172,129,86,60,1 }; // optimal
        if (M == 46) return { 301,129,86,43,42,1 }; // optimal
    }
    if (N == 47) {
        if (M == 3) return { 376,282,188,94,24 }; // optimal as (4)470
        if (M == 4) return { 282,235,94,15 }; // optimal
        if (M == 11) return { 329,308,235,188,55,5 }; // optimal
        if (M == 15) return { 329,235,210,141,20,4 }; // optimal as (4)752 (5)376
        if (M == 23) return { 329,235,94,15,14,3 }; // optimal as (4)2115 (5)376
        if (M == 26) return { 376,282,188,24,2 }; // optimal as (4)705
        if (M == 31) return { 235,188,140,7,2 }; // optimal
        if (M == 35) return { 235,141,30,5,2 }; // optimal
        if (M == 37) return { 188,94,47,4,2 }; // optimal as (4)658
        if (M == 38) return { 376,282,94,24,4,2 }; // optimal as (5)658
        if (M == 42) return { 376,188,94,24,3,2 }; // optimal as (4)1316 (5)846
        if (M == 44) return { 235,94,47,15,3,2 }; // optimal as (5)940
        if (M == 46) return { 329,235,15,14,3,2 }; // optimal as (5)987
        if (M == 49) return { 282,141,94,47,1 }; // optimal
        if (M == 50) return { 376,282,188,94,24,1 }; // optimal
    }
    if (N == 49) {
        if (M == 39) return { 98,28,4,2 }; // optimal
        if (M == 40) return { 98,49,28,4,2 }; // optimal
        if (M == 45) return { 147,98,49,21,3,2 }; // optimal
    }
    
    return {};
}

vector<int> egypt_fractions (int M, int N, int L = 500)
{
    const int LIMIT = L;
    const int MAX_SIZE = 15;
    vint a = hard_coded(M,N);
    if (!a.empty()) return a;
    //vint a;
    
    fraction hours(M,N), MAX_FRACTION(1,LIMIT);
    int i = 1;
    while (true) {
        fraction league(1,i);
        if (hours >= league) {
            hours = hours - league;
            a.push_back(i);
        }
        i++;
        if (!hours.num) break;
        if (hours < MAX_FRACTION) {
            do {
                i = a.back() + 1;
                a.pop_back();
                hours = hours + fraction(1,i-1);
            }
            while (i > LIMIT);
        }
        else if (a.size() == MAX_SIZE) {
            hours = hours + fraction(1,a.back());
            a.pop_back();
            i = a.back() + 1;
            a.pop_back();
            hours = hours + fraction(1,i-1);
        }
    }
    return a;
}

vector<int> brute_force (int M, int N, int L, int from = 1)
{
    fraction F(M,N), f;
    for (int i=from; i<=L; i++) {
        if (fraction(5,i) <= F) break;
        f = f + fraction(1,i);
        if (f > F) { f = f - fraction(1,i); continue; }
        if (f == F) return { i };
        for (int j=i+1; j<=L; j++) {
            if (f + fraction(4,j) <= F) break;
            f = f + fraction(1,j);
            if (f > F) { f = f - fraction(1,j); continue; }
            if (f == F) return { j,i };
            for (int k=j+1; k<=L; k++) {
                if (f + fraction(3,k) <= F) break;
                f = f + fraction(1,k);
                if (f > F) { f = f - fraction(1,k); continue; }
                if (f == F) return { k,j,i };
                for (int m=L; m>k; m--) {
                    f = f + fraction(1,m);
                    if (f > F) { f = f - fraction(1,m); break; }
                    if (f == F) return { m,k,j,i };
                    for (int n=L; n>m; n--) {
                        f = f + fraction(1,n);
                        if (f > F) { f = f - fraction(1,n); break; }
                        if (f == F) return { n,m,k,j,i };
                        fraction rest = F-f;
                        if (rest.num == 1 && rest.den > n && rest.den <= L) return { (int)rest.den,n,m,k,j,i };
                        f = f - fraction(1,n);
                    }
                    f = f - fraction(1,m);
                }
                f = f - fraction(1,k);
            }
            f = f - fraction(1,j);
        }
        f = f - fraction(1,i);
    }
    
    return {};
}

vint find_optimal (int M, int N, int L, int from = 1) // for K <= 6
{
    vint HC = hard_coded(M,N);
    if (!HC.empty()) return HC;
    
    vint a,ret;
    int ub = L;
    while (true) {
        a = brute_force(M,N,ub,from);
        if (a.empty()) break;
        ub = a[0]-1; ret.swap(a);
    } return ret;
}

vint find_optimal_for_big (int M, int N, int L)
{
    fraction F(M,N); F = F - (fraction(1)+fraction(1,2));
    vint ans;
    for (int from=3;;from++) {
        F = F - fraction(1,from);
        if (F.num < 0) break;
        vint a = find_optimal((int)F.num,(int)F.den,L,from+1);
        if (!a.empty()) {
            for (int i=from; i>=1; i--) a.push_back(i);
            if (ans.empty() || a[0] < ans[0]) ans = a;
        }
    } return ans;
}

const int MAX_DEN = 2632; // 10/47 requires such interim denominator
vint rec (int M, int N, int K, int L, int from = 1)
{
    if (N > MAX_DEN) return {};
    if (M == 1 && N > L) return {};
    if (M == 1 && N >= from) return {N};
    if (K == 1) return {};
    fraction F(M,N);
    if (F > fraction(K,from)) return {}; // 1/from + 1/(from+1) + ... + 1/(from+K-1) < K/from
    if (F < fraction(1,L)) return {};
    for (int i=from; i<=L; i++) {
        fraction f = F - fraction(1,i);
        if (f.num == 0) return {i};
        if (f.num < 0) continue;
        vint a = rec((int)f.num,(int)f.den,K-1,L,i+1);
        if (!a.empty()) { a.push_back(i); return a; }
    }
    return {};
}

vint Rec (int M, int N, int K, int L)
{
    vint a,ret;
    int ub = L;
    while (true) {
        a = rec(M,N,K,ub);
        if (a.empty()) break;
        sort(a.rbegin(),a.rend());
        ub = a[0]-1; ret.swap(a);
    } return ret;
}

vector<vector<psii>> D;
void BFS (int L = 376, int K = 20)
{
    D = vector<vector<psii>>(MAX_DEN+1); // < 1/n, count of summands >
    for (int i=1; i<=MAX_DEN; i++) D[i] = vector<psii>(3*i+1);
    const fraction THREE(3);
    
    vector<fraction> v;
    v.push_back(fraction());
    for (int n=1; n<=L; n++) {
        fraction F(1,n);
        for (int i=0;;i++) {
            fraction f = v[i];
            if (D[f.den][f.num].fs == n) break;
            
            sint steps = D[f.den][f.num].sc;
            if (steps == K) continue;
            
            f = f + F;
            if (f > THREE) continue;
            if (f.den > MAX_DEN) continue;
            if (D[f.den][f.num].fs) continue;
            D[f.den][f.num] = mp(n,steps+1);
            v.push_back(f);
        }
    }
}

vint get_from_BFS (int M, int N)
{
    if (D[N][M].fs == 0) return {};
    
    vint a;
    fraction F(M,N);
    while (F.num) {
        int n = D[F.den][F.num].fs;
        F = F - fraction(1,n);
        a.push_back(n);
    } return a;
}

// Algorithms to implement:
// 1. Brute-force O(N^max_summands_count)
// 2. Existing and optimized back-tracking
// 3. Fractional dp with fixed accuracy
// 4. DP with low fractions denominators
// 5. Fibonacci algo (just in case, who knows)
// 6. Incrementing denominator by N step-by-step

int main() {
    clock_t Total_Time = clock();
    cout.precision(3);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    fill_info();
    for (int N=1; N<=50; N++) for (int M=1; M<=3*N && M<=50; M++) {
        if (GCD(N,M) != 1) continue;
        //Total_Time = clock();
        vint a = rec(M,N,13,376);
        //float time = ((float)(clock() - Total_Time))/CLOCKS_PER_SEC;
        cout << M << "/" << N << ": ";
        show_vector(a);
    }
    
    // 4. DP solution
//    BFS();
//    for (int N=1; N<=50; N++) for (int M=1; M<=3*N && M<=50; M++) {
//        if (GCD(N,M) != 1) continue;
//        vint a = get_from_BFS(M,N);
//        cout << M << "/" << N << ": ";
//        show_vector(a);
//    }
    
    // Recursive DP solution
//    for (int N=1; N<=50; N++) for (int M=1; M<=3*N && M<=50; M++) {
//        if (GCD(N,M) != 1) continue;
//        int K = 10; if (fraction(M,N) >= fraction(2)) K = 15;
//        vint a = Rec(M,N,K,429);
//        cout << M << "/" << N << ": ";
//        show_vector(a);
//    }
    
//    pii R[51][51]; // < optimal_1st_number, lcm >
//
//    // 1. Brute-Force with fixed K <= 6 and LIMIT
//    for (int N=1; N<=50; N++) for (int M=1; M<=3*N && M<=50; M++) {
//        if (GCD(N,M) != 1) continue;
//        vint a;
//        if (fraction(M,N) >= fraction(2)) {
//            a = find_optimal_for_big(M,N,138);
//            ll lcm = LCM(a);
//            R[M][N] = mp(a.empty()?0:a[0],lcm);
//        }
//        else if (fraction(M,N) > fraction(1)) { R[M][N] = R[M-N][N]; }
//        else {
//            a = find_optimal(M,N,423);
//            ll lcm = LCM(a);
//            R[M][N] = mp(a.empty()?0:a[0],lcm);
//        }
//        cout << M << "/" << N << ": " << R[M][N].fs << " " << R[M][N].sc << " ";
//        if (R[M][N].sc > ans) ans = R[M][N].sc;
//        cout << endl;
//    }
    
    // OLD ALGO: Backtrack + hard-coded values
//    for (int N=1; N<=50; N++) {
//        for (int M=1; M<=3*N && M<=50; M++) {
//            //Total_Time = clock();
//            if (GCD(N,M) != 1) continue;
//            //if (fraction(M,N) < fraction(2)) continue;
//            vint a = egypt_fractions(M,N);
//            //float time = ((float)(clock() - Total_Time))/CLOCKS_PER_SEC;
//            if (a[0] > a.back()) swap(a[0],a.back());
////            cout << M << "/" << N << ": " << a.back() << " " << a.size() << " " << time << endl;
////            if (time > 1) show_vector(a);
//            ll L = LCM(a);
//            cout << M << "/" << N << ": " << L << " ";
//            if (L > 5000) show_vector(a);
//            else cout << endl;
//            if (L > ans) ans = L;
//        }
//    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
