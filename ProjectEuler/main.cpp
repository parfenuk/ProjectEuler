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
using namespace StringUtils;

const vector<string> TYPE_NAMES = {"",
    "Текст + картинка необязательно",         // 1
    "Текст + ответ с картинкой",
    "Текст + ответ с музыкой",
    "Текст + ответ с видео",
    "Вопрос с картинкой + ответ с картинкой", // 5
    "Вопрос с аудио + ответ с аудио",
    "Вопрос с видео + ответ с видео",
    "Музыкальный (3 файла + картинка)",
    "Вопрос с картинкой + ответ с видео",
    "Вопрос с аудио + ответ с видео",         // 10
    "3 факта о",
    "Блиц с 4 вариантами",
    "Видеотур",
    "Текст + ответ с картинкой и музыкой",
    "Вопрос с картинкой + ответ с картинкой и музыкой",
    "Музыкальный без повторов (2 файла + картинка)",
    "4 варианта и музыка + ответ с картинкой и музыкой",
    "3 строчки"
};

const string PATH = "/Users/Miroslav/Documents/MZGB Diary/Mzgb Hackers/";

string path_filter (string filter, int id) {
    return PATH + "Фильтры/" + filter + "/" + to_string(id) + ".txt";
}
string path_question_base (string version, int id) { // v2, v3a, v3m
    return PATH + "База вопросов " + version + "/" + to_string(id) + ".txt";
}

vector<int> Qtype(55001); // questions types (Y)
vector<int> Qround(55001); // question round (G)
vector<vint> Gtype(55001); // questions game types (P)
vector<int> Qanswer(55001); // answers for 12 and 17 types
vector<string> cityFromID(350);  // ID - city pair
vector<bool> playedInCity[350]; // first - city ID, second - question ID
vector<int> TypeCount(19); // questions count of i-th type
vector<int> CountNotParsed(19); // count of not parsed questions of i-th type
vector<int> not_parsed; // questions that weren't parsed but exist
map<string,int> errors_cnt;

void getQuestionTypes()
{
    for (int i=1; i<19; i++) {
        ifstream in (path_filter("По типам вопроса",i));
        string S; int line = 1;
        while (getline(in,S)) {
            vstring v = parse_by_symbol(S,' ');
            if (v.size() != 2) { cout << "Error in type " << i << ", line " << line << endl; continue; }
            int n = atoi(v[0].c_str());
            int round = atoi(v[1].c_str());
            if (round < 1 || round > 7) { cout << "Round in type " << i << ", line " << line << endl; continue; }
            
            Qtype[n] = i;
            Qround[n] = round;
            TypeCount[i]++;
            line++;
        }
    }
}

void getGameTypes()
{
    for (int i=1; i<661; i++) {
        ifstream in (path_filter("По типам игр",i));
        if (in.fail()) continue;
        string S; int line = 1;
        while (getline(in,S)) {
            vstring v = parse_by_symbol(S,' ');
            if (v.size() != 2) { cout << "Error in gtype " << i << ", line " << line << endl; continue; }
            int n = atoi(v[0].c_str());
            int round = atoi(v[1].c_str());
            if (round < 1 || round > 7) { cout << "Round in gtype " << i << ", line " << line << endl; continue; }
            
            Qround[n] = round; // no conflicts here
            Gtype[n].push_back(i);
            line++;
        }
    }
}

void getCitiesMapping()
{
    ifstream in (PATH + "city ids.txt");
    if (in.fail()) return;
    int n;
    while (in >> n) {
        string city;
        getline(in,city);
        trimSymbol(city, ' ');
        cityFromID[n] = city;
    }
}

void getPlayedQuestionsForCities()
{
    for (int i=1; i<350; i++) {
        playedInCity[i] = vector<bool>(55001);
        ifstream in (path_filter("По городам",i));
        if (in.fail()) continue;
        string S; int line = 1;
        while (getline(in,S)) {
            vstring v = parse_by_symbol(S,' ');
            if (v.size() != 2) { cout << "Error in city " << i << ", line " << line << endl; continue; }
            int n = atoi(v[0].c_str());
            int round = atoi(v[1].c_str());
            if (round < 1 || round > 7) { cout << "Round in city " << i << ", line " << line << endl; continue; }
            playedInCity[i][n] = true;
            line++;
        }
    }
}

void getAnwersWithOptions()
{
    ifstream in (path_filter("Блиц с 4 вариантам - ответы (1474)", 12));
    string S; int line = 1;
    while (getline(in,S)) {
        vstring v = parse_by_symbol(S,' ');
        if (v.size() != 2) { cout << "Error in blitz, line " << line << endl; continue; }
        int n = atoi(v[0].c_str());
        int answer = atoi(v[1].c_str());
        if (answer < 1 || answer > 4) { cout << "Answer in blitz, line " << line << endl; continue; }
        Qanswer[n] = answer;
        line++;
    }
    
    in = ifstream(path_filter("Музыка с 4 вариантами - ответы (110)", 17));
    line = 1;
    while (getline(in,S)) {
        vstring v = parse_by_symbol(S,' ');
        if (v.size() != 2) { cout << "Error in music, line " << line << endl; continue; }
        int n = atoi(v[0].c_str());
        int answer = atoi(v[1].c_str());
        if (answer < 1 || answer > 4) { cout << "Answer in music, line " << line << endl; continue; }
        Qanswer[n] = answer;
        line++;
    }
}

void fillCommonInformation()
{
    getQuestionTypes();
    getGameTypes();
    
    cout << "Looking at download material\n";
    int images_to_download = 0, audios_to_download = 0, videos_to_download = 0;
    for (int i=1; i<=55000; i++) {
        ifstream in (path_question_base("v3a",i));
        string S;
        int lines = 0;
        while (getline(in,S)) {
            lines++;
            if      (StringUtils::hasPrefix(S,"I:")) images_to_download++;
            else if (StringUtils::hasPrefix(S,"D:")) audios_to_download++;
            else if (StringUtils::hasPrefix(S,"V:")) videos_to_download++;
        }
        if (lines <= 3 && Qtype[i]) {
            cout << i << " " << Qtype[i] << endl;
            not_parsed.push_back(i);
            CountNotParsed[Qtype[i]]++;
        }
    }
    for (int i=1; i<19; i++) {
        cout << "Question Type #" << i << ": " << TypeCount[i] << " questions, " << CountNotParsed[i] << " empty\n";
    }
    cout << "Total number of empty questions: " << not_parsed.size() << endl;
    cout << "Total number of images: " << images_to_download << endl;
    cout << "Total number of audios: " << audios_to_download << endl;
    cout << "Total number of videos: " << videos_to_download << endl;
    
    not_parsed.push_back(15022022); // so npid won't go out of bounds
}

void get_media_values (int type, int k, int &image_lines, int &audio_lines, int &video_lines, int &text_lines)
{
    ifstream in (path_question_base("v3a",k));
    string S;
    image_lines = audio_lines = video_lines = text_lines = 0;
    vector<int> ok_text(14); // T,C,A or F1,F2,F3,C,CO,A or TO,B1,B2,B3,B4,CO are correct formats
    set<string> im_links, au_links, vi_links;
    
    while (getline(in,S)) {
        
        if      (StringUtils::hasPrefix(S,"I:")) image_lines++;
        else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
        else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        
        else if (StringUtils::hasPrefix(S,"A:"))  ok_text[0]++;
        else if (StringUtils::hasPrefix(S,"B:"))  ok_text[1]++;
        else if (StringUtils::hasPrefix(S,"C:"))  ok_text[2]++;
        else if (StringUtils::hasPrefix(S,"F:"))  ok_text[3]++;
        else if (StringUtils::hasPrefix(S,"N:"))  ok_text[4]++;
        else if (StringUtils::hasPrefix(S,"O:"))  ok_text[5]++;
        else if (StringUtils::hasPrefix(S,"P:"))  ok_text[6]++;
        else if (StringUtils::hasPrefix(S,"Q:"))  ok_text[7]++;
        else if (StringUtils::hasPrefix(S,"R:"))  ok_text[8]++;
        else if (StringUtils::hasPrefix(S,"S:"))  ok_text[9]++;
        else if (StringUtils::hasPrefix(S,"T:"))  ok_text[10]++;
        else if (StringUtils::hasPrefix(S,"U:"))  ok_text[11]++;
        else if (StringUtils::hasPrefix(S,"W:"))  ok_text[12]++;
        else if (StringUtils::hasPrefix(S,"Z:"))  ok_text[13]++;
    }
    text_lines = total_vector_sum(ok_text);
}

void research (int type, const vint &ok_value, const vint &warning_value, int expected_text = 11, bool should_log_errors = true)
{
    int npid = 0;
    int ok = 0, warning = 0, error = 0, text_errors = 0;
    cout << "\nLooking at type " << type << ": " << TYPE_NAMES[type] << endl;
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (Qtype[i] != type) continue;
        vector<int> A(3);
        int text_lines;
        get_media_values(type, i, A[0], A[1], A[2], text_lines);
        
        if (text_lines != expected_text) {
            text_errors++;
            if (should_log_errors) cout << "#" << i << ": " << text_lines << " " << expected_text << endl;
        }
        if (A == ok_value) { ok++; continue; }
        if (A == warning_value) { warning++; continue; }
        error++;
        if (should_log_errors) {
            cout << "Question #" << i << " got error: "; show(A,false);
            cout << ", expected "; show(ok_value);
        }
    }
    if (should_log_errors) {
        cout << "OK: " << ok << endl;
        cout << "Warning: " << warning << endl;
        cout << "Error: " << error << endl;
        cout << "Not parsed: " << CountNotParsed[type] << endl;
        cout << "Missing text: " << text_errors << endl;
    }
}

void research_for_errors()
{
    fillCommonInformation();
    //          I D V               // v2 base revision
    research(1,{1,0,0},{0,0,0});    // 106 failed to download + 1492 have no image (probably ok)
    research(2,{1,0,0},{});         // OK
    research(3,{0,1,0},{});         // OK
    research(4,{0,0,1},{});         // 1 failed to download + 1 without video (#32095)
    research(5,{2,0,0},{});         // 37 failed to download + 48 with missing images
    research(6,{0,2,0},{});         // 1 failed to download
    research(7,{0,0,2},{});         // OK
    research(8,{1,3,0},{});         // 4 failed to download + 19 with missing audios or image
    research(9,{1,0,1},{});         // OK
    research(10,{0,1,1},{});        // OK, no questions of this type
    research(11,{1,0,0},{0,0,0},15);   // 1 failed to download + 19 have no image (only pages #51567 and #51708 have them)
    research(12,{1,0,0},{0,0,0},14);   // 16 have no image (maybe this is ok)
    research(13,{0,0,3},{});        // 1 has no videos (#26448)
    research(14,{1,1,1},{});        // OK, no questions of this type
    research(15,{2,1,0},{});        // OK
    research(16,{1,2,0},{});        // 2 have no media (#45610, #45613), but their pages have
    research(17,{1,2,0},{},14);        // OK
    research(18,{1,1,0},{1,0,0},15);   // OK, 10 have no audio, but their pages also don't
}

// 1: ACISTZ      (example #53315)
// 2: ACISTZ      (example #14974)
// 3: ACDSTZ      (example #29728)
// 4: ACSTVZ      (example #53619)
// 5: ACIISTZ     (example #53511)
// 6: ACDDSTZ     (example #33840)
// 7: ACSTVVZ     (example #52203)
// 8: ACDDDISTZ   (example #53527)
// 9: ACISTVZ     (example #51892)
//11: ACFFFIOSTZ  (example #53360) - here we don't need O, it just copies first F
//12: BBBBIOSTZ   (example #53480)
//13: ACSTVVVZ    (example #53008)
//15: ACDIISTZ    (example #37409)
//16: ACDDISTZ    (example #45493)
//17: BBBBDDIOSTZ (example #44150)
//18: ACDFFFIOSTZ (example #43458) - here we don't need O, it just copies first F

struct Question {
    bool is_present;
    int id;   // 1..~54000
    string answer;                // A
    vector<string> options;       // B
    string connection;            // C
    vector<string> audio_files;   // D
    vector<string> facts;         // F
    vector<string> image_files;   // I
    string not_accept;            // N
    vector<string> path;          // P
    string question_commentary;   // Q
    vector<string> played_cities; // R
    string source;                // S
    string text;                  // T
    string author;                // U
    vector<string> video_files;   // V
    string answer_commentary;     // W
    int type; // 1..18            // Y
    string accept;                // Z
    vector<string> media;
    
    Question (string version, int qid)
    {
        is_present = false;
        id = qid;
        type = Qtype[qid];
        ifstream in (path_question_base(version, qid));
        if (in.fail()) return;
        is_present = true;
        int N = 0;
        vector<string> elems(30);
        while (getline(in,elems[N++]));
        
        // connect elems of one category, separated by endline symbol
        for (int i=N-1; i>0; i--) { // skipping first QUESTION #id string
            if ((elems[i].length() < 2 || elems[i][1] != ':')) { // tested: no bug exceptions here
                elems[i-1] += '\n';
                elems[i-1] += elems[i];
                elems.erase(elems.begin()+i);
                N--;
            }
        }
        
        for (int i=1; i<N; i++) { // same skip
            string S = elems[i];
            string suf; if (S.length() > 3) suf = S.substr(3,S.length()-3);
            if      (hasPrefix(S,"A:")) answer = suf;
            else if (hasPrefix(S,"B:")) options.push_back(suf);
            else if (hasPrefix(S,"C:")) connection = suf;
            else if (hasPrefix(S,"D:")) { trimSymbol(suf,'\n'); audio_files.push_back(suf); media.push_back(S); }
            else if (hasPrefix(S,"F:")) facts.push_back(suf);
            else if (hasPrefix(S,"I:")) { trimSymbol(suf,'\n'); image_files.push_back(suf); media.push_back(S); }
            else if (hasPrefix(S,"N:")) not_accept = suf;
            else if (hasPrefix(S,"O:")) {
                if (Qtype[qid] == 12 || Qtype[qid] == 17) connection = suf;
            }
            else if (hasPrefix(S,"P:")) {
                vector<string> v = parse_by_symbol(suf,'/');
                for (int i=0; i<(int)v.size(); i++) { trimSymbol(v[i],' '); path.push_back(v[i]); }
            }
            else if (hasPrefix(S,"Q:")) question_commentary = suf;
            else if (hasPrefix(S,"R:")) {}
            else if (hasPrefix(S,"S:")) source = suf;
            else if (hasPrefix(S,"T:")) text = suf;
            else if (hasPrefix(S,"U:")) author = suf;
            else if (hasPrefix(S,"V:")) { trimSymbol(suf,'\n'); video_files.push_back(suf); media.push_back(S); }
            else if (hasPrefix(S,"W:")) answer_commentary = suf;
            else if (hasPrefix(S,"Z:")) accept = suf;
            
            else { // no such cases right now for v3a base
                cout << "WARNING: unrecognized string for qid = " << qid << ": " << S << endl;
            }
        }
        
        // fields, retrieving from filters, not just from questions base
        for (int i=1; i<350; i++) {
            if (playedInCity[i][qid]) played_cities.push_back(cityFromID[i]);
        }
        
        if (type == 12 || type == 17) {
            if (Qanswer[qid] < 1 || Qanswer[qid] > 4) cout << "Error answer " << qid << endl;
            answer = to_string(Qanswer[qid]) + ". " + options[Qanswer[qid]-1];
        }
    }
    
    void write_to_base (string version)
    {
        if (!is_present) return;
        ofstream out (path_question_base(version,id));
        
        // Путь типа игры
        out << "P: " << join_by_symbol(path,'/') << endl;
        if (path.empty()) errors_cnt["P"]++;
        
        // Тип вопроса
        out << "Y: " << TYPE_NAMES[type] << endl;
        if (type < 1 || type > 18) errors_cnt["Y"]++;
        
        // Сыгранные города
        out << "R: " << join_by_symbol(played_cities, ',') << endl;
        
        // Стандарт вопроса
        out << "G: " << Qround[id] << endl;
        if (Qround[id] < 1 || Qround[id] > 7) errors_cnt["G"]++;
        
        // Текст вопроса
        out << "T: " << text << endl;
        if (text.empty()) errors_cnt["T"]++;
        
        // Факты
        for (int i=0; i<(int)facts.size(); i++) out << "F: " << facts[i] << endl;
        if ((type == 11 || type == 18) && facts.size() != 3) errors_cnt["F1"]++;
        if (type != 11 && type != 18 && !facts.empty()) errors_cnt["F2"]++;
        
        // Комментарий ведущему на вопрос
        out << "Q: " << question_commentary << endl;
        
        // Источник
        out << "S: " << source << endl;
        
        // Подводка
        out << "C: " << connection << endl;
        
        // Ответ
        out << "A: " << answer << endl;
        if (answer.empty()) errors_cnt["A"]++;
        
        // Варианты ответа
        for (int i=0; i<(int)options.size(); i++) out << "B: " << options[i] << endl;
        if ((type == 12 || type == 17) && options.size() != 4) errors_cnt["B1"]++;
        if (type != 12 && type != 17 && !options.empty()) errors_cnt["B2"]++;
        
        // Комментарий ведущему на ответ
        out << "W: " << answer_commentary << endl;
        
        // Зачёт
        out << "Z: " << accept << endl;
        
        // Незачёт
        out << "N: " << not_accept << endl;
        
        // Автор
        out << "U: " << author << endl;
        
        // Медиа
        for (int i=0; i<(int)media.size(); i++) out << media[i] << endl;
    }
};

void prepare()
{
    getQuestionTypes();
    getGameTypes();
    getCitiesMapping();
    getPlayedQuestionsForCities();
    getAnwersWithOptions();
    errors_cnt.clear();
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
    
    prepare();
    cout << "Start writing...\n";
    for (int i=1; i<55000; i++) {
        Question q("v3a",i);
        q.write_to_base("v4");
    }
    for (map<string,int>::iterator it=errors_cnt.begin(); it != errors_cnt.end(); it++) {
        cout << it->fs << " " << it->sc << endl;
    }
    
    // PLAYED CITIES
//    for (int i=0; i<cityFromID.size(); i++) {
//        if (!cityFromID[i].empty()) {
//            cout << i << " " << cityFromID[i] << " ";
//            int cnt_played = 0;
//            for (int j=0; j<55000; j++) if (playedInCity[i][j]) cnt_played++;
//            cout << cnt_played << endl;
//        }
//    }
    
    //research_for_errors(); // 801 question has no question text!!!
    
    //
//    set<string> s;
//    for (int i=1; i<55000; i++) {
//        Question q("v3a",i);
//        if (q.is_present && !q.author.empty()) {
//            s.insert(q.author);
//        }
//    }
//    show(s);

    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
