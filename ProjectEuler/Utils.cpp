//
//  Utils.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/3/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

// ********** OTHER USEFUL AND USELESS FUNCTIONS **********

namespace Utils
{

vull Lagged_Fibonacci_Generator (int n)
{
    vull s;
    
    for (ull i=1; i<=n; i++) {
        if (i <= 55) s.push_back((i*i*i*300007 - i*200003 + 100003) % 1000000);
        else s.push_back((s[i-56]+s[i-25]) % 1000000);
    }
    
    return s;
}

vull Blub_Blub_Shum_Generator (int n)
{
    vull v;
    ull a = 290797;
    
    for (int i=0; i<n; i++) {
        v.push_back(a);
        a = a*a % 50515093;
    }
    
    return v;
}

ll random_integer (ll from, ll to)
{
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<ll> uni(from,to); // guaranteed unbiased
    return uni(rng);
}

inline void try_decrease (int &x, const int y) { x > y ? x = y : 0; }

}
