#include "Definitions.hpp"

#include "Algebra.hpp"
#include "Combinatorics.cpp"
#include "Complex.cpp"
// That's why we include .cpp here despite of having header. Ugly workaround
// https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
#include "Containers.cpp"
#include "Geometry.cpp"
#include "Geometry3D.cpp"
#include "FFT.cpp"
#include "Fractions.cpp"
#include "Lnum.hpp"
#include "Matrix.cpp"
#include "NumberUtils.hpp"
#include "Pell_Equation.cpp"
#include "StringUtils.cpp"
#include "Utils.cpp"

using namespace Algebra;
using namespace Containers;

const ull N = 1000000000000;
const ull Q = powmod(2,32);

vector<ull> n235; // numbers of form 2^i * 3^j * 5^k <= N
vector<ull> s235; // sum of prefix of n235 modulo Q
vector<ull> P; // n235[i] + 1 is prime

int index (ull n) // finds max k: n235[k] <= n
{
    int lb = 0, ub = (int)n235.size() - 1, id = 0;
    while (lb <= ub) {
        int M = (lb + ub)/2;
        if (n235[M] <= n) { id = M; lb = M+1; }
        else ub = M-1;
    }
    return id;
}

ull res = 0;

void calculate_count (ull prime_factors, int iP) // iP is the min index in P we can use
{
    ull maxN = N/prime_factors; // guaranteed to be >= 1
    int k = index(maxN);
    res += s235[k] * (prime_factors % Q);
    res %= Q;
    for (int i=iP; i<(int)P.size(); i++) {
        if (P[i] > maxN) break;
        calculate_count(prime_factors * P[i], i+1);
    }
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
    
    set<ull> nums;
    queue<ull> q;
    q.push(1);
    
    while (!q.empty()) {
        ull n = q.front();
        q.pop();
        if (n > N) continue;
        if (nums.find(n) != nums.end()) continue;
        nums.insert(n);
        q.push(n*2); q.push(n*3); q.push(n*5);
    }
    
    n235 = vector<ull>(nums.begin(), nums.end());
    s235 = vector<ull>(n235.size());
    for (int i=0; i<(int)n235.size(); i++) {
        if (i == 0) s235[i] = n235[i];
        else s235[i] = (s235[i-1] + n235[i]) % Q;
        if (n235[i] >= 6 && n235[i] < N && primeQ(n235[i]+1)) P.push_back(n235[i]+1);
    }
    
//    show(n235);
//    show(s235);
//    show(P);
    calculate_count(1,0);
    ans = res;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
