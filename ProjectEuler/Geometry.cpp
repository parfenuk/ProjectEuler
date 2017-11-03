//
//  Geomerty.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 7/23/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

#include <iostream>
#include <math.h>

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;
typedef unsigned int uint;

#define pii pair<int,int>
#define pll pair<ll,ll>
#define puu pair<ull,ull>
#define pdd pair<dd, dd>
#define ppii pair<pair<int,int>, pair<int,int>>
#define ppll pair<pair<ll,ll>, pair<ll,ll>>
#define fs first
#define sc second

const dd PI = acos(-1.0);
const dd EPS = 0.00000001;

struct point
{
    dd x,y;
    
    point() { x = y = 0; }
    point(dd xx, dd yy) { x = xx; y = yy; }
    void read() { std::cin >> x >> y; }
    dd len() { return sqrt(x*x + y*y); }
    void set_length (dd L) {
        dd l = len();
        x *= (L/l);
        y *= (L/l);
    }
    dd polar_angle() {
        if (x == 0 && y == 0) return 0;
        if (x == 0) return y > 0 ? PI/2 : 3*PI/2;
        if (y == 0) return x > 0 ? 0 : PI;
        
        dd a = acos(x/len());
        if (y < 0) a = 2*PI - a;
        
        return a;
    }
}NOT_FOUND = point(-1000000000, -1000000000), INFINITELY_MANY(1000000000, 1000000000);

point make_point (dd xx, dd yy)
{
    point a;
    a.x = xx;
    a.y = yy;
    return a;
}

bool polar_angle_sort (point a, point b)
{
    return a.polar_angle() < b.polar_angle();
}

bool operator== (point a, point b)
{
    return a.x == b.x && a.y == b.y;
}

point operator+ (point a, point b)
{
    point c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

point operator- (point a, point b)
{
    point c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

point operator* (point a, dd k)
{
    point b;
    b.x = a.x*k;
    b.y = a.y*k;
    return b;
}

dd operator* (point a, point b)
{
    return a.x*b.x + a.y*b.y;
}

point operator/ (point a, dd k)
{
    point b;
    b.x = a.x/k;
    b.y = a.y/k;
    return b;
}

dd dist (point a, point b)
{
    return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}

dd dist_2 (point a, point b)
{
    return pow(a.x-b.x,2)+pow(a.y-b.y,2);
}

point symmetric_point (point a, point b)
{
    point c;
    c.x = 2*b.x - a.x;
    c.y = 2*b.y - a.y;
    return c;
}

dd triangle_area (point a, point b, point c)
{
    return 0.5*fabs((a.x-c.x)*(b.y-c.y) - (a.y-c.y)*(b.x-c.x));
}

bool triangle_contains_point (point a, point b, point c, point d) // abc contains d?
{
    return triangle_area(a,b,c) == triangle_area(a,b,d) + triangle_area(b,c,d) + triangle_area(a,c,d);
}

struct line
{
    dd A,B,C;
    
    line() { A = 1; B = -1; C = 0; } // y = x
    line(dd a, dd b, dd c) { A = a; B = b; C = c; }
    line(point a, point b);
    
    void shift_by_vector(point v) { C -= (A*v.x + B*v.y); }
    point any_point(dd y = 0) { return A == 0 ? point(0,-C/B) : point((C + B*y)/A, y); }
    point n() { return point(A,B); }
};

line::line(point a, point b)
{
    A = b.y - a.y;
    B = a.x - b.x;
    C = a.y*(b.x-a.x) - a.x*(b.y-a.y);
}

bool is_parallel_lines (line p, line q)
{
    return p.A*q.B == p.B*q.A;
}

dd dist (point a, line p)
{
    return fabs((p.A*a.x+p.B*a.y+p.C)/sqrt(p.A*p.A + p.B*p.B));
}

point projection (point a, line p)
{
    point b;
    b.x = (p.B*p.B*a.x - p.A*(p.C + p.B*a.y)) / (p.A*p.A + p.B*p.B);
    b.y = (p.A*p.A*a.y - p.B*(p.C + p.A*a.x)) / (p.A*p.A + p.B*p.B);
    return b;
}

point intersection_point (line p, line q)
{
    if (is_parallel_lines(p,q)) {
        
        return p.A*q.C == p.C*q.A ? INFINITELY_MANY : NOT_FOUND;
    }
    point a;
    a.x = (p.B*q.C - q.B*p.C) / (p.A*q.B - q.A*p.B);
    a.y = (p.A*q.C - q.A*p.C) / (q.A*p.B - p.A*q.B);
    return a;
}

line orthogonal_line (point a, line p)
{
    return line(-p.B, p.A, p.B*a.x - p.A*a.y);
}

point symmetric_point (point a, line p)
{
    point b = projection(a,p);
    return symmetric_point(a,b);
}

int Relation (point a, line p)
{
    if (fabs(a.x*p.A + a.y*p.B + p.C) < EPS) return 0;
    if (a.x*p.A + a.y*p.B + p.C > 0) return 1;
    return -1;
}

point reflection_vector (point v, line p, point a = NOT_FOUND)
{
    if (p.n()*v == 0) return NOT_FOUND;
    
    if (a == NOT_FOUND) a = p.any_point();
    return symmetric_point(a+v, p) - a;
}
