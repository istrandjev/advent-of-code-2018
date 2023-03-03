#define _CRT_SECURE_NO_DEPRECATE
#include <iostream> 
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <map>
#include <set>
#include <math.h>
#include <stack>
#include <deque>
#include <numeric>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <regex>

#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;

int moves[8][2] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
        { 1, -1 }, { 0, -1 }, { -1, -1 } };

vector<string> do_step(const vector<string>& field) {
    vector<string> res(field.size(), string(field[0].size(), '.'));
    for (int i = 0; i < (int)field.size(); ++i) {
        for (int j = 0; j < (int)field[i].size(); ++j) {
            int o = 0, w = 0, ly = 0;
            for (int l = 0; l < 8; ++l) {
                int tx = i + moves[l][0];
                int ty = j + moves[l][1];
                if (tx < 0 || tx >= field.size() || ty < 0 || ty >= field[0].size()) {
                    continue;
                }
                switch(field[tx][ty]) {
                    case '.': o++; break;
                    case '|': w++; break;
                    case '#': ly++; break;
                    default: cout << "Uknown cell " << field[tx][ty] << endl; exit(1);
                }
            }
            

            if (field[i][j] == '.') {
                res[i][j] = (w >= 3 ? '|' : '.');
            } else if (field[i][j] == '|') {
                res[i][j] = (ly >= 3 ? '#' : '|');
            } else {
                res[i][j] = (ly >= 1 && w >= 1 ? '#' : '.');
            }
        }
    }
    return res;
}
int get_resource_value(const vector<string>& field) {
    int wood = 0, lumberyards = 0;
    for (auto l : field) {
        for (auto c : l) {
            wood += (c == '|');
            lumberyards += (c == '#');
        }
    }
    return wood * lumberyards;
} 
int main() {
    vector<string> field;
    string s;
    while (getline(cin, s)) {
        field.push_back(s);
    }

    for (int step = 0; step < 10; ++step) {
        field = do_step(field);
    }
    cout << "Part 1 " << get_resource_value(field) << endl;

    unordered_map<string, int> visited;

    int target = 1000000000;
    for (int step = 11; step < 1000; ++step) {
        field = do_step(field);
        string flattened;
        for (auto l : field) {
            flattened += l;
        }
        auto it = visited.find(flattened);
        if (it == visited.end()) {
            visited.insert({flattened, step});
            continue;
        }
        int cycle = step - it->second;
        target = (target - it->second) % cycle;
        break;
    }
    for (int step = 0; step < target; ++step) {
        field = do_step(field);
    }
    cout << "Part 2 " << get_resource_value(field) << endl;
    return 0;
}