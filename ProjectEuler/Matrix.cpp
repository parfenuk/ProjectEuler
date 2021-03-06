//
//  Matrix.cpp
//  ProjectEuler
//
//  Created by Miroslav.P on 12/7/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

namespace Matrix
{

#define MSIZE 2

struct matrix
{
    int N, Q; // all calculations are mod Q if it is set
    ll A[MSIZE][MSIZE];
    matrix (int n) {
        N = n; Q = 0;
        for (int i=0; i<N; i++) for (int j=0; j<N; j++) A[i][j] = 0;
        for (int i=0; i<N; i++) A[i][i] = 1;
    }
    matrix (ll **B, int n) {
        N = n; Q = 0;
        for (int i=0; i<N; i++) for (int j=0; j<N; j++) A[i][j] = B[i][j];
    }
    matrix (vvll a) {
        int n = (int)a.size();
        N = n;
        for (int i=0; i<N; i++) for (int j=0; j<N; j++) A[i][j] = a[i][j];
    }
    ll *operator[] (int k) { return A[k]; }
    void show() {
        for (int i=0; i<N; i++) {
            for (int j=0; j<N; j++) cout << A[i][j] << " ";
            cout << endl;
        }
    }
};

matrix operator+ (matrix a, matrix b)
{
    int N = a.N;
    matrix c(N);
    for (int i=0; i<N; i++) for (int j=0; j<N; j++) {
        c.A[i][j] = a[i][j] + b[i][j];
        if (a.Q) {
            c.A[i][j] %= a.Q;
            if (c.A[i][j] < 0) c.A[i][j] += a.Q;
        }
    }
    
    if (a.Q) c.Q = a.Q;
    return c;
}

matrix operator- (matrix a, matrix b)
{
    int N = a.N;
    matrix c(N);
    for (int i=0; i<N; i++) for (int j=0; j<N; j++) {
        c.A[i][j] = a[i][j] - b[i][j];
        if (a.Q) {
            c.A[i][j] %= a.Q;
            if (c.A[i][j] < 0) c.A[i][j] += a.Q;
        }
    }
    
    if (a.Q) c.Q = a.Q;
    return c;
}

matrix operator* (matrix a, matrix b)
{
    int N = a.N;
    matrix c(N);
    for (int i=0; i<N; i++) for (int j=0; j<N; j++) {
        
        ll s = 0;
        for (int t=0; t<N; t++) {
            s += (a[i][t]*b[t][j]);
            if (a.Q) {
                s %= a.Q;
                if (s < 0) s += a.Q;
            }
        }
        c.A[i][j] = s;
    }
    
    if (a.Q) c.Q = a.Q;
    return c;
}

matrix operator^ (matrix A, ull k)
{
    int N = A.N;
    matrix B(N);
    B.Q = A.Q;
    while (k) {
        if (k%2==0) {
            k /= 2;
            A = A*A;
        }
        else {
            k--;
            B = B*A;
        }
    }
    return B;
}

matrix operator^ (matrix A, Lnum k)
{
    int N = A.N;
    matrix B(N);
    B.Q = A.Q;
    while (k != Lnum::O) {
        if (k%2 == 0) {
            k = k/2;
            A = A*A;
        }
        else {
            k = k - Lnum::one;
            B = B*A;
        }
    }
    return B;
}

// let F[i] = initial_values[i], for i == 0..length(initial_values)
// F[k] = coeffs[0]*F[k-1] + coeffs[1]*F[k-2] + ...  + coeffs[l-1]*F[k-l]
// function finds F[N] mod Q in log(N)
// !size of initial_values must be not less than size of coeffs!
ll linear_recurrence_value (vll coeffs, vll initial_values, Lnum N, int Q)
{
    if (N.get_size() == 1 && N[0] < initial_values.size()) {
        ll f = initial_values[N[0]] % Q;
        if (f < 0) f += Q;
        return f;
    }
    
    int m = (int)coeffs.size();
    matrix M(m);
    M.Q = Q;
    for (int i=0; i<m; i++) M.A[i][i] = 0;
    for (int i=1; i<m; i++) M.A[i][i-1] = 1;
    for (int j=0; j<m; j++) {
        M.A[0][j] = coeffs[j] % Q;
        if (M.A[0][j] < 0) M.A[0][j] += Q;
    }
    
    M = M^(N-Lnum(m)+1);
    ll ans = 0;
    for (int j=0; j<m; j++) {
        ans += M[0][j]*initial_values[m-j-1];
        ans %= Q;
    }
    
    return ans;
}

}
