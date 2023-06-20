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

const int m=-1000,M=1000;

point random_point()
{
    return point(Utils::random_integer(m,M),Utils::random_integer(m,M));
}

line random_line()
{
    while (true) {
        point p1 = random_point(), p2 = random_point();
        if (p1 != p2) return line(p1,p2);
    } return line();
}

// ATTRACTIONS + INTENTIONS = REGENERATION
// ACEGINOSTR
// 0123456789

vector<int> s1;
vector<int> s2;
vector<int> sum;

void fill_words (string S, string T, string U)
{
    vector<int> matches(26,-1);
    int K = 0;
    vector<string> w = { S,T,U };
    for (int j=0; j<3; j++)
    for (int i=0; i<(int)w[j].length(); i++) {
        if (matches[w[j][i]-'A'] == -1) matches[w[j][i]-'A'] = K++;
    }
    vector<int> v[3];
    for (int j=0; j<3; j++)
    for (int i=0; i<(int)w[j].length(); i++) {
        v[j].push_back(matches[w[j][i]-'A']);
    }
    s1 = v[0]; s2 = v[1]; sum = v[2];
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(3);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    fill_words("DONALD", "GERALD", "ROBERT");
    
    vint d1(s1.size()), d2(s2.size()), ds(sum.size());
    vector<int> a = {0,1,2,3,4,5,6,7,8,9};
    do {
        for (int i=0; i<(int)s1.size(); i++) d1[i] = a[s1[i]];
        for (int i=0; i<(int)s2.size(); i++) d2[i] = a[s2[i]];
        for (int i=0; i<(int)sum.size(); i++) ds[i] = a[sum[i]];
        ull n1 = NumberUtils::from_digits(d1);
        ull n2 = NumberUtils::from_digits(d2);
        ull ns = NumberUtils::from_digits(ds);
        if (n1+n2 == ns) {
            cout << n1 << " + " << n2 << " = " << ns << endl;
        }
    } while (next_permutation(a.begin(), a.end()));
        
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
