//
//  Fractions.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 9/21/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;

#define pii pair<int,int>
#define pll pair<ll,ll>
#define puu pair<ull,ull>
#define pdd pair<dd, dd>
#define ppii pair<pair<int,int>, pair<int,int>>
#define ppll pair<pair<ll,ll>, pair<ll,ll>>
#define fs first
#define sc second

ull f_GCD (ull a, ull b)
{
    while (a && b) {
        
        if (a > b) a %= b;
        else b %= a;
    }
    return a + b;
}

struct fraction { // unsigned only
    
    ull num, den;
    fraction() { num = 0; den = 1; }
    fraction(ull n) { num = n; den = 1; }
    fraction(ull n, ull d) { num = n; den = d; }
    void reduce() {
        ull p = f_GCD(num, den);
        num /= p;
        den /= p;
    }
    void show(bool show_endl = true) { cout << num << "/" << den; if (show_endl) cout << endl; }
}f_O, f_one(1);

bool operator== (fraction a, fraction b)
{
    return a.num*b.den == b.num*a.den;
}

bool operator< (fraction a, fraction b)
{
    return a.num*b.den < b.num*a.den;
}

bool operator<= (fraction A, fraction B) { return A < B || A == B; }
bool operator> (fraction A, fraction B) { return !(A <= B); }
bool operator>= (fraction A, fraction B) { return !(A < B); }
bool operator!= (fraction A, fraction B) { return !(A == B); }

fraction operator+ (fraction a, fraction b)
{
    ull d = f_GCD(a.den, b.den);
    ull a_mult = b.den/d, b_mult = a.den/d;
    fraction c (a.num*a_mult + b.num*b_mult, a.den*a_mult);
    c.reduce();
    return c;
}

fraction operator- (fraction a, fraction b)
{
    ull d = f_GCD(a.den, b.den);
    ull a_mult = b.den/d, b_mult = a.den/d;
    fraction c (a.num*a_mult - b.num*b_mult, a.den*a_mult);
    c.reduce();
    return c;
}

fraction operator* (fraction a, ull b)
{
    fraction c (a.num*b, a.den);
    c.reduce();
    return c;
}

fraction operator* (fraction a, fraction b)
{
    fraction c (a.num*b.num, a.den*b.den);
    c.reduce();
    return c;
}

fraction operator/ (fraction a, ull b)
{
    fraction c (a.num, a.den*b);
    c.reduce();
    return c;
}

fraction operator/ (fraction a, fraction b)
{
    fraction c (a.num*b.den, a.den*b.num);
    c.reduce();
    return c;
}
