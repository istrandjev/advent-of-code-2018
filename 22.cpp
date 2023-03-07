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

const ll depth = 11820;
pair<int, int> target(7, 782);

// const ll depth = 510;
// pair<int, int> target(10, 10);

ll get_erosion_level(ll geo_index) {
    return (geo_index + (ll)depth) % 20183LL;
}

char get_char(ll erosion_level) {
    return ".=|"[erosion_level % 3];
}

int get_value(char region) {
    switch(region) {
        case '.': return 0;
        case '=': return 1;
        case '|': return 2;
        default: cout << "Unknown region type " << region << endl; exit(1);
    }
}

struct state {
    int x, y;
    int equipment;
};

const int factor = 2000;
int encode(const state& s) {
    return s.equipment * factor * factor + s.y * factor + s.x;
}

state decode(int code) {
    state res;
    res.x = code % factor;
    code /= factor;
    res.y = code % factor;
    res.equipment = code / factor;
    return res;
}

vector<string> get_field() {
    int n = target.second + 1;
    int m = target.first + 1;
    vector<vector<ll> > erosion_levels(factor , vector<ll>(factor, 0));
    for (int i = 0; i < erosion_levels.size(); ++i) {
        for (int j = 0; j < erosion_levels[0].size(); ++j) {
            if (i == j && j == 0) {
                erosion_levels[i][j] = get_erosion_level(0);
            } else if (j == 0) {
                erosion_levels[i][j] = get_erosion_level(i * 48271);
            } else if (i == 0) {
                erosion_levels[i][j] = get_erosion_level(j * 16807);
            } else {
                erosion_levels[i][j] = get_erosion_level(erosion_levels[i-1][j] * erosion_levels[i][j-1]);
            }
        }
    }
    erosion_levels[n - 1][m - 1] = get_erosion_level(0);
    vector<string> result(erosion_levels.size(), string(erosion_levels[0].size(), ' '));
    for (int i = 0; i < result.size(); ++i) {
        for (int j = 0; j < result[0].size(); ++j) {
            result[i][j] = get_char(erosion_levels[i][j]);
        }
    }
    return result;
}


int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

bool is_valid_equipment(char region, int equipment) {
    if (equipment < 0 || equipment >= 3) {
        return false;
    }
    switch (region) {
        case '.': return equipment != 0;
        case '=': return equipment != 1;
        case '|': return equipment != 2;
    }
    return false;
}
int main() {
    vector<string> field = get_field();
    int res1 = 0;
    for (int i = 0; i < target.second + 1; ++i) {
        for (int j = 0; j < target.first + 1; ++j) {
            res1 += get_value(field[i][j]);
        }
    }
    cout << "Part 1 " << res1 << endl;
    priority_queue<pair<int, int> > pq;
    int initial_state_code = encode(state{0, 0, 1});
    pq.push({0, initial_state_code});
    unordered_map<int, int> vis;
    int final_code = encode(state{target.first, target.second, 1});
    while (!pq.empty()) {
        int d = -pq.top().first;
        int c_code = pq.top().second;
        
        pq.pop();
        if (vis.count(c_code)) {
            continue;
        }
        
        vis[c_code] = d;
        if (c_code == final_code) {
            break;
        }
        state c = decode(c_code);
        for (int eq = 0; eq < 3; ++eq) {
            if (eq == c.equipment || !is_valid_equipment(field[c.y][c.x], eq)) {
                continue;
            }
            state temp = c;
            temp.equipment = eq;
            int temp_code = encode(temp);
            if (vis.count(temp_code)) {
                continue;
            }
            pq.push({-(d + 7), temp_code});
        }
        for (int l = 0; l < 4; ++l) {
            int tx = c.x + moves[l][0];
            int ty = c.y + moves[l][1];
            if (tx < 0 || ty < 0 || tx >= field[0].size() || ty >= field.size()) {
                continue;
            }
            if (!is_valid_equipment(field[ty][tx], c.equipment)) {
                continue;
            }
            state temp{tx, ty, c.equipment};
            int temp_code = encode(temp);
            if (vis.count(temp_code)) {
                continue;
            }
            pq.push({-(d + 1), temp_code});
        }
    }
    cout << "Part 2 " << vis[final_code] << endl;
    return 0;
}