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

vector<string> transform_question (const vector<string> &s)
{
    vector<string> A;
    
    // compose two first strings separately
    size_t two_dots = s[0].find(":");
    string first_string = s[0].substr(11,two_dots-11) + " = Question("; // var MPTwo01 = Question(
    string second_string = spaces(12) + s[1].substr(17,s[1].length()-17); // ID: 2
    A.push_back(first_string);
    A.push_back(second_string);
    
    string code;
    //A.push_back(s[0].substr(11,s[0].length()-11)); // static var MPTwo01: Question = {
    for (int i=2; i<(int)s.size()-1; i++) {
        if (s[i].find("imageQuestion") != string::npos) continue;
        size_t found = s[i].find("imageAnswer");
        if (found != string::npos) {
            code = extract_code(s[i].substr(found+11,s[i].length()-found-11));
        }
        else {
            A.push_back(s[i].substr(5,s[i].length()-5));
        }
    }
    
    A.back().pop_back(); // remove ')'
    A.push_back(spaces(4) + ")");
    //A.back() += ')';
    //A.push_back(s.back()); // }()
    
    A.insert(A.begin()+2,spaces(12) + "type: " + type(code) + ",");
    A.insert(A.begin()+3,spaces(12) + "code: \"" + code + "\",");
    
    return A;
}

string state_code (const string &s)
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

string state_round_name (const string &s)
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

bool state_is_with_image_in_question (const string &s)
{
    return s.find(".textWithImage") != string::npos;
}

bool state_is_custom_music (const string &s)
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

bool state_has_video (const string &s)
{
    return s.find("V:") != string::npos;
}

string current_state_type = "Q"; // Q - Question, R - Repeat, A - Answer

void process_state (const string &s)
{
    string code = state_code(s);
    bool withImageQ = state_is_with_image_in_question(s);
    bool is_music = state_is_custom_music(s);
    bool is_video = state_has_video(s);
    
    M[code + current_state_type] = {withImageQ, is_music, is_video};
}

struct StatesResult {
    int index; // fileprivate var States3 -> index == 3
    int round; // T: .justRound(2, -> round == 2, basically should be the same as index
    string name;
    vector<string> codes;
    bool is_blitz;
};

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
    
    bool was_first_question = false;
    bool was_first_state = false;
    
    vector<StatesResult> R;
    
    vector<string> current_states_codes;
    string current_states_name;
    int current_states_index = 0; // 0...7 basically
    int current_round_index = -1;
    int current_states_count = 0;
    
    // we must process states first, that's why we save strings for now
    vector<string> question_strings;
    string S;
    int si = 0;
    while (getline(cin,S)) {
        si++;
        // check for question or state
        if (was_first_question == false) {
            if (S.find(": Question") == string::npos) {
                if (S.find("extension") == string::npos && si != 10) cout << S << endl;
                continue;
            }
            else was_first_question = true;
        }
        if (was_first_state == false) {
            if (S.find("fileprivate") != string::npos) was_first_state = true;
        }
        
        if (was_first_state) { // process state
            if (S.find("fileprivate") != string::npos) { // here we can obtain states array index
                size_t dot = S.find(":");
                current_states_index = (int)(S[dot-1] - '0');
                continue; // we can do it, because this is definitely not state
            }
            if (S.find("justRound") != string::npos && current_round_index == -1) { // here we can obtain round index
                size_t found = S.find("justRound");
                current_round_index = (int)(S[found+10] - '0');
            }
            
            string name = state_round_name(S);
            if (name == "not_state") continue;
            
            current_states_count++;
            
            if (StringUtils::hasPrefix(name, "Повтор") || StringUtils::hasPrefix(name, "Repetitions")) current_state_type = "R";
            else if (StringUtils::hasPrefix(name, "Ответ") || StringUtils::hasPrefix(name, "Answers")) current_state_type = "A";
            else if (!name.empty() && !(StringUtils::hasPrefix(name, "Окончен") || StringUtils::hasPrefix(name, "Finished"))) {
                current_state_type = "Q";
                current_states_name = name;
            }
            else {
                string code = state_code(S);
                if (code != "empty" && !Containers::contains(current_states_codes, code)) { current_states_codes.push_back(code);
                }
                
                process_state(S); // this call is to determine corresponding question type
            }
            
            if (StringUtils::hasPrefix(name, "Окончен") || StringUtils::hasPrefix(name, "Finished")) { // means this is the last state
                StatesResult r;
                r.index = current_states_index;
                r.round = current_round_index;
                r.name = current_states_name;
                r.codes = current_states_codes;
                r.is_blitz = current_states_count < 30;
                
                R.push_back(r);
                
                current_states_codes.clear();
                current_states_name = "";
                current_states_index = 0;
                current_round_index = -1;
                current_states_count = 0;
            }
        }
        else { // add question string
            question_strings.push_back(S);
        }
    }
    
    // now process all question_strings
    vector<string> cur;
    for (int q=0; q<(int)question_strings.size(); q++) {
        string U = question_strings[q];
        vector<string> v = StringUtils::parse_by_symbol(U, ' ');
        if (v.size() == 0) continue;
        cur.push_back(U);
        if (v[0] == "}()") {
            vector<string> t = transform_question(cur);
            cur.clear();
            for (int i=0; i<(int)t.size(); i++) cout << t[i] << endl;
        }
    }
    
    // output states
    for (int r=0; r<(int)R.size(); r++) {
        vector<string> A;
        A.push_back("fileprivate var States" + to_string(R[r].index) + " = State.allStates(round: " + to_string(R[r].round) + ",");
        string codes_string = "[";
        for (int i=0; i<(int)R[r].codes.size(); i++) {
            codes_string += R[r].codes[i];
            if (i+1 != (int)R[r].codes.size()) codes_string += ", ";
            else codes_string += "]";
        }
        A.push_back(spaces(42) + "questions: " + codes_string);
        A.push_back(spaces(42) + "name: \"" + R[r].name + "\",");
        string code = R[r].codes.back();
        while (is_digit(code.back())) code.pop_back(); // AW12 -> AW
        A.push_back(spaces(42) + "time: " + code + "times[" + to_string(R[r].round) + "]");
        if (R[r].is_blitz) {
            A.back().push_back(',');
            A.push_back(spaces(42) + "isBlitz: true)");
        }
        else {
            A.back().push_back(')');
        }
        
        cout << endl;
        for (int i=0; i<(int)A.size(); i++) cout << A[i] << endl;
    }
    
//    cout << endl << ans << endl;
//    Total_Time = clock() - Total_Time;
//    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
