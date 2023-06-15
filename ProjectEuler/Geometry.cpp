//
//  Geomerty.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 7/23/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

// TODO: create side extension with integer geometry (or inline with predefined numbers type)

namespace Geometry
{

typedef dd gtype;

struct point
{
    gtype x,y;
    
    point() { x = y = 0; }
    point(gtype xx, gtype yy) { x = xx; y = yy; }
    point(pair<gtype,gtype> p) { x = p.fs; y = p.sc; }
    
    void read() { cin >> x >> y; }
    void show() { cout << fixed << x << " " << y << endl; }
    
    dd len() { return sqrt(x*x + y*y); }
    gtype len2() { return x*x + y*y; }
    vec orthogonal() { return vec(-y,x); }
    
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

istream& operator>> (istream &is, point &a) { cin >> a.x >> a.y; return is; }
ostream& operator<< (ostream &os, point &a) { cout << fixed << a.x << " " << a.y; return os; }

bool operator== (point a, point b) { return a.x == b.x && a.y == b.y; }
bool operator!= (point a, point b) { return a.x != b.x || a.y != b.y; }
bool operator< (point a, point b) { return a.x < b.x || (a.x == b.x && a.y < b.y); }

point operator+ (point a, point b) { return point(a.x+b.x, a.y+b.y); }
point operator+ (point a, gtype k) { return point(a.x+k, a.y+k); }
point operator- (point a) { return point(-a.x, -a.y); }
point operator- (point a, point b) { return point(a.x-b.x, a.y-b.y); }
point operator- (point a, gtype k) { return point(a.x-k, a.y-k); }
point operator* (point a, gtype k) { return point(a.x*k, a.y*k); }
point operator/ (point a, gtype k) { return point(a.x/k, a.y/k); }
gtype operator* (point a, point b) { return a.x*b.x + a.y*b.y; }
gtype operator^ (point a, point b) { return a.x*b.y - a.y*b.x; }
bool operator| (point a, point b) { return (a^b) == 0; }

dd dist (point a, point b) { return (a-b).len(); }
gtype dist2 (point a, point b) { return (a-b).len2(); }

point symmetric_point (point a, point b) { return b+b-a; }

gtype skew (point a, point b, point c) { return (a.x-c.x)*(b.y-c.y) - (a.y-c.y)*(b.x-c.x); }
dd signed_area (point a, point b, point c) { return 0.5*skew(a,b,c); }
dd triangle_area (point a, point b, point c) { return fabs(0.5*skew(a,b,c)); }

bool triangle_contains_point (point a, point b, point c, point d) // abc contains d?
{
    return triangle_area(a,b,c) == triangle_area(a,b,d) + triangle_area(b,c,d) + triangle_area(a,c,d);
}

bool lies_on_segment (point a, seg s, bool should_check_collinearity = false)
{
    if (should_check_collinearity && skew(a,s.fs,s.sc) != 0) return false;
    return (s.fs-a)*(s.sc-a) <= 0;
}

bool on_same_line (const vector<point> &a)
{
    if (a.size() < 3) return true;
    for (int i=2; i<(int)a.size(); i++) {
        if (skew(a[0],a[1],a[i]) != 0) return false;
    } return true;
}

point geom_inversion (point a, circ c)
{
    vec v = a-c.fs;
    dd L = v.len();
    v.set_length(c.sc*c.sc/L);
    return c.fs+v;
}
// clockwise, counterclockwise
bool cw (point a, point b, point c, bool q = false) { return q ? skew(a,b,c) < 0 : skew(a,b,c) <= 0; }
bool ccw (point a, point b, point c, bool q = false) { return !cw(a,b,c,!q); }

vector<point> convex_hull (vector<point> a, bool should_skip_border_points = true)
{
    if (a.size() <= 2) return a;
    
    sort(a.begin(),a.end());
    point p1 = a[0], p2 = a.back();
    vector<point> up(1,p1), down(1,p1);
    
    for (size_t i=1; i<a.size(); i++) {
        if (i == a.size()-1 || cw(p1, a[i], p2, should_skip_border_points)) {
            while (up.size() >= 2 && !cw(up[up.size()-2], up.back(), a[i], should_skip_border_points)) up.pop_back();
            up.push_back(a[i]);
        }
        if (i == a.size()-1 || ccw(p1, a[i], p2, !should_skip_border_points)) {
            while (down.size() >= 2 && !ccw(down[down.size()-2], down.back(), a[i], should_skip_border_points)) down.pop_back();
            down.push_back(a[i]);
        }
    }
    
    for (size_t i=down.size()-2; i>0; i--) up.push_back(down[i]);
    return up;
}


struct line
{
    gtype A,B,C;
    
    line() { A = 1; B = -1; C = 0; } // y = x
    line(gtype a, gtype b, gtype c) { A = a; B = b; C = c; }
    line(point a, point b) { A = b.y - a.y; B = a.x - b.x; C = (b^a); }
    line(seg s) : line(s.fs, s.sc) {}
    
    point n() { return point(A,B); }
    point dv() { return point(-B,A); } // direction vector
    gtype at (point a) { return A*a.x + B*a.y + C; }
    line orthogonal_line (point a) { return line(-B, A, a^n()); }
    
    void shift_by_vector (vec v) { C -= (A*v.x + B*v.y); }
    point any_point (dd y = 0) { return A == 0 ? point(0,-C/B) : point(-(C + B*y)/A, y); }
    
    sint relation (point a) {
        gtype d = at(a);
        if (d == 0) return 0; // optionally can be changed to EPS
        if (d < 0) return -1;
        return 1;
    }
    
    bool contains (point a) { return relation(a) == 0; }
}NO_LINE(-1000000000,-1000000000,-1000000000);

bool operator| (line p, line q) { return p.n()|q.n(); }
bool is_orthogonal_lines (line p, line q) { return p.n()*q.n() == 0; }

dd dist (point a, line p) { return fabs(p.at(a))/p.n().len(); }

point projection (point a, line p) { return point(p.B*(a^p.n()) - p.A*p.C, p.A*(p.n()^a) - p.B*p.C) / p.n().len2(); }
point symmetric_point (point a, line p) { point b = projection(a,p); return symmetric_point(a,b); }

dd dist (point a, seg s)
{
    line p(s);
    point b = projection(a,p);
    if (lies_on_segment(b,s)) return dist(a,b);
    return min(dist(a,s.fs), dist(a,s.sc));
}

line symmetric_line (line p, line q) // returns p' which is symmetric to p with respect to q
{
    point c = p.any_point();
    return line(symmetric_point(c,q), symmetric_point(c+p.dv(),q));
}

vec reflection_vector (point v, line p, point a = NOT_FOUND)
{
    if (p.n()*v == 0) return NOT_FOUND;
    
    if (a == NOT_FOUND) a = p.any_point();
    return symmetric_point(a+v, p) - a;
}

// *** INTERSECTIONS (& operators) ***

point operator& (line p, line q)
{
    if (p|q) { return p.A*q.C == p.C*q.A ? INFINITELY_MANY : NOT_FOUND; }
    return point(p.B*q.C - q.B*p.C, q.A*p.C - p.A*q.C) / (p.n()^q.n());
}

point operator& (seg a, line p)
{
    if (a.fs == a.sc) return p.at(a.fs) == 0 ? a.fs : NOT_FOUND;
    line q(a);
    if (p|q) return NOT_FOUND; // TODO: INFINITELY_MANY case
    point c = p & q;
    return lies_on_segment(c,a) ? c : NOT_FOUND;
}

point operator& (seg a, seg b)
{
    line p(a), q(b);
    if (p|q) return NOT_FOUND; // TODO: INFINITELY_MANY case
    point c = p & q;
    return lies_on_segment(c,a) && lies_on_segment(c,b) ? c : NOT_FOUND;
}

pair<point,point> operator& (circ c, line p)
{
    point c0 = c.fs; dd R = c.sc;
    if (dist(c0,p) > R) return mp(NOT_FOUND,NOT_FOUND);
    
    p.shift_by_vector(-c0); // move circle into zero point
    dd D = p.n().len2();
    dd mult = sqrt((R*R - (p.C*p.C) / D) / D);
    point p0 = -p.n()*p.C / D;
    point m = p.n().orthogonal()*mult;
    return mp(p0+c0-m, p0+c0+m);
}

circ circumscribed_circle (point a, point b, point c)
{
    line p(a,b), q(a,c);
    p = p.orthogonal_line((a+b)/2);
    q = q.orthogonal_line((a+c)/2);
    
    point t = p & q;
    return mp(t,dist(a,t));
}

struct Polygon {
    
    vector<point> P; // should be sorted in angle order!
    dd area;
    dd perimeter;
    
    Polygon() {}
    Polygon(int n) {
        for (int i=0; i<n; i++) {
            P.push_back(point(cos((2*i+1)*PI/n),sin((2*i+1)*PI/n)));
        }
    }
    point operator[] (int k) { return P[k]; }
    bool empty() { return P.empty(); }
    int size() { return (int)P.size(); }
    void add_point (point p) { P.push_back(p); }
    
    void show() {
        for (int i=0; i<(int)P.size(); i++) {
            cout << fixed << P[i].x << " " << P[i].y << endl;
        }
    }
    
    void calculate_area() { // works for convex polygons only
        area = 0;
        for (int i=2; i<(int)P.size(); i++) area += triangle_area(P[0],P[i-1],P[i]);
    }
    
    void calculate_perimeter() {
        perimeter = 0;
        for (int i=0; i<(int)P.size(); i++) {
            int n = i+1; if (n == (int)P.size()) n = 0;
            perimeter += dist(P[i], P[n]);
        }
    }
    // area must be calculated before
    bool contains_point (point p, bool strictly_inside = false) {
        
        dd a = 0;
        for (int i=1; i<(int)P.size(); i++) {
            dd sq = triangle_area(p,P[i-1],P[i]);
            if (sq == 0) return !strictly_inside;
            a += sq;
        }
        dd sq = triangle_area(p,P.back(),P[0]);
        if (sq == 0) return !strictly_inside;
        a += sq;
        
        return a == area;
    }
};

// *** CIRCLES ***

dd arc_angle (dd L, dd R) { return 2*asin(L/(2*R)); }
dd chord_length (dd a, dd R) { return 2*R*sin(a/2); }
dd circle_segment_area (dd a, dd R) { return R*R*(a-sin(a))/2; }
dd circle_sector_area (dd L, dd R) { return R*R*arc_angle(L,R)/2; }

static line aux_tangent_calcs (point c, dd r1, dd r2) // guaranteed to be exist
{
    dd r = r2-r1;
    dd l = c.len2();
    dd d = l - r*r;
    if (d < EPS) d = 0;
    d = sqrt(d);
    
    return line((c.x*r + c.y*d)/l, (c.y*r - c.x*d)/l, r1);
}

pair<line,line> inner_tangents (circ a, circ b)
{
    if (dist(a.fs,b.fs) < a.sc+b.sc - EPS) return mp(NO_LINE,NO_LINE);
    line p = aux_tangent_calcs(b.fs-a.fs, a.sc, -b.sc);
    line q = aux_tangent_calcs(b.fs-a.fs, -a.sc, b.sc);
    p.shift_by_vector(a.fs); q.shift_by_vector(a.fs);
    return mp(p,q);
}

pair<line,line> outer_tangents (circ a, circ b)
{
    if (dist(a.fs,b.fs) < fabs(a.sc-b.sc) - EPS) return mp(NO_LINE,NO_LINE);
    line p = aux_tangent_calcs(b.fs-a.fs, a.sc, b.sc);
    line q = aux_tangent_calcs(b.fs-a.fs, -a.sc, -b.sc);
    p.shift_by_vector(a.fs); q.shift_by_vector(a.fs);
    return mp(p,q);
}

// D is the distance between centers of two circles with radii r1 and r2
dd circles_intersection_area (dd r1, dd r2, dd D)
{
    if (r1+r2 <= D) return 0;
    if (D+r1 <= r2) return PI*r1*r1;
    if (D+r2 <= r1) return PI*r2*r2;
    
    dd k = (r1*r1 - r2*r2) / D;
    dd x = (D+k)/2;
    dd L = sqrt(2*(r1*r1 + r2*r2) - D*D - k*k);
    
    dd s1 = circle_segment_area(arc_angle(L,r1),r1); if (x < 0) s1 = PI*r1*r1 - s1;
    dd s2 = circle_segment_area(arc_angle(L,r2),r2); if (x > D) s2 = PI*r2*r2 - s2;
    
    return s1+s2;
}

}
