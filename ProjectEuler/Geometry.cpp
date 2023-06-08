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

dd arc_angle (dd l, dd R) { return 2*asin(l/(2*R)); }
dd segment_square (dd a, dd R) { return R*R*(a-sin(a))/2; }

struct point
{
    dd x,y;
    
    point() { x = y = 0; }
    point(dd xx, dd yy) { x = xx; y = yy; }
    point(pdd p) { x = p.fs; y = p.sc; }
    
    void read() { cin >> x >> y; }
    void show() { cout << fixed << x << " " << y << endl; }
    
    dd len() { return sqrt(x*x + y*y); }
    dd len2() { return x*x + y*y; }
    point orthogonal() { return point(-y,x); }
    
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

bool polar_angle_sort (point a, point b) { return a.polar_angle() < b.polar_angle(); }

bool operator== (point a, point b) { return a.x == b.x && a.y == b.y; }
bool operator< (point a, point b) { return a.x < b.x || (a.x == b.x && a.y < b.y); }

point operator+ (point a, point b) { return point(a.x+b.x, a.y+b.y); }
point operator+ (point a, dd k) { return point(a.x+k, a.y+k); }
point operator- (point a, point b) { return point(a.x-b.x, a.y-b.y); }
point operator- (point a, dd k) { return point(a.x-k, a.y-k); }
point operator* (point a, dd k) { return point(a.x*k, a.y*k); }
point operator/ (point a, dd k) { return point(a.x/k, a.y/k); }
dd operator* (point a, point b) { return a.x*b.x + a.y*b.y; }

dd dist (point a, point b) { return (a-b).len(); }
dd dist2 (point a, point b) { return (a-b).len2(); }

bool is_collinear (vec a, vec b) { return a.x*b.y == a.y*b.x; }

point symmetric_point (point a, point b) { return b+b-a; }

dd signed_area (point a, point b, point c) { return 0.5*((a.x-c.x)*(b.y-c.y) - (a.y-c.y)*(b.x-c.x)); }
dd triangle_area (point a, point b, point c) { return fabs(signed_area(a,b,c)); }

bool triangle_contains_point (point a, point b, point c, point d) // abc contains d?
{
    return triangle_area(a,b,c) == triangle_area(a,b,d) + triangle_area(b,c,d) + triangle_area(a,c,d);
}

bool lies_on_segment (point a, seg s, bool should_check_collinearity = false)
{
    if (should_check_collinearity && signed_area(a,s.fs,s.sc) != 0) return false;
    return (s.fs-a)*(s.sc-a) <= 0;
}

point geom_inversion (point a, point c, dd R)
{
    vec v = a-c;
    dd L = v.len();
    v.set_length(R*R/L);
    return c+v;
}

bool cw (point a, point b, point c, bool q = false) // clockwise
{
    dd expr = a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y);
    if (q) return expr < 0;
    return expr <= 0;
}

bool ccw (point a, point b, point c, bool q = false) // counterclockwise
{
    return !cw(a,b,c,!q);
}

vector<point> convex_hull (vector<point> a, bool should_skip_border_points = true)
{
    if (a.size() <= 2) return a;
    
    sort(a.begin(),a.end());
    point p1 = a[0], p2 = a.back();
    vector<point> up, down;
    up.push_back(p1);
    down.push_back(p1);
    for (size_t i=1; i<a.size(); i++) {
        if (i == a.size()-1 || cw(p1, a[i], p2, should_skip_border_points)) {
            while (up.size() >= 2 && !cw(up[up.size()-2], up[up.size()-1], a[i], should_skip_border_points)) up.pop_back();
            up.push_back(a[i]);
        }
        if (i == a.size()-1 || ccw(p1, a[i], p2, !should_skip_border_points)) {
            while (down.size() >= 2 && !ccw(down[down.size()-2], down[down.size()-1], a[i], should_skip_border_points)) down.pop_back();
            down.push_back(a[i]);
        }
    }
    vector<point> b = up;
    for (size_t i=down.size()-2; i>0; i--) b.push_back(down[i]);
    return b;
}


struct line
{
    dd A,B,C;
    
    line() { A = 1; B = -1; C = 0; } // y = x
    line(dd a, dd b, dd c) { A = a; B = b; C = c; }
    line(point a, point b) { A = b.y - a.y; B = a.x - b.x; C = a.y*b.x - a.x*b.y; }
    line(seg s) { line(s.fs, s.sc); }
    
    void shift_by_vector (vec v) { C -= (A*v.x + B*v.y); }
    point any_point (dd y = 0) { return A == 0 ? point(0,-C/B) : point((C + B*y)/A, y); }
    point n() { return point(A,B); }
    dd at (point a) { return A*a.x + B*a.y + C; }
    
    int relation (point a) {
        dd d = at(a);
        if (d == 0) return 0; // optionally can be changed to EPS
        if (d < 0) return -1;
        return 1;
    }
    
    bool contains (point a) { return relation(a) == 0; }
    
    line orthogonal_line (point a) { return line(-B, A, B*a.x - A*a.y); }
};

bool is_parallel_lines (line p, line q) { return p.A*q.B == p.B*q.A; }

dd dist (point a, line p) { return fabs(p.at(a))/p.n().len(); }

point projection (point a, line p)
{
    return point(p.B*p.B*a.x - p.A*(p.C + p.B*a.y), p.A*p.A*a.y - p.B*(p.C + p.A*a.x)) / p.n().len2();
}

dd dist_to_segment (point a, seg s)
{
    line p(s);
    point b = projection(a,p);
    if (lies_on_segment(b,s)) return dist(a,b);
    return min(dist(a,s.fs), dist(a,s.sc));
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

point symmetric_point (point a, line p)
{
    point b = projection(a,p);
    return symmetric_point(a,b);
}

vec reflection_vector (point v, line p, point a = NOT_FOUND)
{
    if (p.n()*v == 0) return NOT_FOUND;
    
    if (a == NOT_FOUND) a = p.any_point();
    return symmetric_point(a+v, p) - a;
}

circ circumscribed_circle (point a, point b, point c)
{
    line p(a,b), q(a,c);
    p = p.orthogonal_line((a+b)/2);
    q = q.orthogonal_line((a+c)/2);
    
    point t = intersection_point(p,q);
    
    return make_pair(t,dist(a,t));
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

}
