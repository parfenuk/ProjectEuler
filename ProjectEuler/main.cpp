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

#include "json.hpp"
using json = nlohmann::json;

using namespace Algebra;
using namespace Containers;

json gem_offers()
{
    json j = json::array();
    vint count = {1200,3000,6500};
    vdd price_usd = {3.99,6.99,14.99};
    for (int i=0; i<3; i++) {
        json g = json::object();
        g["count"] = count[i];
        g["price_usd"] = price_usd[i];
        j.push_back(g);
    }
    return j;
}

json streak_offers()
{
    json j = json::array();
    vint count = {1,3,5};
    vint price_gems = {150,350,500};
    for (int i=0; i<3; i++) {
        json g = json::object();
        g["count"] = count[i];
        g["price_gems"] = price_gems[i];
        j.push_back(g);
    }
    return j;
}

json streak_rewards()
{
    json j = json::array();
    vint days = {7,14,30};
    vint gems = {50,100,250};
    for (int i=0; i<3; i++) {
        json g = json::object();
        g["days"] = days[i];
        g["gems"] = gems[i];
        j.push_back(g);
    }
    return j;
}

json skill_level_up_rewards()
{
    json j = json::object();
    j["math"] = 120;
    j["logic"] = 120;
    j["focus"] = 120;
    return j;
}

json progress_percentage_rewards()
{
    json j = json::object();
    j["per_game"] = 5;
    j["per_level_good"] = 5;
    j["per_level_perfect"] = 10;
    return j;
}

json gem_rewards()
{
    json j = json::object();
    j["per_2048_tile"] = 60;
    j["per_level_good"] = 5;
    j["per_level_perfect"] = 20;
    return j;
}

json gems_spend_costs()
{
    json j = json::object();
    j["lives_refill"] = 300;
    j["bo_use_hint"] = 50;
    j["bo_skip_level"] = 100;
    j["true_false_use_hint"] = 50;
    j["test_use_hint"] = 50;
    j["input_use_hint"] = 50;
    j["balance_use_hint"] = 50;
    j["2048_move_back"] = 100;
    j["2048_use_hammer"] = 100;
    j["2048_continue"] = 200;
    return j;
}

json global_mechanic()
{
//    static let initialGems = 100
//    static let subscriptionGems = 500
//    static let livesMaxCount = 5
//    static let lifeRefillCooldown = 18000
//    static let streakRewards: [Int : Int] = [7:50, 14:100, 30:250]
    json j = json::object();
    j["initial_gems"] = 100;
    j["subscription_gems"] = 500;
    j["lives_max_count"] = 5;
    j["live_auto_refill_duration_seconds"] = 18000;
    j["min_percentage_for_good"] = 60;
    j["min_percentage_for_perfect"] = 90;
    j["gems_spend_costs"] = gems_spend_costs();
    j["gem_rewards"] = gem_rewards();
    j["progress_percentage_rewards"] = progress_percentage_rewards();
    j["skill_level_up_rewards"] = skill_level_up_rewards();
    j["streak_rewards"] = streak_rewards();
    j["gem_offers"] = gem_offers();
    j["streak_offers"] = streak_offers();
    
    return j;
}

vstring games = {"TEST", "TRUE_FALSE", "INPUT", "BALANCE", "BRAIN_OVER", "FIND_DIFFERENCES", "MATCHES"};
vint points_earned = {15,10,30,20,25,20,20};
vint math_difficulties = {1,1,1,1};
vint level_indeces = {0,0,0};
vint bo_levels = {56, 41, 12, 11, 58, 4, 2, 66, 3, 62, 27, 44, 50, 22, 15, 7, 9, 5, 8, 17, 23, 29, 13, 35, 10, 18, 26, 33, 24, 39, 6, 16, 51, 37, 20, 31, 36, 28, 19, 30, 49, 25, 1, 40, 38, 43, 45, 46, 47, 48, 52, 54, 55, 60, 61, 63, 64, 65, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 90, 91, 92, 93, 94, 95, 53, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198};
vint fd_levels = {168,161,163,6,2,214,3,4,213,5,7,218,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45};
vint ms_levels = {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107};

int random_game()
{
    vint random_game_mapping = {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,3,5,2,1,0,0,1,2,3,0,1,2,3,4,0,1,2,3,4,0,1,2,3,0,1,2,3,4,0,1,2,3,0,1,2,3,0,1,2,3};
    int game = (int)Utils::random_integer(0,(int)random_game_mapping.size()-1);
    return random_game_mapping[game];
}

vvint generate_challenges_count() // each level is 7 numbers: count of each challenge
{
    vvint A(100);
    for (int i=0; i<100; i++) A[i] = vint(6); // except matches
        
    const vint block_main_number = {3,3,3,4,4,4,5,5,5,6,6,7,7,7,8,8,8,9,9,9};
    
    for (int i=0; i<100; i+=5) {
        int n = block_main_number[i/5];
        
        int game0 = random_game();
        A[i][game0] = n-1;
        
        int game1 = random_game();
        while (game1 == game0) { game1 = random_game(); }
        A[i+1][game1] = n;
        
        int game21 = random_game();
        while (game21 == game0 || game21 == game1) { game21 = random_game(); }
        int game22 = random_game();
        while (game22 == game21) { game22 = random_game(); }
        A[i+2][game21] = n;
        A[i+2][game22] = n;
        
        int game31 = random_game();
        while (game31 == game0 || game31 == game1 || game31 == game21 || game31 == game22) { game31 = random_game(); }
        int game32 = random_game();
        while (game32 == game31) { game32 = random_game(); }
        int game33 = random_game();
        while (game33 == game31 || game33 == game32 ) { game33 = random_game(); }
        A[i+3][game31] = n-1;
        A[i+3][game32] = n-1;
        A[i+3][game33] = n-1;
        
        for (int j=0; j<6; j++) A[i+4][j] = n-2;
    }
    
    vint B(6);
    for (int i=0; i<100; i++) for (int j=0; j<6; j++) B[j] += A[i][j];
    while (B[4] > (int)bo_levels.size()) {
        for (int i=0; i<100 && B[4] > (int)bo_levels.size(); i++) {
            if (A[i][4] > 0) { A[i][4]--; B[4]--; }
        }
    }
    while (B[5] > (int)fd_levels.size()) {
        for (int i=0; i<100 && B[5] > (int)fd_levels.size(); i++) {
            if (A[i][5] > 0) { A[i][5]--; B[5]--; }
        }
    }
    
    return A;
}

json challenges_json (vint B, vstring math_signs)
{
    vector<json> challenges;
    for (int i=0; i<4; i++) {
        if (B[i] == 0) continue;
        json c = json::object();
        c["type"] = games[i];
        c["points_earned"] = points_earned[i];
        json math = json::object();
        math["count"] = B[i];
        json signs = json::array();
        for (int k=0; k<(int)math_signs.size(); k++) signs.push_back(math_signs[k]);
        math["signs"] = signs;
        math["initial_difficulty"] = math_difficulties[i];
        math["difficulty_step"] = 1;
        c["math_params"] = math;
        math_difficulties[i] += B[i];
        
        challenges.push_back(c);
    }
    for (int i=4; i<6; i++) {
        if (B[i] == 0) continue;
        json c = json::object();
        c["type"] = games[i];
        c["points_earned"] = points_earned[i];
        json level_ids = json::array();
        for (int z=0; z<B[i]; z++) {
            int id = i == 4 ? bo_levels[level_indeces[0]] : fd_levels[level_indeces[1]];
            level_ids.push_back(id);
            level_indeces[i-4]++;
        }
        c["level_ids"] = level_ids;
        
        challenges.push_back(c);
    }
    
    random_device rd;
    default_random_engine rng(rd());
    shuffle(challenges.begin(), challenges.end(), rng);
    json j = json::array();
    for (int i=0; i<(int)challenges.size(); i++) j.push_back(challenges[i]);
    return j;
}

json matches_json()
{
    json c = json::object();
    c["type"] = games[6];
    c["points_earned"] = points_earned[6];
    c["level_ids"] = json::array({ms_levels[level_indeces[2]]});
    level_indeces[2]++;
    return c;
}

json levels()
{
    vvint A;
    while (true) {
        bool ok = true;
        A = generate_challenges_count();
        vint B(6);
        for (int i=0; i<100; i++) for (int j=0; j<6; j++) B[j] += A[i][j];
        if (B[4] < (int)bo_levels.size() || B[5] < (int)fd_levels.size()) continue;
        for (int i=0; i<100; i++) {
            if (total_vector_sum(A[i]) < 2) { ok = false; break; }
        }
        if (ok) break;
    }
    
    const vector<vstring> signs = {{"+"},{"+","-"},{"+","-","*"},{"+","-","*","/"},{"+","-","*","/"}};
    const vint blocks_to_add_matches = {0,2,4,6,8,9,10,12,14,16,18,19};
    
    json j = json::array();
    
    for (int i=0; i<100; i+=5) {
        // Add levels block
        for (int k=0; k<5; k++) {
            json level = json::object();
            string difficulty;
            if (total_vector_sum(A[i+k]) < 9) difficulty = "EASY";
            else if (total_vector_sum(A[i+k]) < 15) difficulty = "MEDIUM";
            else difficulty = "HARD";
            level["difficulty"] = difficulty;
            level["challenges"] = challenges_json(A[i+k], signs[k]);
            if (k == 4 && contains(blocks_to_add_matches, i/5)) {
                level["challenges"].push_back(matches_json());
            }
            j.push_back(level);
        }
        // Add chest
        json chest = json::object();
        json params = json::object();
        params["type"] = "CHEST";
        params["points_earned"] = 10*(i/5 + 1);
        chest["challenges"] = json::array();
        chest["challenges"].push_back(params);
        j.push_back(chest);
    }
    
    return j;
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
    
    random_device rd;
    default_random_engine rng(rd());
    shuffle(bo_levels.begin(), bo_levels.end(), rng);
    
    json root = json::object();
    root["global_mechanic"] = global_mechanic();
    root["levels"] = levels();
    cout << root.dump(4) << endl;
    
    cout << bo_levels.size() << " " << fd_levels.size() << " " << ms_levels.size() << endl;
    
    cout << endl << ans << endl;
    Total_Time = clock() - Total_Time;
    cout << "Running time: " << ((float)Total_Time)/CLOCKS_PER_SEC << " seconds\n";
    
    return 0;
}
