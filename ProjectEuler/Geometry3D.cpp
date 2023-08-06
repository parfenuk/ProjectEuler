//
//  Geomerty.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 7/23/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

namespace Geometry3D
{

typedef dd gtype;

struct point
{
    gtype x,y,z;
    
    point() { x = y = z = 0; }
    point(gtype xx, gtype yy, gtype zz) { x = xx; y = yy; z = zz; }
    
    void read() { cin >> x >> y >> z; }
    void show() { cout << x << " " << y << " " << z << endl; }
    
    dd len() { return sqrt(x*x + y*y + z*z); }
    gtype len2() { return x*x + y*y + z*z; }
    
    void set_length (dd L) {
        dd l = len();
        x *= (L/l);
        y *= (L/l);
        z *= (L/l);
    }
}NOT_FOUND = point(-1000000000, -1000000000, -1000000000);

istream& operator>> (istream &is, point &a) { is >> a.x >> a.y >> a.z; return is; }
ostream& operator<< (ostream &os, point &a) { os << a.x << " " << a.y << " " << a.z; return os; }

bool operator== (point a, point b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
bool operator!= (point a, point b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
bool operator< (point a, point b) {
    return a.x < b.x || (a.x == b.x && (a.y < b.y || (a.y == b.y && a.z < b.z)));
}

point operator+ (point a, point b) { return point(a.x+b.x, a.y+b.y, a.z+b.z); }
point operator+ (point a, gtype k) { return point(a.x+k, a.y+k, a.z+k); }
point operator- (point a, point b) { return point(a.x-b.x, a.y-b.y, a.z-b.z); }
point operator- (point a, gtype k) { return point(a.x-k, a.y-k, a.z-k); }
point operator* (point a, gtype k) { return point(a.x*k, a.y*k, a.z*k); }
point operator/ (point a, gtype k) { return point(a.x/k, a.y/k, a.z/k); }
gtype operator* (point a, point b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
point operator^ (point a, point b) { return point(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }
bool operator| (vec a, vec b) { return (a^b) == point(); }

void operator+= (point &a, point b) { a = a+b; }
void operator-= (point &a, point b) { a = a-b; }
void operator*= (point &a, gtype k) { a = a*k; }
void operator/= (point &a, gtype k) { a = a/k; }

dd dist (point a, point b) { return (a-b).len(); }
gtype dist2 (point a, point b) { return (a-b).len2(); }

point symmetric_point (point a, point b) { return b+b-a; }

struct line
{
    point a;
    vec dv;
    line() {}
    line(point _a, point _v) { a = _a; dv = _v; }
    point at (dd t) { return a+dv*t; }
};

dd dist (point a, line p) { return ((p.a-a)^p.dv).len() / p.dv.len(); }
point projection (point a, line p) { dd t = (a-p.a)*p.dv/p.dv.len2(); return p.a + p.dv*t; }

struct plane
{
    gtype A,B,C,D;
    
    plane() { A = 1; B = 1; C = 1; D = 0; } // x + y + z = 0;
    plane(dd a, dd b, dd c, dd d) { A = a; B = b; C = c; D = d; }
    plane(point a, point b, point c) {
        A = a.y*(b.z-c.z) + b.y*(c.z-a.z) + c.y*(a.z-b.z);
        B = a.z*(b.x-c.x) + b.z*(c.x-a.x) + c.z*(a.x-b.x);
        C = a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y);
        D = -A*a.x - B*a.y - C*a.z;
    }
    
    point n (dd l = 0) const { point p(A,B,C); if (l) p.set_length(l); return p; } // normal vector
    gtype at (point a) const { return A*a.x + B*a.y + C*a.z + D; }
    void shift_by_vector (vec v) { D -= (A*v.x + B*v.y + C*v.z); }
    pair<vec,vec> dvs() const { // two orthogonal directed vectors
        if (A || B) return mp(point(B,-A,0), point(A*C, B*C, -A*A-B*B));
        if (A || C) return mp(point(C,0,-A), point(A*B, -A*A-C*C, B*C));
        return mp(point(0,C,-B), point(-B*B-C*C, A*B, A*C));
    }
        
    sint relation (point a) {
        gtype d = at(a);
        if (d == 0) return 0; // optionally can be changed to EPS
        if (d < 0) return -1;
        return 1;
    }
    
    bool contains (point a) { return relation(a) == 0; }
};

bool operator| (plane p, line q) { return p.n()*q.dv == 0; }

dd dist (point a, plane p) { return fabs(p.at(a)) / p.n().len(); }
point projection (point a, plane p) { dd t = p.at(a) / p.n().len2(); return a - p.n()*t; }

plane orthogonal_plane (point a, line p)
{
    plane q(p.dv.x, p.dv.y, p.dv.z, 0);
    q.D = -q.at(a);
    return q;
}

// *** INTERSECTIONS (& operators) ***

point operator& (plane p, line l)
{
    dd sp = p.n()*l.dv;
    if (sp == 0) return NOT_FOUND; // TODO: INFINITELY_MANY case
    
    dd t = p.at(l.a) / sp;
    return l.a - l.dv*t;
}

line operator& (plane p, plane q) // returns <point,vector> of intersection line of planes. Must not be parallel
{
    point a;
    
    dd prodAB = q.A*p.B - p.A*q.B;
    dd prodAC = q.A*p.C - p.A*q.C;
    dd prodBC = q.B*p.C - p.B*q.C;
    
    if (fabs(prodBC) > EPS) a = point(0, q.C*p.D - p.C*q.D, p.B*q.D - q.B*p.D) / prodBC;
    else if (fabs(prodAC) > EPS) a = point(q.C*p.D - p.C*q.D, 0, p.A*q.D - q.A*p.D) / prodAC;
    else a = point(q.B*p.D - p.B*q.D, p.A*q.D - q.A*p.D, 0) / prodAB;
    
    point v = p.n()^q.n();
    return line(a,v);
}

pair<point,point> operator& (sphere c, line p)
{
    point c0 = c.fs; dd R = c.sc;
    point pr = projection(c0,p);
    dd D = dist(c0,pr);
    if (D > R) return mp(NOT_FOUND,NOT_FOUND);

    p.a -= c0; pr -= c0; // move circle into zero point
    dd L = sqrt(R*R - D*D);
    p.dv.set_length(L);
    return mp(pr+c0-p.dv,pr+c0+p.dv);
}

// actually, there're two angles with the sum equals PI
// point a belongs to P, b belongs to Q, so they denote the quarter for which to calculate the angle
dd dihedral_angle (plane P, plane Q, point a, point b)
{
    line k = P & Q;
    point p = k.a, v = k.dv;
    point va = v^P.n();
    if (Q.relation(p+va) != Q.relation(a)) va = va*(-1);
    point vb = v^Q.n();
    if (Q.relation(p+vb) != Q.relation(b)) vb = vb*(-1);
    
    dd cosa = va*vb / (va.len()*vb.len());
    if (cosa < -1) cosa = -1;
    if (cosa > 1) cosa = 1;
    
    return acos(cosa);
}

}
