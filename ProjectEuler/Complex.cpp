//
//  Complex.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 8/26/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

namespace Complex
{

struct complex {
    
    ll x,y;
    
    ll norm() { return x*x + y*y; }
    complex() { x = y = 0; }
    complex(ll _x) { x = _x; y = 0; }
    complex(ll _x, ll _y) { x = _x; y = _y; }
    complex conjugate() { complex z; z.x = x; z.y = -y; return z; }
    
    static complex O;
    
    void show() {
        
        if (y == 0) cout << x;
        else if (x == 0) {
            if (y == -1) cout << "-i";
            else if (y == 1) cout << "i";
            else cout << y << "i";
        }
        else {
            cout << x;
            if (y > 0) cout << "+";
            if (y == -1) cout << "-i";
            else if (y == 1) cout << "i";
            else cout << y << "i";
        }
        cout << endl;
    }
};

complex complex::O;

complex operator+ (complex a, complex b)
{
    return complex(a.x+b.x,a.y+b.y);
}

complex operator- (complex a, complex b)
{
    return complex(a.x-b.x,a.y-b.y);
}

complex operator* (complex a, ll k)
{
    return complex(a.x*k,a.y*k);
}

complex operator* (complex a, complex b)
{
    return complex(a.x*b.x-a.y*b.y, a.y*b.x+a.x*b.y);
}

complex operator/ (complex a, complex b) // integer-valued division with remainder!!!
{
    complex c;
    ll A = a.x*b.x + a.y*b.y;
    ll B = b.x*b.x + b.y*b.y;
    int sign = 1;
    if (A < 0) { sign = -sign; A = -A; }
    if (2*(A%B) <= B) c.x = A / B;
    else c.x = A / B + 1;
    c.x *= sign;
    sign = 1;
    A = b.x*a.y - a.x*b.y;
    if (A < 0) { sign = -sign; A = -A; }
    if (2*(A%B) <= B) c.y = A / B;
    else c.y = A / B + 1;
    c.y *= sign;
    
    return c;
}

complex operator% (complex a, complex b) // integer-valued division with remainder!!!
{
    complex c;
    ll A = a.x*b.x + a.y*b.y;
    ll B = b.x*b.x + b.y*b.y;
    int sign = 1;
    if (A < 0) { sign = -sign; A = -A; }
    if (2*(A%B) <= B) c.x = A / B;
    else c.x = A / B + 1;
    c.x *= sign;
    sign = 1;
    A = b.x*a.y - a.x*b.y;
    if (A < 0) { sign = -sign; A = -A; }
    if (2*(A%B) <= B) c.y = A / B;
    else c.y = A / B + 1;
    c.y *= sign;
    
    return a - b*c;
}

complex power (complex a, int k)
{
    complex s(1);
    for (int i=0; i<k; i++) s = s*a;
    return s;
}

}
