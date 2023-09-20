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

u32string utf8ToUtf32 (const string &utf8) {
    
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cnv;
    u32string utf32 = cnv.from_bytes(utf8);
    if (cnv.converted() < utf8.size())
        throw std::runtime_error("utf8ToUtf32: Could not convert the following string: \"" + utf8 + "\"\n");
    return utf32;
}

string utf32ToUtf8 (const u32string &utf32) {
    
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cnv;
    string utf8 = cnv.to_bytes(utf32);
    if (cnv.converted() < utf32.size())
        throw std::runtime_error("utf32ToUtf8: could not convert string from utf32 to utf8\n");
    return utf8;
}

void print32 (const u32string &str) {
    
    wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
    cout << converter.to_bytes(str) << endl;
}

void add_escapings (string &S) {
    for (int i=(int)S.length()-1; i>=0; i--) {
        if (S[i] == '"') {
            S.insert(i,"\\");
        }
    }
}

int main() {
    clock_t Total_Time = clock();
    cout.precision(12);
    cout.setf(ios::fixed);
    ios_base::sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("input.txt","rt",stdin);
    freopen("output.txt","wt",stdout);
#endif
    
    //ifstream in("Hints.csv");
    vector<string> keys = { "cancel_continue_game", "common_error", "common_ok", "correct_answer", "dialog_menu", "dialog_numbers_game_unblocked_title", "dialog_restart", "finish_game_new_best_score_title", "game_over", "game_problem", "high_score", "instructions", "move_back", "onboarding_continue_button", "rating_in_top", "reset_dialog_message", "score", "settings", "titile_dialog_move_back", "video_continue_game", "video_watch_a_video", "wrong_answer", "you_win" };
    
    string S;
    cout << "//QB 2048 Strings\n";
    while (getline(cin,S)) {
        if (!StringUtils::hasPrefix(S, "\"")) continue;
        S.erase(0,1);
        S.pop_back();
        S.pop_back();
        for (int i=0; i<(int)keys.size(); i++) {
            if (!StringUtils::hasPrefix(S,keys[i]+'"')) continue;
            size_t c = S.find("\"");
            c = S.find("\"", c+1);
            string s1 = keys[i];
            string s2 = S.substr(c+1,S.length()-c-1);
            add_escapings(s2);
            cout << "\"" << keys[i] << "\" = \"" << s2 << "\";\n";
        }
    }
    
    //Total_Time = clock() - Total_Time;
    //cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
