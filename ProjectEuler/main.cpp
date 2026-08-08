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
    
    const ll N = 10000000;
    const int Q = (int)powmod(11,8); // 214358881
    const int phiQ = (int)EulerPhi(Q); // 194871710
    
    Eratosthenes_sieve(N);
    vchar mu = MoebuisMuSieve(N);
    EulerPhiSieve(N);
    
    Lnum T(-1); // total number of points (excluding (1,1,1) -> (0,0))
    for (ll d=1; d<=N; d++) {
        Lnum A(N/d+1);
        T += mu[d]*((A^3)-1);
    }
    
    vll F(N+1); // how many points lie on a ray with characteristic D
    // there're 6*phi(D) such rays
    for (ll D=1; D<=N; D++) {
        for (ll d=1; d<=N/D; d++) {
            ll kd = N/D/d;
            ll s = kd*(N/d+1) - D*kd*(kd+1)/2;
            F[D] += mu[d]*s; F[D] %= phiQ; if (F[D] < 0) F[D] += phiQ;
        }
    }
     
    // and then just iterate over all rays to calculate the answer
    ans = powmod(2,T%phiQ+1,Q);
    ll t = T/2 % phiQ;
    for (ll D=1; D<=N; D++) {
        ll s = powmod(2,F[D],Q)-1;
        ll pow_phi = (t - F[D]) % phiQ;
        if (pow_phi < 0) pow_phi += phiQ;
        s *= powmod(2,pow_phi,Q); s %= Q;
        s *= 6*eulerPhi[D]; s %= Q;
        if (s < 0) s += Q;
        ans = (ans + Q - s)%Q;
    }
    ans--;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
