#include "Definitions.hpp"

#include "Algebra.hpp"
#include "Combinatorics.cpp"
#include "Complex.cpp"
// That's why we include .cpp here despite of having header. Ugly workaround
// https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
#include "Containers.cpp"
#include "Geometry.cpp"
#include "Geometry3D.cpp"
#include "Fractions.cpp"
#include "Lnum.hpp"
#include "Matrix.cpp"
#include "NumberUtils.hpp"
#include "Pell_Equation.cpp"
#include "StringUtils.cpp"
#include "Utils.cpp"

using namespace Algebra;
using namespace Containers;

struct Cell
{
    string hor; // horisontal sum
    string ver; // vertical sum
    string value; // empty if 'hor' or 'ver' is non-empty
    void read (const string &S)
    {
        hor = ver = value = "";
        if (S[0] != '(') value = S;
        else {
            size_t found = S.find("h");
            if (found != string::npos) {
                for (size_t i=found+1; S[i] != 'v' && S[i] != ')'; i++) {
                    hor += S[i];
                }
            }
            found = S.find("v");
            if (found != string::npos) {
                for (size_t i=found+1; S[i] != 'h' && S[i] != ')'; i++) {
                    ver += S[i];
                }
            }
        }
    }
}F[7][7];



int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","rt",stdin);
    //freopen("output.txt","wt",stdout);
#endif
    
    ull ans = 0;
    
//    vector<dd> sol;
//    ans = Gauss({{1,1,1,5},{1,-1,2,7}},sol);
//    show(sol);
    
    for (int T=0; T<200; T++) {
        
        int equations = 0, variables = 0;
        string Kakuro; cin >> Kakuro;
        vector<string> S = parse_by_symbol(Kakuro, ',');
        int N = atoi(S[0].c_str());
        int id = 1;
        for (int i=0; i<N; i++) for (int j=0; j<N; j++) {
            if (S[id][0] == '(' && S[id].back() != ')') {
                id++;
                S[id] = S[id-1] + S[id];
            }
            F[i][j].read(S[id++]);
            //cout << i << " " << j << endl << F[i][j].hor << " " << F[i][j].ver << " " << F[i][j].value << endl;
            if (F[i][j].value == "O") variables++;
            if (!F[i][j].hor.empty()) equations++;
            if (!F[i][j].ver.empty()) equations++;
        }
        cout << variables-equations << endl;
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
