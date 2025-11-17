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

const string RANKS = "23456789TJQKA";
const string SUITS = "CHSD";

struct card {
    sint rank;
    sint suit;
    
    card() { rank = suit = 0; }
    card(sint r, sint s) { rank = r; suit = s; }
};

istream& operator>> (istream &is, card &a) {
    char r,s;
    is >> r >> s;
    a.rank = RANKS.find(r);
    a.suit = SUITS.find(s);
    return is;
}

ostream& operator<< (ostream &os, card &a) { os << RANKS[a.rank] << SUITS[a.suit]; return os; }

bool operator< (const card &a, const card &b) { return a.rank < b.rank; }
bool operator== (const card &a, const card &b) { return a.rank == b.rank; }

struct hand {
    vector<card> C; // size = 5
    vsint R; // initial ranks
    vsint S; // actual ranks
    int type; // combination type
    
    hand(const vector<card> &A) {
        C = A;
        sort(C.begin(), C.end());
        reverse(C.begin(), C.end());
        R = vector<sint>(5);
        for (int i=0; i<5; i++) R[i] = C[i].rank;
    }
    
    vsint detect_flush() {
        for (int i=0; i<4; i++) {
            if (C[i].suit != C[i+1].suit) return {};
        }
        return R;
    }
    
    vsint detect_straight() {
        for (int i=1; i<4; i++) {
            if (C[i].rank != C[i+1].rank + 1) return {};
        }
        // A2345 case
        if (C[0].rank == 12 && C[1].rank == 3) return { 3 };
        if (C[0].rank == C[1].rank + 1) return { C[0].rank };
        return {};
    }
    
    vsint detect_straight_flush() {
        if (detect_flush().empty()) return {};
        vsint S = detect_straight();
        if (!S.empty()) return S;
        return {};
    }
    
    vsint detect_quad() {
        if (C[0].rank == C[3].rank) return { C[0].rank, C[4].rank };
        if (C[1].rank == C[4].rank) return { C[4].rank, C[0].rank };
        return {};
    }
    
    vsint detect_full_house() {
        if (C[0].rank == C[2].rank && C[3].rank == C[4].rank) return { C[0].rank, C[4].rank };
        if (C[0].rank == C[1].rank && C[2].rank == C[4].rank) return { C[4].rank, C[0].rank };
        return {};
    }
    
    vsint detect_triple() {
        if (C[0].rank == C[2].rank) return { C[0].rank, C[3].rank, C[4].rank };
        if (C[1].rank == C[3].rank) return { C[1].rank, C[0].rank, C[4].rank };
        if (C[2].rank == C[4].rank) return { C[2].rank, C[0].rank, C[1].rank };
        return {};
    }
    
    vsint detect_double() {
        if (C[0].rank == C[1].rank && C[2].rank == C[3].rank) return { C[0].rank, C[2].rank, C[4].rank };
        if (C[0].rank == C[1].rank && C[3].rank == C[4].rank) return { C[0].rank, C[3].rank, C[2].rank };
        if (C[1].rank == C[2].rank && C[3].rank == C[4].rank) return { C[1].rank, C[3].rank, C[0].rank };
        return {};
    }
    
    vsint detect_pair() {
        if (C[0].rank == C[1].rank) return { C[0].rank, C[2].rank, C[3].rank, C[4].rank };
        if (C[1].rank == C[2].rank) return { C[1].rank, C[0].rank, C[3].rank, C[4].rank };
        if (C[2].rank == C[3].rank) return { C[2].rank, C[0].rank, C[1].rank, C[4].rank };
        if (C[3].rank == C[4].rank) return { C[3].rank, C[0].rank, C[1].rank, C[2].rank };
        return {};
    }
    
    void set_type() {
        S = detect_straight_flush();
        if (!S.empty()) { type = 8; return; }
        S = detect_quad();
        if (!S.empty()) { type = 7; return; }
        S = detect_full_house();
        if (!S.empty()) { type = 6; return; }
        S = detect_flush();
        if (!S.empty()) { type = 5; return; }
        S = detect_straight();
        if (!S.empty()) { type = 4; return; }
        S = detect_triple();
        if (!S.empty()) { type = 3; return; }
        S = detect_double();
        if (!S.empty()) { type = 2; return; }
        S = detect_pair();
        if (!S.empty()) { type = 1; return; }
        
        type = 0;
        S = R;
    }
};

bool operator< (const hand &a, const hand &b)
{
    if (a.type < b.type) return true;
    if (a.type > b.type) return false;
    for (int i=0; i<(int)a.S.size(); i++) {
        if (a.S[i] < b.S[i]) return true;
        if (a.S[i] > b.S[i]) return false;
    } return false;
}

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
    
    vector<hand> H;
    
    for (int i=0; i<13; i++)
    for (int j=0; j<=i; j++)
    for (int k=0; k<=j; k++)
    for (int l=0; l<=k; l++)
    for (int m=0; m<=l; m++) {
        if (i == m) continue; // 5 same cards
        
        vector<card> A = { card(i,0),card(j,1),card(k,2),card(l,3),card(m,0) };
        hand h(A);
        h.set_type();
        H.push_back(h);
        
        if (i > j && j > k && k > l && l > m) { // same suit possible
            vector<card> B = { card(i,0),card(j,0),card(k,0),card(l,0),card(m,0) };
            hand g(B);
            g.set_type();
            H.push_back(g);
        }
    }
    
    sort(H.begin(),H.end());
    //for (int i=0; i<5; i++) cout << H[0].C[i] << " "; cout << endl;
    //for (int i=0; i<5; i++) cout << H[7461].C[i] << " "; cout << endl;
    
    vector<card> C(5);
    for (int i=0; i<5; i++) cin >> C[i];
    hand h(C);
    h.set_type();
    
    int lb = 0, ub = 7461;
    while (true) {
        int M = (lb + ub) / 2;
        if (H[M] < h) lb = M+1;
        else if (h < H[M]) ub = M-1;
        else { ans = M+1; break; }
    }
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
