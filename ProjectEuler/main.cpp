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

const vector<string> TYPE_NAMES = {"",
    "Текст + картинка необязательно",
    "Текст + ответ с картинкой",
    "Текст + ответ с музыкой",
    "Текст + ответ с видео",
    "Вопрос с картинкой + ответ с картинкой",
    "Вопрос с аудио + ответ с аудио",
    "Вопрос с видео + ответ с видео",
    "Музыкальный (3 файла + картинка)",
    "Вопрос с картинкой + ответ с видео",
    "Вопрос с аудио + ответ с видео",
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

string path_city (int city) {
    return PATH + "Фильтры/По городам/" + to_string(city) + ".txt";
}
string path_question_type (int type) {
    return PATH + "Фильтры/По типам вопроса/" + to_string(type) + ".txt";
}
string path_question_text (int id) {
    return PATH + "База вопросов v3a/" + to_string(id) + ".txt";
}

int with_image, without_image, with_double_image, with_multiple_images, with_audio, without_audio, with_video, without_video, npid;

void zeroset() {
    with_image = without_image = with_double_image = with_multiple_images = with_audio = without_audio = with_video = without_video = npid = 0;
}

vector<int> Qtype(55001); // questions types
vector<int> TypeCount(19); // questions count of i-th type
vector<int> CountNotParsed(19); // count of not parsed questions of i-th type
vector<int> not_parsed; // questions that weren't parsed but exist

void fillCommonInformation()
{
    // getting question types
    for (int i=1; i<19; i++) {
        ifstream in (path_question_type(i));
        int n, not_now;
        while (in >> n) {
            in >> not_now;
            Qtype[n] = i;
            TypeCount[i]++;
        }
    }
        
    cout << "Looking at download material\n";
    int images_to_download = 0, audios_to_download = 0, videos_to_download = 0;
    for (int i=1; i<=55000; i++) {
        ifstream in (path_question_text(i));
        string S;
        int lines = 0;
        while (getline(in,S)) {
            lines++;
            if (StringUtils::hasPrefix(S,"I:"))      images_to_download++;
            else if (StringUtils::hasPrefix(S,"D:")) audios_to_download++;
            else if (StringUtils::hasPrefix(S,"V:")) videos_to_download++;
        }
        if (lines <= 3 && Qtype[i]) {
            not_parsed.push_back(i);
            CountNotParsed[Qtype[i]]++;
        }
    }
    for (int i=1; i<19; i++) {
        cout << "Question Type #" << i << ": " << TypeCount[i] << " questions, " << CountNotParsed[i] << " empty\n";
    }
    cout << "Total number of empty questions: " << not_parsed.size() << endl;
    cout << "Tota number of images: " << images_to_download << endl;
    cout << "Tota number of audios: " << audios_to_download << endl;
    cout << "Tota number of videos: " << videos_to_download << endl;
    
    not_parsed.push_back(15022022); // so npid won't go out of bounds
}

void get_media_values (int type, int k, int &image_lines, int &audio_lines, int &video_lines, int &text_lines)
{
    ifstream in (path_question_text(k));
    string S;
    image_lines = audio_lines = video_lines = text_lines = 0;
    vector<int> ok_text(14); // T,C,A or F1,F2,F3,C,CO,A or TO,B1,B2,B3,B4,CO are correct formats
    set<string> im_links, au_links, vi_links;
    while (getline(in,S)) {
        if      (StringUtils::hasPrefix(S,"I:")) im_links.insert(S.substr(3,S.length()-3));
        else if (StringUtils::hasPrefix(S,"D:")) au_links.insert(S.substr(3,S.length()-3));
        else if (StringUtils::hasPrefix(S,"V:")) vi_links.insert(S.substr(3,S.length()-3));
        
        image_lines = (int)im_links.size();
        audio_lines = (int)au_links.size();
        video_lines = (int)vi_links.size();
        
        if      (StringUtils::hasPrefix(S,"A:"))  ok_text[0]++;
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

void research (int type, const vint &ok_value, const vint &warning_value, int expected_text = 3)
{
    zeroset();
    int ok = 0, warning = 0, error = 0, text_errors = 0;
    cout << "\nLooking at type " << type << ": " << TYPE_NAMES[type] << endl;
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (Qtype[i] != type) continue;
        vector<int> A(3);
        int text_lines;
        get_media_values(type, i, A[0], A[1], A[2], text_lines);
        
        if (text_lines != expected_text) { text_errors++; cout << "#" << i << ": " << text_lines << " " << expected_text << endl; }
        if (A == ok_value) { ok++; continue; }
        if (A == warning_value) { warning++; continue; }
        error++;
        cout << "Question #" << i << " got error: "; show(A,false);
        cout << ", expected "; show(ok_value);
    }
    cout << "OK: " << ok << endl;
    cout << "Warning: " << warning << endl;
    cout << "Error: " << error << endl;
    cout << "Not parsed: " << CountNotParsed[type] << endl;
    cout << "Missing text: " << text_errors << endl;
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
    
    fillCommonInformation();
    //          I D V
    research(1,{1,0,0},{0,0,0},11);    // 106 failed to download + 1492 have no image (probably ok)
    research(2,{1,0,0},{},11);         // OK
    research(3,{0,1,0},{},11);         // OK
    research(4,{0,0,1},{},11);         // 1 failed to download + 1 without video (#32095)
    research(5,{2,0,0},{},11);         // 37 failed to download + 48 with missing images
    research(6,{0,2,0},{},11);         // 1 failed to download
    research(7,{0,0,2},{},11);         // OK
    research(8,{1,3,0},{},11);         // 4 failed to download + 19 with missing audios or image
    research(9,{1,0,1},{},11);         // OK
    research(10,{0,1,1},{},11);        // OK, no questions of this type
    research(11,{1,0,0},{0,0,0},15);   // 1 failed to download + 19 have no image (only pages #51567 and #51708 have them)
    research(12,{1,0,0},{0,0,0},14);   // 16 have no image (maybe this is ok)
    research(13,{0,0,3},{},11);        // 1 has no videos (#26448)
    research(14,{1,1,1},{},11);        // OK, no questions of this type
    research(15,{2,1,0},{},11);        // OK
    research(16,{1,2,0},{},11);        // 2 have no media (#45610, #45613), but their pages have
    research(17,{1,2,0},{},14);        // OK
    research(18,{1,1,0},{1,0,0},15);   // OK, 10 have no audio, but their pages also don't
    
    ans=55000;
    while (!Qtype[ans--]);
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
