//
//  Complex.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 8/26/17.
//  Copyright © 2017 Miraslau Parafeniuk. All rights reserved.
//

typedef long long ll;
typedef unsigned long long ull;
typedef long double dd;
typedef short int sint;
typedef unsigned int uint;
typedef int ltype;

using namespace std;

struct complex {
    
    ll x, y;
    ll norm() { return x*x + y*y; }
    complex() { x = y = 0; }
    complex(ull _x) { x = _x; y = 0; }
    complex conjugate() { complex z; z.x = x; z.y = -y; return z; }
    
    void show() {
        
        if (y == 0) cout << x;
        else if (x == 0) cout << y << "i";
        else {
            cout << x;
            if (y > 0) cout << "+";
            cout << y << "i";
        }
        cout << endl;
    }
}Oz;
