//
//  Lnum.hpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/4/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

#pragma once

#include "Definitions.hpp"
#include "NumberUtils.hpp"

typedef int ltype;

#define LSIZE 10

class Lnum
{
    int size;
    bool minus;
    ltype z[LSIZE];
    
public:
    
    Lnum() { z[0] = 0; minus = false; size = 1;}
    Lnum(ltype n) { if (n < 0) { minus = true; n = -n; } else minus = false; z[0] = n; size = 1; }
    Lnum(ll n);
    Lnum(ull n);
    Lnum(string S);
    Lnum(const vector<ltype> &a) { size = (int)a.size(); for (int i=0; i<size; i++) z[i] = a[i]; minus = false; }
    Lnum(ltype a[], int s) { size = s; for (int i=0; i<s; i++) z[i] = a[i]; minus = false; }
    
    string to_string();
    ll to_long_long();
    int to_int();
    dd to_double();
    vint digits();
    int digits_count();
    
    int get_size()    const { return size; }
    ltype back()      const { return z[size-1]; }
    bool isPositive() const { return !minus; } // 0 is considered to be positive
    bool isNegative() const { return minus; }
    
    void set_value (int pos, ltype k) { z[pos] = k; }
    void push_back (ltype n)          { z[size++] = n; }
    void pop_back()                   { z[--size] = 0; if (!size) size++; }
    void change_sign()                { if (size != 1 || z[0]) minus = !minus; }
    void set_sign (bool sign)         { if (size == 1 && z[0] == 0) minus = false; else minus = sign; }
    
    static Lnum abs (Lnum A)  { A.set_sign(false); return A; }
    static Lnum sqrt (const Lnum &A);
    
    static Lnum O;
    static Lnum one;
    
    friend istream& operator>> (istream &is, Lnum &A);
    friend ostream& operator<< (ostream &os, const Lnum &A);
    
    friend bool operator== (const Lnum &A, const Lnum &B);
    friend bool operator<  (const Lnum &A, const Lnum &B);
    friend bool operator<= (const Lnum &A, const Lnum &B) { return !(B < A); }
    friend bool operator>  (const Lnum &A, const Lnum &B) { return B < A; }
    friend bool operator>= (const Lnum &A, const Lnum &B) { return !(A < B); }
    friend bool operator!= (const Lnum &A, const Lnum &B) { return !(A == B); }
    
    friend Lnum operator+ (Lnum A, Lnum B);
    friend Lnum operator- (Lnum A);
    friend Lnum operator- (Lnum A, Lnum B);
    friend Lnum operator* (Lnum A, ltype b);
    friend Lnum operator* (Lnum A, Lnum B);
    friend Lnum operator/ (Lnum A, ltype b);
    friend ltype operator% (Lnum A, ltype b);
    friend Lnum operator^ (Lnum A, ull k);
    
    ltype operator[] (int k)   const { return z[k]; }
    Lnum& operator+= (Lnum B)  { *this = *this + B; return *this; }
    Lnum& operator-= (Lnum B)  { *this = *this - B; return *this; }
    Lnum& operator*= (ltype b) { *this = *this * b; return *this; }
    Lnum& operator*= (Lnum B)  { *this = *this * B; return *this; }
    Lnum& operator/= (ltype b) { *this = *this / b; return *this; }
    
    Lnum& operator++() { *this = *this + one; return *this; }
    Lnum operator++(int) { Lnum clone = *this; operator++(); return clone; }
    Lnum& operator--() { *this = *this - one; return *this; }
    Lnum operator--(int) { Lnum clone = *this; operator--(); return clone; }
    
    static pair<Lnum,Lnum> divmod (Lnum A, Lnum B);
};
