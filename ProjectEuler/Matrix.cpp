//
//  Matrix.cpp
//  ProjectEuler
//
//  Created by Miroslav.P on 12/7/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;

struct Matrix
{
    int N, Q;
    ll **A;
    Matrix (int n) {
        N = n; Q = 0;
        A = new ll *[n];
        for (int i=0; i<N; i++) A[i] = new ll [n];
        for (int i=0; i<N; i++) for (int j=0; j<N; j++) A[i][j] = 0;
        for (int i=0; i<N; i++) A[i][i] = 1;
    }
    Matrix(ll **B, int n) {
        N = n; Q = 0;
        A = new ll *[n];
        for (int i=0; i<N; i++) A[i] = new ll [n];
        for (int i=0; i<N; i++) for (int j=0; j<N; j++) A[i][j] = B[i][j];
    }
    ll *operator[] (int k) { return A[k]; }
    void show() {
        for (int i=0; i<N; i++) {
            for (int j=0; j<N; j++) cout << A[i][j] << " ";
            cout << endl;
        }
    }
};

Matrix operator+ (Matrix a, Matrix b)
{
    int N = a.N;
    Matrix c(N);
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

Matrix operator- (Matrix a, Matrix b)
{
    int N = a.N;
    Matrix c(N);
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

Matrix operator* (Matrix a, Matrix b)
{
    int N = a.N;
    Matrix c(N);
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

Matrix power (Matrix A, ull k)
{
    int N = A.N;
    Matrix B(N);
    B.Q = A.Q;
    while (k) {
        cout << k << endl;
        if (k%2==0) {
            k /= 2;
            A = A*A; // [ a = (a*a)%n; ]
        }
        else {
            k--;
            B = B*A; // [ b = (b*a)%n; ]
        }
    }
    return B;
}
