//
//  Geomerty.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 7/23/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;
typedef unsigned int uint;

using namespace std;

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
    point(pii p) { x = p.fs; y = p.sc; }
    void read() { std::cin >> x >> y; }
    void show() { cout << fixed << x << " " << y << endl; }
    dd len() { return sqrt(x*x + y*y); }
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

bool operator< (point a, point b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
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

bool is_collinear (point a, point b)
{
    return a.x*b.y == a.y*b.x;
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

dd signed_area (point a, point b, point c)
{
    return 0.5*((a.x-c.x)*(b.y-c.y) - (a.y-c.y)*(b.x-c.x));
}

bool triangle_contains_point (point a, point b, point c, point d) // abc contains d?
{
    return triangle_area(a,b,c) == triangle_area(a,b,d) + triangle_area(b,c,d) + triangle_area(a,c,d);
}

bool geom_cw (point a, point b, point c, bool q = false) {
    if (q) return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) < 0;
    return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) <= 0;
}

bool geom_ccw (point a, point b, point c, bool q = false) {
    if (q) return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) > 0;
    return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) >= 0;
}

vector<point> convex_hull (vector<point> a, bool should_skip_border_points = true)
{
    if (a.size() <= 1) return vector<point>();
    
    sort (a.begin(), a.end());
    point p1 = a[0],  p2 = a.back();
    vector<point> up, down;
    up.push_back (p1);
    down.push_back (p1);
    for (size_t i=1; i<a.size(); ++i) {
        if (i==a.size()-1 || geom_cw(p1, a[i], p2, should_skip_border_points)) {
            while (up.size()>=2 && !geom_cw(up[up.size()-2], up[up.size()-1], a[i], should_skip_border_points))
                up.pop_back();
            up.push_back (a[i]);
        }
        if (i==a.size()-1 || geom_ccw(p1, a[i], p2, !should_skip_border_points)) {
            while (down.size()>=2 && !geom_ccw(down[down.size()-2], down[down.size()-1], a[i], should_skip_border_points))
                down.pop_back();
            down.push_back (a[i]);
        }
    }
    vector<point>b;
    for (size_t i=0; i<up.size(); ++i) b.push_back (up[i]);
    for (size_t i=down.size()-2; i>0; --i) b.push_back (down[i]);
    return b;
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
    C = a.y*b.x - a.x*b.y;
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

pair<point,dd> circumscribed_circle (point a, point b, point c)
{
    line p(a,b), q(a,c);
    p = orthogonal_line((a+b)/2, p);
    q = orthogonal_line((a+c)/2, q);
    
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
            P.push_back(make_point(cos((2*i+1)*PI/n),sin((2*i+1)*PI/n)));
        }
    }
    point operator[] (int k) { return P[k]; }
    bool empty() { return P.empty(); }
    int size() { return (int)P.size(); }
    void addPoint (point p) { P.push_back(p); }
    
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

