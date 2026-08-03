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

// a <= b <= c
pbb angles_ab (ll a, ll b, ll c) // < true if A >= 45 degrees, true if B > 45 degrees >
{
    pbb p;
    Lnum A(a), B(b), C(c);
    Lnum aa = B*B+C*C-A*A;
    p.fs = (aa*aa <= 2*B*B*C*C);
    Lnum bb = A*A+C*C-B*B;
    p.sc = (bb*bb < 2*A*A*C*C);
        
    return p;
}

ll bounding_square_side (ll a, ll b, ll c) // returns 0 if it's not an integer
{
    pbb p = angles_ab(a,b,c);
    if (!p.sc) return 0;
    
    Lnum A(a), B(b), C(c);
    
    ll S = Lnum::integer_sqrt((A+B-C)*(A-B+C)*(B+C-A)*(A+B+C)).to_long_long();
    if (S == 0) return 0;
    
    if (!p.fs) {
        ll sq = NumberUtils::integer_sqrt(b*b+c*c-S);
        if (sq == 0) return 0;
        if ((b*b+c*c-a*a)%(2*sq)) return 0;
        return (b*b+c*c-a*a)/(2*sq);
    }
    else {
        S /= 4;
        if (a*a <= 2*S) { // ha >= a
            if (2*S % a) return 0;
            return 2*S/a;
        }
        else { // ha < a
            ll sq = NumberUtils::integer_sqrt(a*a+b*b-4*S);
            if (sq == 0) return 0;
            if ((a*a+b*b-c*c)%(2*sq)) return 0;
            return (a*a+b*b-c*c)/(2*sq);
        }
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
    
    const int N = 1000000;
    vvpll P(N+1);
    
    for (ll p=2; p<=N/2; p++) for (ll q=1; q<p && 2*p*q<=N; q++) {
        if ((p+q)%2 == 0) continue;
        if (GCD(p,q) != 1) continue;
        ll a = p*p - q*q, b = 2*p*q, c = p*p + q*q;
        if (a > N) continue;
        if (a > b) swap(a,b);
        // now a <= b <= N
        for (ll t=1; b*t<=N; t++) {
            P[b*t].push_back(mp(a*t,c*t));
        }
    }
    
    vector<pair<pll,ll>> solutions;
    
    for (int n=1; n<=N; n++) {
        for (int i=0; i<(int)P[n].size(); i++) for (int j=i; j<(int)P[n].size(); j++) {
            pll p = P[n][i], q = P[n][j];
            // Family I
            ll a = p.fs + q.fs, b = p.sc, c = q.sc;
            if (a > b) swap(a,b); if (b > c) swap(b,c); if (a > b) swap(a,b);
            ll sq = bounding_square_side(a,b,c);
            if (sq && sq <= N) {
                //cout << a << " " << b << " " << c << endl;
                solutions.push_back(mp(mp(a,b),c));
            }
            // Family II
            a = p.sc; b = q.sc;
            c = NumberUtils::integer_sqrt((n-p.fs)*(n-p.fs) + (n-q.fs)*(n-q.fs));
            if (c == 0) continue;
            if (a > b) swap(a,b); if (b > c) swap(b,c); if (a > b) swap(a,b);
            sq = bounding_square_side(a,b,c);
            if (sq && sq <= N) {
                //cout << a << " " << b << " " << c << endl;
                solutions.push_back(mp(mp(a,b),c));
            }
        }
    }
    
    cout << solutions.size() << endl;
    sort(solutions.begin(), solutions.end());
    
    for (int i=0; i<(int)solutions.size(); i++) {
        if (i && solutions[i] == solutions[i-1]) continue;
        ans += solutions[i].fs.fs + solutions[i].fs.sc + solutions[i].sc;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
