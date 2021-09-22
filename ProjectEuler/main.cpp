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
#include <math.h>
#include <ctime>
#include <cstring>
#include <string>
#include <cassert>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <numeric>
#include <random>
#include <utility>

//#include "Lnum.cpp"
#include "Geometry.cpp"
//#include "Geometry3D.cpp"
#include "Fractions.cpp"
#include "Matrix.cpp"
#include "Complex.cpp"
//#include "Pell_Equation.cpp"
#pragma comment(linker, "/STACK:416777216")

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;
typedef unsigned int uint;
typedef int ltype;

using namespace std;

#define pii pair<int,int>
#define pll pair<ll,ll>
#define pull pair<ull,ull>
#define pdd pair<dd, dd>
#define ppii pair<pair<int,int>, pair<int,int>>
#define ppll pair<pair<ll,ll>, pair<ll,ll>>
#define mp make_pair
#define fs first
#define sc second


int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    const int MAX = 14;
    const string S = "ECAAA8647623200100f";
    ans = (int)S.length();
    vector<int> a(MAX+1);
    vector<int> negative_values;
    for (int i=0; i<(int)S.length(); i++) {
        if (S[i] >= '0' && S[i] <= '9') a[S[i]-'0']++; // 0-9
        else if (S[i] >= 'A' && S[i] <= 'Z') a[S[i]-'A'+10]++; // A-Z, 10-35
        else negative_values.push_back(S[i]-'a'+1); // a-z, -1..-26
    }
    int most_appeared = 0;
    dd average = 0;
    for (int i=MAX; i>=0; i--) {
        cout << a[i];
        if (i) cout << "-";
        else cout << endl;
        if (a[i] > a[most_appeared]) most_appeared = i;
        average += i*a[i];
    }
    if (!negative_values.empty()) {
        cout << "Negative values: ";
        for (int i=0; i<(int)negative_values.size(); i++) {
            cout << -negative_values[i] << " ";
            average -= negative_values[i];
        }
        cout << endl;
    }
    cout << "Most appeared: " << most_appeared << endl;
    cout << "Average: " << fixed << average/S.length() << endl;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
