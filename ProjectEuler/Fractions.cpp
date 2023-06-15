//
//  Fractions.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 9/21/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

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
        ll p = Algebra::signedGCD(num, den);
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
    ll d = Algebra::signedGCD(a.den, b.den);
    ll a_mult = b.den/d, b_mult = a.den/d;
    fraction c (a.num*a_mult + b.num*b_mult, a.den*a_mult);
    c.reduce();
    return c;
}

fraction operator- (fraction a, fraction b)
{
    ll d = Algebra::signedGCD(a.den, b.den);
    ll a_mult = b.den/d, b_mult = a.den/d;
    fraction c(a.num*a_mult - b.num*b_mult, a.den*a_mult);
    c.reduce();
    return c;
}

fraction operator* (fraction a, ll b)
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

fraction operator/ (fraction a, ll b)
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

}
