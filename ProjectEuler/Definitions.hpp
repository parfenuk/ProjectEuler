//
//  Definitions.hpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/5/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

// Cool variable names: best, tmp, item, unit, pwr

#pragma once
// TODO: comment what we use from each include, like in json.hpp
#include <set>
#include <unordered_set>
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
#define vbool vector<bool>
#define vchar vector<char>
#define vsint vector<sint>
#define vint vector<int>
#define vll vector<ll>
#define vull vector<ull>
#define vdd vector<dd>
#define vstring vector<string>
#define vpii vector<pair<int,int>>
#define vpll vector<pair<ll,ll>>
#define vpull vector<pair<ull,ull>>
#define vvchar vector<vector<char>>
#define vvsint vector<vector<sint>>
#define vvint vector<vector<int>>
#define vvll vector<vector<ll>>
#define vvull vector<vector<ull>>
#define vvdd vector<vector<dd>>
#define vvpii vector<vector<pair<int,int>>>
#define vvpll vector<vector<pair<ll,ll>>>
#define vvpull vector<vector<pair<ull,ull>>>
#define mii map<int,int>
#define mll map<ll,ll>
#define mull map<ull,ull>
#define msi map<string,int>
#define mp make_pair
#define fs first
#define sc second

const dd EPS = 0.00000001;
const dd PI = acos(-1.0);
const ull INF = 1000000000000000000;
