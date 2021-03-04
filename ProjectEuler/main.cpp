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
#include <ctime>
#include <cstring>
#include <cassert>

#include <math.h>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <numeric>
#include <random>
#include <utility>

#include "Algebra.cpp"
//#include "NumberUtils.cpp"
//#include "Lnum.cpp"
#include "Geometry.cpp"
//#include "Geometry3D.cpp"
#include "Fractions.cpp"
#include "Matrix.cpp"
//#include "Complex.cpp"
//#include "Pell_Equation.cpp"
#include "Utils.cpp"
#pragma comment(linker, "/STACK:16777216")

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;

using namespace std;

#define pbb pair<bool,bool>
#define pcc pair<char,char>
#define psii pair<sint,sint>
#define pii pair<int,int>
#define pll pair<ll,ll>
#define pull pair<ull,ull>
#define pdd pair<dd,dd>
#define ppii pair<pair<int,int>, pair<int,int>>
#define ppll pair<pair<ll,ll>, pair<ll,ll>>
#define pss pair<string,string>
#define vint vector<int>
#define vvint vector<vector<int>>
#define vpii vector<pair<int,int>>
#define mp make_pair
#define fs first
#define sc second

// TODO: Add header files!!!
// TODO: Add definitions file

// TODO: make generator function of all this stuff
//const ull POWER[21] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576};
//const ull POWER[12] = {1,3,9,27,81,243,729,2187,6561,19683,59049,177147};
//const ull FACT[12] = {1,1,2,6,24,120,720,5040,40320,362880,3628800,39916800};

// TODO: write operators for pairs (with templates ofc, in utils)
// TODO: write map and set search: find minimum greater / maximum less
// TODO: write binomials generation


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
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
