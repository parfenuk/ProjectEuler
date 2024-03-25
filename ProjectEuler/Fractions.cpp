//
//  Fractions.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 9/21/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

#include "Lnum.hpp"

namespace Fractions
{

typedef ll ftype;

struct fraction {
    
    ftype num, den;
    
    fraction() { num = 0; den = 1; }
    fraction(ftype n) { num = n; den = 1; }
    fraction(ftype n, ftype d) { num = n; den = d; }
    
    static fraction O;
    static fraction one;
    
    void reduce() {
        ftype p = Algebra::signedGCD(num, den);
        num /= p;
        den /= p;
    }
    void show (bool show_endl = true) { cout << num << "/" << den; if (show_endl) cout << endl; }
};

fraction fraction::O;
fraction fraction::one(1);

bool operator< (fraction a, fraction b) { return a.num*b.den < b.num*a.den; }
bool operator== (fraction a, fraction b) { return a.num*b.den == b.num*a.den; }
bool operator<= (fraction A, fraction B) { return A < B || A == B; }
bool operator>  (fraction A, fraction B) { return !(A <= B); }
bool operator>= (fraction A, fraction B) { return !(A < B); }
bool operator!= (fraction A, fraction B) { return !(A == B); }

fraction operator+ (fraction a, fraction b)
{
    ftype d = Algebra::signedGCD(a.den, b.den);
    ftype a_mult = b.den/d, b_mult = a.den/d;
    fraction c (a.num*a_mult + b.num*b_mult, a.den*a_mult);
    c.reduce();
    return c;
}

fraction operator- (fraction a, fraction b)
{
    ftype d = Algebra::signedGCD(a.den, b.den);
    ftype a_mult = b.den/d, b_mult = a.den/d;
    fraction c(a.num*a_mult - b.num*b_mult, a.den*a_mult);
    c.reduce();
    return c;
}

fraction operator* (fraction a, ftype b)
{
    fraction c(a.num*b, a.den);
    c.reduce();
    return c;
}

fraction operator* (fraction a, fraction b)
{
    fraction c(a.num*b.num, a.den*b.den);
    c.reduce();
    return c;
}

fraction operator/ (fraction a, ftype b)
{
    fraction c(a.num, a.den*b);
    c.reduce();
    return c;
}

fraction operator/ (fraction a, fraction b)
{
    fraction c(a.num*b.den, a.den*b.num);
    c.reduce();
    return c;
}

struct Lfraction {
    
    Lnum num, den;
    
    Lfraction() { num = Lnum::O; den = Lnum::one; }
    Lfraction(ll n) { num = n; den = 1; }
    Lfraction(ll n, ll d) { num = n; den = d; }
    Lfraction(Lnum n, Lnum d) { num = n; den = d; }
    
    static Lfraction O;
    static Lfraction one;
    
    void reduce() {
        Lnum p = Lnum::GCD(num, den);
        num = Lnum::divmod(num,p).fs;
        den = Lnum::divmod(den,p).fs;
    }
    void show (bool show_endl = true) { cout << num << "/" << den; if (show_endl) cout << endl; }
};

Lfraction Lfraction::O;
Lfraction Lfraction::one(1);

bool operator< (Lfraction a, Lfraction b) { return a.num*b.den < b.num*a.den; }
bool operator== (Lfraction a, Lfraction b) { return a.num*b.den == b.num*a.den; }
bool operator<= (Lfraction A, Lfraction B) { return A < B || A == B; }
bool operator>  (Lfraction A, Lfraction B) { return !(A <= B); }
bool operator>= (Lfraction A, Lfraction B) { return !(A < B); }
bool operator!= (Lfraction A, Lfraction B) { return !(A == B); }

Lfraction operator+ (Lfraction a, Lfraction b)
{
    Lnum d = Lnum::GCD(a.den, b.den);
    Lnum a_mult = Lnum::divmod(b.den,d).fs, b_mult = Lnum::divmod(a.den,d).fs;
    Lfraction c (a.num*a_mult + b.num*b_mult, a.den*a_mult);
    c.reduce();
    return c;
}

Lfraction operator- (Lfraction a, Lfraction b)
{
    Lnum d = Lnum::GCD(a.den, b.den);
    Lnum a_mult = Lnum::divmod(b.den,d).fs, b_mult = Lnum::divmod(a.den,d).fs;
    Lfraction c (a.num*a_mult - b.num*b_mult, a.den*a_mult);
    c.reduce();
    return c;
}

Lfraction operator* (Lfraction a, int b)
{
    Lfraction c(a.num*b, a.den);
    c.reduce();
    return c;
}

Lfraction operator* (Lfraction a, Lnum b)
{
    Lfraction c(a.num*b, a.den);
    c.reduce();
    return c;
}

Lfraction operator* (Lfraction a, Lfraction b)
{
    Lfraction c(a.num*b.num, a.den*b.den);
    c.reduce();
    return c;
}

Lfraction operator/ (Lfraction a, int b)
{
    Lfraction c(a.num, a.den*b);
    c.reduce();
    return c;
}

Lfraction operator/ (Lfraction a, Lnum b)
{
    Lfraction c(a.num, a.den*b);
    c.reduce();
    return c;
}

Lfraction operator/ (Lfraction a, Lfraction b)
{
    Lfraction c(a.num*b.den, a.den*b.num);
    c.reduce();
    return c;
}

Lfraction fr_sum (Lnum from, Lnum to)
{
    Lfraction F;
    for (Lnum i = from; i <= to; i += Lnum::one) {
        F = F + Lfraction(Lnum::one,i);
    }
    return F;
}

}
