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

bool is_digit (char c)
{
    return c >= '0' && c <= '9';
}

string spaces (int k)
{
    string s;
    for (int i=0; i<k; i++) s += ' ';
    return s;
}

string extract_code (string s)
{
    string code;
    bool was_brackets = false;
    bool was_digits = false;
    for (int i=0; i<(int)s.length(); i++) {
        if (is_digit(s[i])) was_digits = true;
        else if (was_digits) break;
        
        if (s[i] == '"') was_brackets = true;
        else if (was_brackets) {
            code += s[i];
        }
    }
    
    return code;
}

map<string, tuple<bool, bool, bool>> M; // <code, <is_with_image, is_custom_music, is_custom_video> >

string type (string code)
{
    tuple<bool, bool, bool> Q = M[code+"Q"];
    tuple<bool, bool, bool> A = M[code+"A"];
    
    bool q_with_image = get<0>(Q);
    bool q_with_custom_music = get<1>(Q);
    bool q_with_video = get<2>(Q);
    bool a_with_image = get<0>(A);
    bool a_with_custom_music = get<1>(A);
    bool a_with_video = get<2>(A);
    
    if (q_with_video && a_with_video) return ".video";
    if (a_with_video) {
        if (q_with_image) return ".imageAnsVideo";
        else return ".textAnsVideo";
    }
    if (q_with_custom_music && a_with_custom_music) return ".music";
    if (a_with_custom_music) {
        if (q_with_image) return ".imageAnsMusic";
        else return ".textAnsMusic";
    }
    
    return q_with_image ? ".imageAnsImage" : ".textAnsImage";
//    int first_digit = -1;
//    for (int i=0; i<(int)code.length(); i++) {
//        if (is_digit(code[i])) {
//            first_digit = (int)(code[i] - '0');
//            break;
//        }
//    }
    
    // 7 tours
//    if (first_digit == 0) return ".textAnsImage";
//    if (first_digit == 1) return ".textAnsImage";
//    if (first_digit == 2) return ".textAnsImage";
//    if (first_digit == 3) return ".music";
//    if (first_digit == 4) return ".textAnsImage";
//    if (first_digit == 5) return ".imageAnsImage";
//    if (first_digit == 6) return ".textAnsImage";
//    if (first_digit == 7) return ".textAnsImage";
    
    // 8 tours
//    if (first_digit == 0) return ".textAnsImage";
//    if (first_digit == 1) return ".textAnsImage";
//    if (first_digit == 2) return ".music";
//    if (first_digit == 3) return ".imageAnsImage";
//    if (first_digit == 4) return ".textAnsImage";
//    if (first_digit == 5) return ".music";
//    if (first_digit == 6) return ".imageAnsImage";
//    if (first_digit == 7) return ".textAnsImage";
//    return ".textAnsImage";
}

vector<string> transform_question (vector<string> s)
{
    vector<string> A;
    
    string code;
    A.push_back(s[0].substr(11,s[0].length()-11)); // static var MPTwo01: Question = {
    for (int i=1; i<(int)s.size()-1; i++) {
        if (s[i].find("imageQuestion") != string::npos) continue;
        size_t found = s[i].find("imageAnswer");
        if (found != string::npos) {
            code = extract_code(s[i].substr(found+11,s[i].length()-found-11));
        }
        else {
            A.push_back(s[i]);
        }
    }
    
    A.back().pop_back();
    A.back() += ')';
    A.push_back(s.back()); // }()
    
    A.insert(A.begin()+2,spaces(17) + "type: " + type(code) + ",");
    A.insert(A.begin()+3,spaces(17) + "code: \"" + code + "\",");
    
    return A;
}

string state_code (string &s)
{
    string code;
    size_t found = s.find("Q:");
    if (found == string::npos) return code;
    found += 2;
    while (s[found] != '.') found++;
    while (s[found+1] != ',') {
        found++;
        code += s[found];
    }
    
    return code;
}

string state_round_name (string &s)
{
    if (s.find("T:") == string::npos) return "not_state";
    
    string name;
    size_t found = s.find(".justRound");
    if (found == string::npos) return name;
    found += 9;
    while (s[found] != '"') found++;
    while (s[found+1] != '"') {
        found++;
        name += s[found];
    }
    
    return name;
}

bool state_is_with_image_in_question (string &s)
{
    return s.find(".textWithImage") != string::npos;
}

bool state_is_custom_music (string &s)
{
    string code = state_code(s);
    string file;
    
    size_t found = s.find("M:");
    found += 2;
    while (s[found] != '"') found++;
    while (s[found+1] != '"') {
        found++;
        file += s[found];
    }
    
    return StringUtils::hasPrefix(file, code);
}

bool state_has_video (string &s)
{
    return s.find("V:") != string::npos;
}

string current_state_type = "Q"; // Q - Question, R - Repeat, A - Answer

void process_state (string &s)
{
    string code = state_code(s);
    bool withImageQ = state_is_with_image_in_question(s);
    bool is_music = state_is_custom_music(s);
    bool is_video = state_has_video(s);
    
    M[code + current_state_type] = {withImageQ, is_music, is_video};
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
    
    ull ans = 0;
    
    vector<string> round_names;
    
    ifstream in ("states.txt");
    string U;
    while (getline(in,U)) {
        string name = state_round_name(U);
        if (name == "not_state") continue;
        if (StringUtils::hasPrefix(name, "Повтор") || StringUtils::hasPrefix(name, "Repetitions")) current_state_type = "R";
        else if (StringUtils::hasPrefix(name, "Ответ") || StringUtils::hasPrefix(name, "Answers")) current_state_type = "A";
        else if (!name.empty() && !(StringUtils::hasPrefix(name, "Окончен") || StringUtils::hasPrefix(name, "Finished"))) {
            current_state_type = "Q";
            round_names.push_back(name);
        }
        else {
            process_state(U);
        }
    }
    
    vector<string> cur;
    string S;
    while (getline(cin,S)) {
        //cout << S.length() << " " << S << endl;
        vector<string> v = StringUtils::parse_by_symbol(S, ' ');
        if (v.size() == 0) continue;
        cur.push_back(S);
        if (v[0] == "}()") {
            vector<string> U = transform_question(cur);
            cur.clear();
            for (int i=0; i<(int)U.size(); i++) cout << U[i] << endl;
        }
    }
    
    for (int i=0; i<(int)round_names.size(); i++) cout << round_names[i] << endl;
    
//    cout << endl << ans << endl;
//    Total_Time = clock() - Total_Time;
//    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
