//
//  Geomerty.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 7/23/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

namespace Geometry3D
{

// TODO: move to constants
const dd PI = acos(-1.0);
const dd EPS = 0.000000001;

struct point
{
    dd x,y,z;
    
    point() { x = y = z = 0; }
    point(dd xx, dd yy, dd zz) { x = xx; y = yy; z = zz; }
    void read() { cin >> x >> y >> z; }
    dd len() { return sqrt(x*x + y*y + z*z); }
    void set_length (dd L) {
        dd l = len();
        x *= (L/l);
        y *= (L/l);
        z *= (L/l);
    }
};

point make_point (dd xx, dd yy, dd zz)
{
    point a;
    a.x = xx;
    a.y = yy;
    a.z = zz;
    return a;
}

bool operator== (point a, point b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator< (point a, point b) {
    return a.x < b.x || (a.x == b.x && (a.y < b.y || (a.y == b.y && a.z < b.z)));
}

point operator+ (point a, point b)
{
    point c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    return c;
}

point operator- (point a, point b)
{
    point c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    return c;
}

point operator* (point a, dd k)
{
    point b;
    b.x = a.x*k;
    b.y = a.y*k;
    b.z = a.z*k;
    return b;
}

dd operator* (point a, point b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

point operator/ (point a, dd k)
{
    point b;
    b.x = a.x/k;
    b.y = a.y/k;
    b.z = a.z/k;
    return b;
}

point operator^ (point a, point b)
{
    return make_point(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

dd dist (point a, point b)
{
    return sqrt(pow(a.x-b.x,2) + pow(a.y-b.y,2) + pow(a.z-b.z,2));
}

dd dist_2 (point a, point b)
{
    return pow(a.x-b.x,2) + pow(a.y-b.y,2) + pow(a.z-b.z,2);
}

point symmetric_point (point a, point b)
{
    point c;
    c.x = 2*b.x - a.x;
    c.y = 2*b.y - a.y;
    c.z = 2*b.z - a.z;
    return c;
}

struct plane
{
    dd A,B,C,D;
    
    plane() { A = 1; B = 1; C = 1; D = 0; } // x + y + z = 0;
    plane(dd a, dd b, dd c, dd d) { A = a; B = b; C = c; D = d; }
    plane(point a, point b, point c);
    
    point n() { return point(A,B,C); }
    bool contains_point (point a) { return fabs(A*a.x + B*a.y + C*a.z + D) < EPS; }
};

plane::plane (point a, point b, point c)
{
    A = a.y*b.z + a.z*c.y + b.y*c.z - a.z*b.y - b.z*c.y - a.y*c.z;
    B = a.z*b.x + b.z*c.x + a.x*c.z - b.x*c.z - a.x*b.z - a.z*c.x;
    C = a.x*b.y + a.y*c.x + b.x*c.y - b.y*c.x - a.x*c.y - a.y*b.x;
    D = -A*a.x - B*a.y - C*a.z;
}

int Relation (point a, plane p)
{
    if (fabs(a.x*p.A + a.y*p.B + a.z*p.C + p.D) < EPS) return 0;
    if (a.x*p.A + a.y*p.B + a.z*p.C + p.D > 0) return 1;
    return -1;
}

dd dist (point a, plane p)
{
    return fabs((p.A*a.x+p.B*a.y+p.C*a.z + p.D)/sqrt(p.A*p.A + p.B*p.B + p.C*p.C));
}

pair<point,point> intersection (plane p, plane q) // returns <point,vector> of intersection line of planes. Must not be parallel
{
    point a;
    if (fabs(q.B*p.C - p.B*q.C) > EPS) {
        a = make_point(0, (q.C*p.D - p.C*q.D)/(q.B*p.C - p.B*q.C), (p.B*q.D - q.B*p.D)/(q.B*p.C - p.B*q.C));
    }
    else if (fabs(q.A*p.C - p.A*q.C) > EPS) {
        a = make_point((q.C*p.D - p.C*q.D)/(q.A*p.C - p.A*q.C), 0, (p.A*q.D - q.A*p.D)/(q.A*p.C - p.A*q.C));
    }
    else {
        a = make_point((q.B*p.D - p.B*q.D)/(q.A*p.B - p.A*q.B), (p.A*q.D - q.A*p.D)/(q.A*p.B - p.A*q.B), 0);
    }
    point v = p.n()^q.n();
    return make_pair(a,v);
}

// actually, there're two angles with the sum equals PI
// point a belongs P, b belongs Q, so they denote the quarter for which to calculate the angle
dd dihedral_angle (plane P, plane Q, point a, point b)
{
    pair<point,point> zzz = intersection(P,Q);
    point p = zzz.fs, v = zzz.sc;
    point va = v^P.n();
    if (Relation(p+va,Q) != Relation(a,Q)) va = va*(-1);
    point vb = v^Q.n();
    if (Relation(p+vb,P) != Relation(b,P)) vb = vb*(-1);
    
    dd cosa = va*vb / (va.len()*vb.len());
    if (cosa < -1) cosa = -1;
    if (cosa > 1) cosa = 1;
    
    return acos(cosa);
}

}
