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
    void show() { cout << fixed << x << " " << y << " " << z << endl; }
    
    dd len() { return sqrt(x*x + y*y + z*z); }
    gtype len2() { return x*x + y*y + z*z; }
    
    void set_length (dd L) {
        dd l = len();
        x *= (L/l);
        y *= (L/l);
        z *= (L/l);
    }
};

istream& operator>> (istream &is, point &a) { cin >> a.x >> a.y >> a.z; return is; }
ostream& operator<< (ostream &os, point &a) { cout << fixed << a.x << " " << a.y << " " << a.z; return os; }

bool operator== (point a, point b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
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

dd dist (point a, point b) { return (a-b).len(); }
gtype dist2 (point a, point b) { return (a-b).len2(); }

bool is_collinear (vec a, vec b) { return a.x*b.y == a.y*b.x && a.x*b.z == a.z*b.x && a.y*b.z == a.z*b.y; }

point symmetric_point (point a, point b) { return b+b-a; }


struct plane
{
    gtype A,B,C,D;
    
    plane() { A = 1; B = 1; C = 1; D = 0; } // x + y + z = 0;
    plane(dd a, dd b, dd c, dd d) { A = a; B = b; C = c; D = d; }
    plane(point a, point b, point c);
    
    point n() { return point(A,B,C); }
    gtype at (point a) { return A*a.x + B*a.y + C*a.z + D; }
    
    sint relation (point a) {
        gtype d = at(a);
        if (d == 0) return 0; // optionally can be changed to EPS
        if (d < 0) return -1;
        return 1;
    }
    
    bool contains (point a) { return relation(a) == 0; }
};

plane::plane (point a, point b, point c)
{
    A = a.y*b.z + a.z*c.y + b.y*c.z - a.z*b.y - b.z*c.y - a.y*c.z;
    B = a.z*b.x + b.z*c.x + a.x*c.z - b.x*c.z - a.x*b.z - a.z*c.x;
    C = a.x*b.y + a.y*c.x + b.x*c.y - b.y*c.x - a.x*c.y - a.y*b.x;
    D = -A*a.x - B*a.y - C*a.z;
}

dd dist (point a, plane p) { return fabs(p.at(a))/p.n().len(); }

pair<point,vec> intersection (plane p, plane q) // returns <point,vector> of intersection line of planes. Must not be parallel
{
    point a;
    
    dd prodAB = q.A*p.B - p.A*q.B;
    dd prodAC = q.A*p.C - p.A*q.C;
    dd prodBC = q.B*p.C - p.B*q.C;
    
    if (fabs(prodBC) > EPS) {
        a = point(0, q.C*p.D - p.C*q.D, p.B*q.D - q.B*p.D) / prodBC;
    }
    else if (fabs(prodAC) > EPS) {
        a = point(q.C*p.D - p.C*q.D, 0, p.A*q.D - q.A*p.D) / prodAC;
    }
    else {
        a = point(q.B*p.D - p.B*q.D, p.A*q.D - q.A*p.D, 0) / prodAB;
    }
    point v = p.n()^q.n();
    return mp(a,v);
}

// actually, there're two angles with the sum equals PI
// point a belongs to P, b belongs to Q, so they denote the quarter for which to calculate the angle
dd dihedral_angle (plane P, plane Q, point a, point b)
{
    pair<point,point> zzz = intersection(P,Q);
    point p = zzz.fs, v = zzz.sc;
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
