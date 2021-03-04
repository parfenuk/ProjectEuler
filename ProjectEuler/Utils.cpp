//
//  Utils.cpp
//  ProjectEuler
//
//  Created by Miraslau Parafeniuk on 3/3/21.
//  Copyright © 2021 Miraslau Parafeniuk. All rights reserved.
//

// ********** OTHER USEFUL AND USELESS FUNCTIONS **********

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

namespace Utils
{

vector<ull> Lagged_Fibonacci_Generator (int n)
{
    vector<ull> s;
    
    for (ull i=1; i<=n; i++) {
        
        if (i <= 55) s.push_back((i*i*i*300007 - i*200003 + 100003) % 1000000);
        else s.push_back((s[i-56]+s[i-25]) % 1000000);
    }
    
    return s;
}

vector<ull> Blub_Blub_Shum_Generator (int n)
{
    vector<ull> v;
    ull a = 290797;
    
    for (int i=0; i<n; i++) {
        v.push_back(a);
        a = a*a % 50515093;
    }
    
    return v;
}

// TODO: use templates
int random_integer (int from, int to)
{
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(from,to); // guaranteed unbiased
    return uni(rng);
}

}
