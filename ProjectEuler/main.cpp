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

const string PATH = "/Users/Miroslav/Documents/MZGB Diary/Mzgb Hackers/";

string path_city (int city) {
    return PATH + "Фильтры/По городам/" + to_string(city) + ".txt";
}
string path_question_type (int type) {
    return PATH + "Фильтры/По типам вопроса/" + to_string(type) + ".txt";
}
string path_question_text (int id) {
    return PATH + "База вопросов/" + to_string(id) + ".txt";
}

int with_image, without_image, with_double_image, with_multiple_images, with_audio, without_audio, with_video, without_video, npid;

void zeroset() {
    with_image = without_image = with_double_image = with_multiple_images = with_audio = without_audio = with_video = without_video = npid = 0;
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
    
    vector<int> A(55001); // questions types
    vector<int> cnt(19); // questions count of i-th type
    for (int i=1; i<19; i++) {
        ifstream in (path_question_type(i));
        int n, not_now;
        while (in >> n) {
            in >> not_now;
            A[n] = i;
            cnt[i]++;
        }
    }
    
    vector<int> cnt_np(19); // count of not parsed questions of i-th type
    int images_to_download = 0, audio_to_download = 0, video_to_download = 0;
    vector<int> not_parsed;
    
    cout << "Looking at download material\n";
    for (int i=1; i<=55000; i++) {
        ifstream in (path_question_text(i));
        string S;
        int lines = 0;
        while (getline(in,S)) {
            lines++;
            if (StringUtils::hasPrefix(S,"I:"))     images_to_download++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_to_download++;
            else if (StringUtils::hasPrefix(S,"V:")) video_to_download++;
        }
        if (lines <= 3 && A[i]) {
            not_parsed.push_back(i);
            cnt_np[A[i]]++;
        }
    }
    for (int i=1; i<19; i++) {
        cout << "Type #" << i << ": " << cnt[i] << " questions, " << cnt_np[i] << " empty\n";
    }
    cout << not_parsed.size() << " " << images_to_download << " " << audio_to_download << " " << video_to_download << endl;
    not_parsed.push_back(15022022); // so npid won't go out of bounds
    
    cout << "\nLooking at type 1 and 2 - Текст + картинка необязательно\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 1 && A[i] != 2) continue;
        ifstream in (path_question_text(i));
        string S;
        int im_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) im_lines++;
        }
        if (im_lines == 0) without_image++;
        else if (im_lines == 1) with_image++;
        else with_multiple_images++;
    } cout << with_image << " " << without_image << " " << with_multiple_images/*empty, no anomalies here*/ << endl;
    // 106 failed to download + 1492 have no image
    
    cout << "\nLooking at type 3 - Текст + ответ с музыкой\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 3) continue;
        ifstream in (path_question_text(i));
        string S;
        int audio_lines = 0;
        bool has_image = false;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) has_image = true;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
        }
        if (has_image) with_image++;
        with_audio += audio_lines;
    } cout << with_audio << " " << with_image << endl; // 34 0, no anomalies
    
    cout << "\nLooking at type 4 - Текст + ответ с видео\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 4) continue;
        ifstream in (path_question_text(i));
        string S;
        int video_lines = 0;
        bool has_image = false;
        bool has_audio = false;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) has_image = true;
            else if (StringUtils::hasPrefix(S,"D:")) has_audio = true;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        if (has_image) with_image++;
        if (has_audio) with_audio++;
        with_video += video_lines;
        if (!video_lines) cout << "No video " << i << endl;
    } cout << with_video << " " << with_audio << " " << with_image << endl;
    // 1 failed to download (questionID = 24)
    // 1 has no video link, but question page does have it (questionID = 32095)
    // no other anomalies
    
    cout << "\nLooking at type 5 - Вопрос с картинкой + ответ с картинкой\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 5) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        bool has_video = false;
        bool has_audio = false;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"V:")) has_video = true;
            else if (StringUtils::hasPrefix(S,"D:")) has_audio = true;
            else if (StringUtils::hasPrefix(S,"I:")) image_lines++;
        }
        if (has_video) with_video++;
        if (has_audio) with_audio++;
        if (image_lines == 0) { without_image++; cout << "No image " << i << endl; }
        else if (image_lines == 1) { with_image++; cout << "One image " << i << endl; }
        else if (image_lines == 2) with_double_image++;
        else with_multiple_images++; // no cases
    } cout << without_image << " " << with_image << " " << with_double_image << " " << with_multiple_images << " " << with_video << " " << with_audio << " " << endl;
    // 37 failed to download
    // 19 have no image
    // 29 have only one image
    // 8300 have two images, OK
    // no audio, video, 3+ images anomalies
    
    cout << "\nLooking at type 6 - Вопрос с аудио + ответ с аудио\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 6) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        if (image_lines) cout << "IWTF\n";
        if (video_lines) cout << "VWTF\n";
        with_audio += audio_lines;
        if (audio_lines != 2) cout << "Audio mismatch: " << i << " has " << audio_lines << " files\n";
    } cout << with_audio << " " << with_video << " " << with_image << endl;
    // 1 failed to download, that's all
    
    cout << "\nLooking at type 7 - Вопрос с видео + ответ с видео\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 7) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        if (audio_lines) cout << "AWTF\n";
        if (image_lines) cout << "IWTF\n";
        with_video += video_lines;
        if (video_lines != 2) cout << "Video mismatch: " << i << " has " << video_lines << " files\n";
    } cout << with_video << " " << with_audio << " " << with_image << endl; // 1098 0 0
    // no anomalies
    
    cout << "\nLooking at type 8 - Музыкальный (3 файла + картинка)\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 8) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        if (video_lines) cout << "VWTF\n";
        with_video += video_lines;
        with_audio += audio_lines;
        with_image += image_lines;
        if (audio_lines != 3 || image_lines != 1) cout << "Music mismatch: " << i << " has " << audio_lines << " audio files and " << image_lines << " image files\n";
    } cout << with_audio << " " << with_image << " " << with_video << endl;
    // 19 mismatches + 4 failed to download
    
    cout << "\nLooking at type 9 - Вопрос с картинкой + ответ с видео\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 9) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        if (audio_lines) cout << "VWTF\n";
        with_video += video_lines;
        with_audio += audio_lines;
        with_image += image_lines;
        if (image_lines != 1 || video_lines != 1) cout << "Video mismatch: " << i << " has " << video_lines << " video files and " << image_lines << " image files\n";
    } cout << with_video << " " << with_image << " " << with_audio << endl; // 172 172 0
    // Oll Korrect
    
    cout << "\nType 10 - nothing to look at\n";
    
    cout << "\nLooking at type 11 - 3 факта о\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 11) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        with_video += video_lines;
        with_audio += audio_lines;
        if (image_lines == 0) { without_image++; /*cout << "No image " << i << endl;*/ }
        else if (image_lines == 1) with_image++;
        else with_multiple_images++;
    } cout << with_image << " " << without_image << " " << with_multiple_images << " " << with_audio << " " << with_video << endl;
    // 19 without images, but only pages #51567 and #51708 have them. Also 1 falied to download
    
    cout << "\nLooking at type 12 - Блиц с 4 вариантами\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 12) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        with_video += video_lines;
        with_audio += audio_lines;
        if (image_lines == 0) { without_image++; /*cout << "No image " << i << endl;*/ }
        else if (image_lines == 1) with_image++;
        else with_multiple_images++;
    } cout << with_image << " " << without_image << " " << with_multiple_images << " " << with_audio << " " << with_video << endl;
    // 16 without images
    
    cout << "\nLooking at type 13 - Видеотур\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 13) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        if (audio_lines) cout << "AWTF\n";
        if (image_lines) cout << "IWTF\n";
        with_video += video_lines;
        if (video_lines != 3) cout << "Video mismatch: " << i << " has " << video_lines << " files\n";
    } cout << with_video << " " << with_audio << " " << with_image << endl; // 2322 0 0
    // 1 has no videos (#26448), but the corresponsding page has
    
    cout << "\nType 14 - nothing to look at\n";
    
    cout << "\nLooking at type 15 - Вопрос с картинкой + ответ с картинкой и музыкой\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 15) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        if (image_lines == 0) without_image++;
        else if (image_lines == 1) with_image++;
        else if (image_lines == 2) with_double_image++;
        else with_multiple_images++;
        if (image_lines != 2 || audio_lines != 1) cout << "Format mismatch: " << i << " has " << audio_lines << " audio files and " << image_lines << " image files\n";
    } cout << without_image << " " << with_image << " " << with_double_image << " " << with_multiple_images << " " << with_video << " " << with_audio << " " << endl;
    // Oll Korrect
    
    cout << "\nLooking at type 16 - Музыкальный без повторов (2 файла + картинка)\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 16) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        if (video_lines) cout << "VWTF\n";
        with_video += video_lines;
        with_audio += audio_lines;
        with_image += image_lines;
        if (audio_lines != 2 || image_lines != 1) cout << "Music mismatch: " << i << " has " << audio_lines << " audio files and " << image_lines << " image files\n";
    } cout << with_audio << " " << with_image << " " << with_video << endl;
    // 2 have no media (#45610, #45613), but their pages have
    
    cout << "\nLooking at type 17 - 4 варианта и музыка + ответ с картинкой и музыкой\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 17) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        if (video_lines) cout << "VWTF\n";
        with_video += video_lines;
        with_audio += audio_lines;
        with_image += image_lines;
        if (audio_lines != 2 || image_lines != 1) cout << "Music mismatch: " << i << " has " << audio_lines << " audio files and " << image_lines << " image files\n";
    } cout << with_audio << " " << with_image << " " << with_video << endl;
    // Oll Korrect
    
    cout << "\nLooking at type 18 - 3 строчки о\n";
    zeroset();
    for (int i=1; i<=55000; i++) {
        if (not_parsed[npid] == i) { npid++; continue; }
        if (A[i] != 18) continue;
        ifstream in (path_question_text(i));
        string S;
        int image_lines = 0;
        int audio_lines = 0;
        int video_lines = 0;
        while (getline(in,S)) {
            if (StringUtils::hasPrefix(S,"I:")) image_lines++;
            else if (StringUtils::hasPrefix(S,"D:")) audio_lines++;
            else if (StringUtils::hasPrefix(S,"V:")) video_lines++;
        }
        with_video += video_lines;
        with_audio += audio_lines;
        if (image_lines == 0) without_image++;
        else if (image_lines == 1) with_image++;
        else with_multiple_images++;
        if (audio_lines != 1 || image_lines != 1) cout << "Music mismatch: " << i << " has " << audio_lines << " audio files and " << image_lines << " image files\n";
    } cout << with_image << " " << without_image << " " << with_multiple_images << " " << with_audio << " " << with_video << endl;
    // 10 have no audio, but their pages also don't, so, everything is ok here
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
