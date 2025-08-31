#include "Definitions.hpp"

#include "Algebra.hpp"
#include "Combinatorics.cpp"
#include "Complex.cpp"
// That's why we include .cpp here despite of having header. Ugly workaround
// https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
#include "Containers.cpp"
#include "Geometry.cpp"
#include "Geometry3D.cpp"
#include "FFT.cpp"
#include "Fractions.cpp"
#include "Lnum.hpp"
#include "Matrix.cpp"
#include "NumberUtils.hpp"
#include "Pell_Equation.cpp"
#include "StringUtils.cpp"
#include "Utils.cpp"

using namespace Algebra;
using namespace Containers;

vector<char> DS(100000);
pii A[100][200]; // A[i][j] means sequence a[0] = j, a[n] = a[n-1] + s(a[n-1]) + i. Pair is <number of elems, next element>

const ull TARGET = 1000000000000000;
const ull MID = 100000000;

void precalcs()
{
    for (int i=0; i<100; i++) for (int j=0; j<200; j++) A[i][j] = mp(0,0);

    for (int n=1; n<100000; n++) {
        DS[n] = total_vector_sum(NumberUtils::digits(n));
    }
}

sint digit_sum (ull n)
{
    return DS[n/100000] + DS[n%100000];
}

pii get_A (int s, int res, int id = -1) // -> <count, next_res> or exact elem if id == -1
{
    if (id == -1 && A[s][res].fs != 0) return A[s][res];
    
    int count = 0;
    int k = res;
    
    do {
        count++;
        if (count == id) return mp(k,0);
        k = k + digit_sum(k) + s;
    } while (k < MID);
    
    A[s][res] = mp(count, k % MID);
    return A[s][res];
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    //freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
    precalcs();
            
    ull index = 0;
    int pref = 0, res = 1;
    while (index < TARGET) {
        int s = digit_sum(pref);
        pii p = get_A(s,res);
        
        if (index + p.fs >= TARGET) {
            cout << pref << get_A(s,res,(int)(TARGET-index)).fs << endl; // this is the answer
            break;
        } else {
            index += p.fs;
            pref++;
            res = p.sc;
        }
        
        ans++;
        //cout << index << endl;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
