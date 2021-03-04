//
//  Lnum.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 7/24/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

#include "Lnum.hpp"

const ltype BASE = 1000000000; // MUST be power of ten
const sint BASE_POWER = 9;

Lnum Lnum::O; // for some reasons I can't just use O instead of Lnum::O below
Lnum Lnum::one(1);

Lnum::Lnum (ll n)
{
    if (n < 0) { minus = true; n = -n; }
    else minus = false;
    size = 0;
    do {
        z[size++] = (n % BASE);
        n /= BASE;
    } while (n);
}

Lnum::Lnum (ull n)
{
    minus = false;
    size = 0;
    do {
        z[size++] = (n % BASE);
        n /= BASE;
    } while (n);
}

Lnum::Lnum (string S)
{
    if (S.empty()) { z[0] = 0; minus = false; size = 1; return; }
    if (S[0] == '-') {
        minus = true;
        S.erase(0,1);
    }
    else minus = false;
    size = 0;
    for (int i=(int)S.length(); i>0; i-=BASE_POWER) {
        if (i < BASE_POWER) z[size++] = (atoi(S.substr(0,i).c_str()));
        else z[size++] = (atoi(S.substr(i-BASE_POWER,BASE_POWER).c_str()));
    }
    while (size > 1 && z[size-1] == 0) size--;
}

Lnum::Lnum (const vector<int> &a)
{
    minus = false;
    size = 0;
    for (int i=(int)a.size()-1; i>=0; i--) {
        // TBD...
        //z[size++] = from_digits(subvector(a,i-BASE_POWER+1,i));
    }
}

// TODO: ostr << *this?
string Lnum::to_string()
{
    ostringstream ostr;
    if (minus) ostr << "-";
    ostr << (size ? z[size-1] : 0);
    for (int i=size-2; i>=0; i--) ostr << setfill('0') << setw(BASE_POWER) << z[i];
    
    return ostr.str();
}

// TODO: don't templates appliable here?
int Lnum::to_int()
{
    int s = 0, k = 1;
    for (int i=0; i<size; i++) {
        s += k*z[i];
        k *= BASE;
    }
    if (minus) s = -s;
    return s;
}

ll Lnum::to_long_long()
{
    ll s = 0, k = 1;
    for (int i=0; i<size; i++) {
        s += k*z[i];
        k *= BASE;
    }
    if (minus) s = -s;
    return s;
}

dd Lnum::to_double()
{
    dd s = 0, k = 1;
    for (int i=0; i<size; i++) {
        s += k*z[i];
        k *= BASE;
    }
    if (minus) s = -s;
    return s;
}

int Lnum::digits_count()
{
    int d = BASE_POWER*(size-1);
    int n = z[size-1];
    while (n) {
        n /= 10;
        d++;
    }
    return d;
}

vector<int> Lnum::digits()
{
    vector<int> a;
    // TODO: TBD
//    for (int i=size-1; i>=0; i--) {
//        vector<int> b = digits(z[i]);
//        if (i != size-1) while (b.size() < BASE_POWER) b.insert(b.begin(),0);
//        for (int j=0; j<(int)b.size(); j++) a.push_back(b[j]);
//    }
    
    return a;
}

Lnum Lnum::sqrt (const Lnum &A) // smallest x, that x*x <= A
{
    Lnum lb(1), ub = A, ret;
    while (lb <= ub) {
        Lnum M = (lb + ub) / 2;
        Lnum N = M*M;
        if (N <= A) { ret = M; lb = M + one; }
        else ub = M - one;
    }
    return ret;
}

bool operator== (const Lnum &A, const Lnum &B) // A == B ? 1 : 0
{
    if (A.isNegative() != B.isNegative()) return false;
    if (A.get_size() != B.get_size()) return false;
    for (int i=A.get_size()-1; i>=0; i--) if (A[i] != B[i]) return false;
    return true;
}

bool operator< (const Lnum &A, const Lnum &B) // A < B ? 1 : 0
{
    if (A.isNegative() ^ B.isNegative()) return A.isNegative();
    if (A.get_size() > B.get_size()) return A.isNegative();
    if (A.get_size() < B.get_size()) return !A.isNegative();
    for (int i=A.get_size()-1; i>=0; i--) {
        if (A[i] > B[i]) return A.isNegative();
        if (A[i] < B[i]) return !A.isNegative();
    }
    return false;
}

ostream& operator<< (ostream &os, const Lnum &A)
{
    if (A.minus) os << "-";
    os << (A.size ? A[A.size-1] : 0);
    for (int i=A.size-2; i>=0; i--) os << setfill('0') << setw(BASE_POWER) << A[i];
    return os;
}

Lnum operator+ (Lnum A, Lnum B)
{
    ltype carry = 0; bool minus;
    if (A.isNegative() == B.isNegative()) {
        minus = A.isNegative();
        for (int i=0; i<max(A.get_size(),B.get_size()) || carry; i++) {
            if (i == A.get_size()) A.push_back(0);
            A.set_value(i, A[i] + carry + (i < B.get_size() ? B[i] : 0));
            carry = A[i] >= BASE;
            if (carry) A.set_value(i, A[i] - BASE);
        }
    }
    else if (A.isPositive()) { // => B < 0
        B.change_sign();
        if (A >= B) minus = false;
        else { // A < B
            minus = true;
            bool minus_a = A.isNegative(), minus_b = B.isNegative();
            Lnum T = A; A = B; B = T;
            A.set_sign(minus_a);
            B.set_sign(minus_b);
        }
        for (int i=0; i<B.get_size() || carry; i++) {
            A.set_value(i, A[i] - carry - (i < B.get_size() ? B[i] : 0));
            carry = A[i] < 0;
            if (carry) A.set_value(i, A[i] + BASE);
        }
        while (A.get_size() > 1 && A.back() == 0) A.pop_back();
    }
    else { // A < 0, B >= 0
        return B + A;
    }
    
    A.set_sign(minus);
    return A;
}

Lnum operator- (Lnum A)
{
    A.change_sign();
    return A;
}

Lnum operator- (Lnum A, Lnum B)
{
    B.change_sign();
    return A + B;
}

Lnum operator* (Lnum A, ltype b) // b < BASE
{
    if (b == 0 || A == Lnum::O) return Lnum::O;
    
    ltype carry = 0;
    bool minus = A.isPositive() ^ (b > 0);
    if (b < 0) b = -b;
    
    for (int i=0; i<A.get_size() || carry; i++) {
        if (i == A.get_size()) A.push_back(0);
        ll cur = carry + A[i]*1ll*b;
        A.set_value(i, int(cur % BASE));
        carry = int(cur / BASE);
    }
    
    while (A.get_size() > 1 && A.back() == 0) A.pop_back();
    A.set_sign(minus);
    
    return A;
}

// TODO: templates again?
Lnum operator* (Lnum A, ll b)
{
    if (b == 0 || A == Lnum::O) return Lnum::O;
    
    ltype carry = 0;
    bool minus = A.isPositive() ^ (b > 0);
    if (b < 0) b = -b;
    
    for (int i=0; i<A.get_size() || carry; i++) {
        if (i == A.get_size()) A.push_back(0);
        ll cur = carry + A[i]*1ll*b;
        A.set_value(i, int(cur % BASE));
        carry = int(cur / BASE);
    }
    
    while (A.get_size() > 1 && A.back() == 0) A.pop_back();
    A.set_sign(minus);
    
    return A;
}

Lnum operator* (Lnum A, Lnum B)
{
    if (A == Lnum::O || B == Lnum::O) return Lnum::O;
    
    int size_c = A.get_size()+B.get_size();
    ltype *c = new ltype[size_c];
    for (int i=0; i<size_c; i++) c[i] = 0;
    
    for (int i=0; i<A.get_size(); i++)
    for (int j=0, carry=0; j<B.get_size() || carry; j++) {
        ll cur = c[i+j] + A[i]*1ll*(j < (int)B.get_size() ? B[j] : 0) + carry;
        c[i+j] = int(cur % BASE);
        carry = int(cur / BASE);
    }
    while (size_c > 1 && c[size_c-1] == 0) size_c--;
    
    Lnum C(c, size_c);
    C.set_sign(A.isPositive() ^ B.isPositive());
    
    delete[] c;
    
    return C;
}

Lnum operator/ (Lnum A, ltype b)
{
    if (A == Lnum::O) return Lnum::O;
    
    ltype carry = 0;
    bool minus = A.isPositive() ^ (b > 0);
    if (b < 0) b = -b;
    
    for (int i=A.get_size()-1; i>=0; i--) {
        ll cur = A[i] + carry*1ll*BASE;
        A.set_value(i, int (cur / b));
        carry = int (cur % b);
    }
    
    while (A.get_size() > 1 && A.back() == 0) A.pop_back();
    A.set_sign(minus);
    
    return A;
}

ltype operator% (Lnum A, ltype b) // assume that b > 0
{
    ltype carry = 0;
    for (int i=A.get_size()-1; i>=0; i--) {
        ll cur = A[i] + carry*1ll*BASE;
        A.set_value(i, int(cur / b));
        carry = int(cur % b);
    }
    if (A.isNegative()) carry = b - carry;
    if (carry == b) carry = 0;
    return carry;
}

pair<Lnum,Lnum> divmod (Lnum A, Lnum B) // < A/B, A%B >
{
    if (B.get_size() == 1) {
        return make_pair(A/B[0], A%B[0]);
    }
    
    bool neg_a = false, neg_b = false;
    if (A.isNegative()) {
        A.change_sign();
        neg_a = true;
    }
    if (B.isNegative()) {
        B.change_sign();
        neg_b = true;
    }
    
    Lnum lb(1), ub = A, M, res;
    while (true) {
        M = (lb + ub) / 2;
        if (B*M > A) ub = M - Lnum::one;
        else { // B*M <= A
            if (B*(M + Lnum::one) > A) { res = A - B*M; break; }
            lb = M + Lnum::one;
        }
    }
    
    if (neg_a) {
        if (res == Lnum::O) { if (!neg_b) M.change_sign(); }
        else {
            M = M + Lnum::one;
            res = B - res;
            if (!neg_b && M != Lnum::O) M.change_sign();
        }
    }
    else {
        if (neg_b && M != Lnum::O) M.change_sign();
    }
    
    return make_pair(M,res);
}

Lnum operator^ (Lnum A, ull k)
{
    Lnum B(1);
    
    while (k) {
        if (k%2 == 0) {
            k /= 2;
            A = A*A;
        }
        else {
            k--;
            B = B*A;
        }
    }
    if (A.isNegative() && k%2) B.change_sign();
    return B;
}
