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
    Eratosthenes_sieve(N, true);
    
    dd res = 0;
    vbool used(N);
    
    for (ull n=3; n<=N; n+=10) {
        vpull F = factorize(n);
        if (F.size() == 1) used[F[0].fs] = true;
    }
    
    for (ull n=3; n<=N; n+=10) {
        vpull F = factorize(n);
        bool already = false;
        for (int i=0; i<(int)F.size(); i++) {
            if (used[F[i].fs]) { already = true; break; }
        }
        if (!already && F.size() == 2) {
            cout << n << ": ";
            for (int i=0; i<(int)F.size(); i++) cout << F[i].fs << " "; cout << endl;
        }
    }
    
    vull D;
    for (ull n=3; n<=N; n+=10) {
        bool coprime = true;
        for (int i=0; i<(int)D.size(); i++) {
            if (n % D[i] == 0) { coprime = false; break; }
        }
        if (coprime) {
            vpull F = factorize(n);
            D.push_back(F[0].fs);
            res += log((dd)F[0].fs) / log(2.718281828459);
        }
        vpull F = factorize(n);
        cout << n << ": ";
        for (int i=0; i<(int)F.size(); i++) cout << F[i].fs << " "; cout << endl;
    }
    
    res -= log(37.0) / log(2.718281828459);
    res -= log(47.0) / log(2.718281828459);
    res += log(59.0) / log(2.718281828459);
    cout << fixed << res;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
