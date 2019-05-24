#include <set>
#include <bitset>
#include <queue>
#include <deque>
#include <stack>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <ctime>
#include <cstring>
#include <string>
#include <cassert>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <utility>
#pragma comment(linker, "/STACK:416777216")

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;
typedef int ltype;

using namespace std;

void crash()
{
    int n = 16082016;
    cout << n/(n-n);
}

#define pii pair<int,int>
#define pll pair<ll, ll>
#define pdd pair<dd, dd>
#define ppii pair<pair<int,int>, pair<int,int>>
#define ppll pair<pair<ll,ll>, pair<ll,ll>>
#define fs first
#define sc second

#define EMPTY_SYMBOL   ' '
#define PAINT_SYMBOL   '*'
#define UNKNOWN_SYMBOL '?'

#define USER_INTERACTION_MODE
//#define ONLINE_JUDGE
#define PRINT_DIAGNOSTIC

#define MAX_SIZE 50
#define BACKTRACK_DEPTH 1

int N, M;
string S; // length == N
bool dp_pref[MAX_SIZE][MAX_SIZE];
bool dp_suff[MAX_SIZE][MAX_SIZE];
vector<sint> a; // size == M
bool was_any_changes;
string result;

bool can_put (int block, int pos)
{
    if (pos < 0) return false;
    if (pos && S[pos-1] == PAINT_SYMBOL) return false;
    if (pos+block > N) return false;
    if (pos+block != N && S[pos+block] == PAINT_SYMBOL) return false;
    for (int i=pos; i<pos+block; i++) if (S[i] == EMPTY_SYMBOL) return false;
    return true;
}

void fill (bool flag)
{
    bool dp_zero[MAX_SIZE];
    for (int i=0; i<N; i++) {
        
        if (S[i] == PAINT_SYMBOL) { for (int j=i; j<N; j++) dp_zero[j] = false; break; }
        dp_zero[i] = true;
    }
    
    sint sum = 0;
    for (int j=0; j<M; j++) {
        
        if (sum == 0) sum = a[j];
        else sum += a[j]+1;
        
        for (int i=0; i<sum-1; i++) {
            
            if (flag) dp_pref[i][j] = false;
            else dp_suff[i][j] = false;
        }
        
        for (int i=sum-1; i<N; i++) {
            
            if (S[i] == EMPTY_SYMBOL) {
                
                if (i) {
                    
                    if (flag) dp_pref[i][j] = dp_pref[i-1][j];
                    else dp_suff[i][j] = dp_suff[i-1][j];
                }
                else {
                    
                    if (flag) dp_pref[i][j] = false;
                    else dp_suff[i][j] = false;
                }
                continue;
            }
            if (S[i] == PAINT_SYMBOL) {
                
                if (can_put(a[j],i-a[j]+1)) {
                    
                    if (i == a[j]-1) {
                        
                        if (flag) dp_pref[i][j] = true;
                        else dp_suff[i][j] = true;
                    }
                    else if (j) {
                        
                        if (flag) {
                            
                            if (i-a[j]-1 < 0) dp_pref[i][j] = false;
                            else dp_pref[i][j] = dp_pref[i-a[j]-1][j-1];
                        }
                        else {
                            
                            if (i-a[j]-1 < 0) dp_suff[i][j] = false;
                            else dp_suff[i][j] = dp_suff[i-a[j]-1][j-1];
                        }
                    }
                    else {
                        
                        if (flag) dp_pref[i][j] = dp_zero[i-a[j]];
                        else dp_suff[i][j] = dp_zero[i-a[j]];
                    }
                }
                else {
                    
                    if (flag) dp_pref[i][j] = false;
                    else dp_suff[i][j] = false;
                }
                continue;
            }
            if (S[i] == UNKNOWN_SYMBOL) {
                
                if (!i || (flag && dp_pref[i-1][j]) || (!flag && dp_suff[i-1][j])) {
                    
                    if (flag) dp_pref[i][j] = true;
                    else dp_suff[i][j] = true;
                }
                else {
                    
                    if (can_put(a[j],i-a[j]+1)) {
                        
                        if (i == a[j]-1) {
                            
                            if (flag) dp_pref[i][j] = true;
                            else dp_suff[i][j] = true;
                        }
                        else if (j) {
                            
                            if (flag) {
                                
                                if (i-a[j]-1 < 0) dp_pref[i][j] = false;
                                else dp_pref[i][j] = dp_pref[i-a[j]-1][j-1];
                            }
                            else {
                                
                                if (i-a[j]-1 < 0) dp_suff[i][j] = false;
                                else dp_suff[i][j] = dp_suff[i-a[j]-1][j-1];
                            }
                        }
                        else {
                            
                            if (flag) dp_pref[i][j] = dp_zero[i-a[j]];
                            else dp_suff[i][j] = dp_zero[i-a[j]];
                        }
                    }
                    else {
                        
                        if (flag) dp_pref[i][j] = false;
                        else dp_suff[i][j] = false;
                    }
                }
            }
        }
    }
}

bool onReturnFalse()
{
#ifdef PRINT_DIAGNOSTIC
    cout << "S = " << S << endl;
    for (int i=0; i<(int)a.size(); i++) cout << a[i] << " ";
    cout << endl;
#endif
    return false;
}

bool process_string()
{
    result = "";
    
    int checksum = 0;
    for (int i=0; i<M; i++) {
        
        if (checksum == 0) checksum = a[i];
        else checksum += a[i]+1;
    }
    
    if (checksum > N) return onReturnFalse();
    
    if (M == 0) {
        
        for (int i=0; i<N; i++) if (S[i] == PAINT_SYMBOL) return onReturnFalse();
        for (int i=0; i<N; i++) { result += EMPTY_SYMBOL; if (S[i] == UNKNOWN_SYMBOL) was_any_changes = true; }
        return 0;
    }
    if (N == 1) {
        
        if (S[0] == EMPTY_SYMBOL) return onReturnFalse();
        else result += PAINT_SYMBOL; //cout << "*";
        if (S[0] == UNKNOWN_SYMBOL) was_any_changes = true;
        return true;
    }
    
    fill(true);
    
    reverse(S.begin(), S.end());
    reverse(a.begin(), a.end());
    
    fill(false);
    for (int j=0; j<M; j++) for (int i=0; i<(N+1)/2; i++) swap(dp_suff[i][j], dp_suff[N-i-1][j]);
    for (int i=0; i<N; i++) for (int j=0; j<(M+1)/2; j++) swap(dp_suff[i][j], dp_suff[i][M-1-j]);
    
    reverse(S.begin(), S.end());
    reverse(a.begin(), a.end());
    
    if (!dp_pref[N-1][M-1]) return onReturnFalse();
    
    bool dp_zero_pref[400], dp_zero_suff[400];
    for (int i=0; i<N; i++) {
        
        if (S[i] == PAINT_SYMBOL) { for (int j=i; j<N; j++) dp_zero_pref[j] = false; break; }
        dp_zero_pref[i] = true;
    }
    for (int i=N-1; i>=0; i--) {
        
        if (S[i] == PAINT_SYMBOL) { for (int j=i; j>=0; j--) dp_zero_suff[j] = false; break; }
        dp_zero_suff[i] = true;
    }
    
    for (int i=0; i<N; i++) {
        
        if (S[i] != UNKNOWN_SYMBOL) { result += S[i]; continue; }
        
        bool can_0 = false, can_1 = false;
        
        if (i == 0) {
            
            can_0 = dp_suff[1][0];
            can_1 = can_put(a[0],0) && ((M > 1 && a[0]+1 < N && dp_suff[a[0]+1][1]) || (M == 1 && (a[0]+1 >= N || dp_zero_suff[a[0]+1])));
        }
        else if (i == N-1) {
            
            can_0 = dp_pref[N-2][M-1];
            can_1 = can_put(a[M-1],N-a[M-1]) && ((M > 1 && N-a[M-1]-2 >= 0 && dp_pref[N-a[M-1]-2][M-2]) || (M == 1 && (N-a[M-1]-2 < 0 || dp_zero_pref[N-a[M-1]-2])));
        }
        else {
            
            if (dp_zero_pref[i-1] && dp_suff[i+1][0]) can_0 = true;
            else if (dp_pref[i-1][M-1] && dp_zero_suff[i+1]) can_0 = true;
            else {
                
                for (int j=0; j<M-1; j++) {
                    
                    if (dp_pref[i-1][j] && dp_suff[i+1][j+1]) { can_0 = true; break; }
                }
            }
            
            for (int j=0; j<M; j++) {
                
                for (int pos = i-a[j]+1; pos<=i; pos++) {
                    
                    if (!can_put(a[j], pos)) continue;
                    if (M == 1) {
                        
                        if ((pos < 2 || dp_zero_pref[pos-2]) && (pos+a[j]+1 >= N || dp_zero_suff[pos+a[j]+1])) { can_1 = true; break; }
                        continue;
                    }
                    
                    if (j == 0) {
                        
                        if ((pos < 2 || dp_zero_pref[pos-2]) && (pos+a[j]+1 < N && dp_suff[pos+a[j]+1][j+1])) { can_1 = true; break; }
                    }
                    else if (j == M-1) {
                        
                        if ((pos >= 2 && dp_pref[pos-2][j-1]) && (pos+a[j]+1 >= N || dp_zero_suff[pos+a[j]+1])) { can_1 = true; break; }
                    }
                    else {
                        
                        if ((pos >= 2 && dp_pref[pos-2][j-1]) && (pos+a[j]+1 < N && dp_suff[pos+a[j]+1][j+1])) { can_1 = true; break; }
                    }
                }
                if (can_1) break;
            }
        }
        
        if (can_0 && can_1) result += UNKNOWN_SYMBOL;
        else if (can_0) { result += EMPTY_SYMBOL; was_any_changes = true; }
        else if (can_1) { result += PAINT_SYMBOL; was_any_changes = true; }
        else return onReturnFalse();
    }
    
    return true;
}

int rows, columns;
char Field[MAX_SIZE][MAX_SIZE];
vector<sint> rows_digits[MAX_SIZE];
vector<sint> cols_digits[MAX_SIZE];

bool process_row (int i)
{
    N = columns;
    a = rows_digits[i];
    M = (int)a.size();
    
    S = "";
    for (int j=0; j<columns; j++) S += Field[i][j];
    
    bool q = process_string();
    if (q) for (int j=0; j<columns; j++) Field[i][j] = result[j];
    
    //if (i == 7) cout << S << endl << result << endl << endl;
    
    return q;
}

bool process_column (int j)
{
    N = rows;
    a = cols_digits[j];
    M = (int)a.size();
    
    S = "";
    for (int i=0; i<rows; i++) S += Field[i][j];
    
    bool q = process_string();
    if (q) for (int i=0; i<rows; i++) Field[i][j] = result[i];
    
    //if (j == 13) cout << S << endl << result << endl << endl;
    
    return q;
}

bool run_loop()
{
    was_any_changes = false;
    
    for (int i=0; i<rows; i++) if (!process_row(i)) return false;
    for (int j=0; j<columns; j++) if (!process_column(j)) return false;
    
    return true;
}

// 0 - no solution
// 1 - exact solution
// 2 - ambiguous

void paint_answer();

void reset_field()
{
    for (int i=0; i<rows; i++) for (int j=0; j<columns; j++) Field[i][j] = UNKNOWN_SYMBOL;
}

sint try_solve (const vector<pii> cells)
{
    reset_field();
    for (int i=0; i<(int)cells.size(); i++) Field[cells[i].fs][cells[i].sc] = PAINT_SYMBOL;
    
    was_any_changes = true;
    
    while (was_any_changes) {
        
        bool q = run_loop();
        if (!q) return 0;
    }
    
    for (int i=0; i<rows; i++) for (int j=0; j<columns; j++) if (Field[i][j] == UNKNOWN_SYMBOL) return 2;
    
    return 1;
}

void paint_answer()
{
    for (int i=0; i<rows; i++) {
        
        for (int j=0; j<columns; j++) cout << Field[i][j] << " ";
        cout << endl;
    }
}

vector<pii> cells;

bool choose_next_cell (bool from_start = false)
{
    if (!from_start && cells.empty()) return false;
    
    pii p = from_start ? make_pair(0,-1) : cells.back();
    if (!from_start) { cells.pop_back(); try_solve(cells); }
    
    for (int i=p.fs; i<rows; i++) {
        
        int k = 0;
        if (i == p.fs) k = p.sc+1;
        for (int j=k; j<columns; j++) {
            
            if (Field[i][j] == '?') {
                
                cells.push_back(make_pair(i,j));
                return true;
            }
        }
    }
    
    return false;
}

int main()
{
    ios_base::sync_with_stdio (false);
    //cout.precision(3);
#ifndef USER_INTERACTION_MODE
    freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    cin >> rows;
    for (int i=0; i<rows; i++) {
        
        int K; cin >> K;
        for (int j=0; j<K; j++) {
            
            sint b; cin >> b;
            rows_digits[i].push_back(b);
        }
    }
    
    cin >> columns;
    for (int i=0; i<columns; i++) {
        
        int K; cin >> K;
        for (int j=0; j<K; j++) {
            
            sint b; cin >> b;
            cols_digits[i].push_back(b);
        }
    }
    
    while (true) {
        
        sint n = try_solve(cells);
        
        if (n == 1) { cout << "Solved!\n"; paint_answer(); break; }
        if (n == 0) {
            
            if (cells.empty()) { cout << "This puzzle doesn't have any solution. We're sorry :(\n"; break; }
            
#ifdef USER_INTERACTION_MODE
            cout << "No solution for this cell being painted. Choose another one\n";
            cells.pop_back();
#else
            while (!choose_next_cell()) {
                
                if (cells.empty()) { cout << "This puzzle doesn't have any solution. We're sorry :(\n"; return 0; }
            }
#endif
        }
        if (n == 2) {
            
#ifdef USER_INTERACTION_MODE
            cout << "This solution is ambiguous. Type the cell '?' you want to paint\n";
            paint_answer();
            
            while (true) {
                
                pii p;
                cin >> p.fs >> p.sc;
                
                if (Field[p.fs][p.sc] != UNKNOWN_SYMBOL) cout << "This cell can't be painted\n";
                
                else {
                    
                    cells.push_back(p);
                    break;
                }
            }
#else
            if (cells.size() < BACKTRACK_DEPTH) choose_next_cell(true);
            else {
                
                while (!choose_next_cell()) {
                    
                    if (cells.empty()) { cout << "This puzzle doesn't have any solution. We're sorry :(\n"; return 0; }
                }
            }
#endif
        }
    }
    
    return 0;
}
