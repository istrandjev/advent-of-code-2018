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


int moves[4][2] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };

map<pair<int, int>, int> creatures;
vector<string> a;
int total_creatures[128];
int n, m;
char other(char c) {
    if (c == 'E' ){
        return 'G';
    }
    if (c == 'G') {
        return 'E';
    }
    exit(13);
}

void do_attack(int x, int y, int attack_power=3) {
    char eye = a[x][y];
    char they = other(a[x][y]);
    int ox = -1, oy = -1;
    for (int l = 0; l < 4; ++l) {
        int tx = x + moves[l][0];
        int ty = y + moves[l][1];
        if (tx < 0 || tx >= n || ty < 0 || ty >= m || a[tx][ty] != they) {
            continue;
        }
        if (ox == -1) {
            ox = tx;
            oy = ty;
        } else {
            int ch = creatures[{tx, ty}];
            int bh = creatures[{ox, oy}]; 
            if (ch < bh) {
                ox = tx;
                oy = ty;
            }
        }
    }
    if (ox < 0) {
        return;
    }
    auto it = creatures.find({ox, oy});
    if (it->second <= attack_power) {
        creatures.erase(it);
        total_creatures[a[ox][oy]]--;
        a[ox][oy] = '.';
    } else {
        it->second -= attack_power;
    }
}
pair<int, int> do_move(int x, int y) {
    char eye = a[x][y];
    char they = other(a[x][y]);
    for (int l = 0; l < 4; ++l) {
        int tx = x + moves[l][0];
        int ty = y + moves[l][1];
        if (tx < 0 || tx >= n || ty < 0 || ty >= m || a[tx][ty] != they) {
            continue;
        }
        return {x, y};
    }
    
    vector<vector<int> > distances(n, vector<int>(m, -1));
    queue<pair<int, int> > q;
    q.push({x, y});
    while (!q.empty()) {
        auto c = q.front();
        q.pop();
        for (int l = 0; l < 4; ++l) {
            int tx = c.first + moves[l][0];
            int ty = c.second + moves[l][1];
            if (tx < 0 || tx >= n || ty < 0 || ty >= m || distances[tx][ty] != -1 || a[tx][ty] != '.') {
                continue;
            }
            distances[tx][ty] = distances[c.first][c.second] + 1;
            q.push({tx, ty});
        }
    }

    int targetx = -1, targety;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (distances[i][j] == -1 || a[i][j] != '.') {
                continue;
            }
            bool adjacent = false;
            for (int l = 0; l < 4; ++l) {
                int tx = i + moves[l][0];
                int ty = j + moves[l][1];
                if (tx < 0 || tx >= n || ty < 0 || ty >= m || a[tx][ty] != they) {
                    continue;
                }
                adjacent = true;
                break;
            }
            if (!adjacent) {
                continue;
            }
            if (targetx == -1 || distances[targetx][targety] > distances[i][j]) {
                targetx = i;
                targety = j;
            }
        }
    }
    if (targetx == -1) {
        return {x, y};
    }
    q.push({targetx, targety});
    for (int i = 0; i < n; ++i) {
        fill(all(distances[i]), -1);
    }
    distances[targetx][targety] = 0;

    while (!q.empty()) {
        auto c = q.front();
        q.pop();
        for (int l = 0; l < 4; ++l) {
            int tx = c.first + moves[l][0];
            int ty = c.second + moves[l][1];
            if (tx < 0 || tx >= n || ty < 0 || ty >= m || distances[tx][ty] != -1 || a[tx][ty] != '.') {
                continue;
            }
            distances[tx][ty] = distances[c.first][c.second] + 1;
            q.push({tx, ty});
        }
    }
    int sx = -1, sy = -1;
    for (int l = 0; l < 4; ++l) {
        int tx = x + moves[l][0];
        int ty = y + moves[l][1];
        if (tx < 0 || tx >= n || ty < 0 || ty >= m || a[tx][ty] != '.' || distances[tx][ty] == -1) {
            continue;
        }
        if (sx == -1 || distances[tx][ty] < distances[sx][sy]) {
            sx = tx;
            sy = ty;
        }
    }
    if (sx != -1) {
        int h = creatures[{x, y}];
        a[sx][sy] = a[x][y];
        a[x][y] = '.';
        creatures.erase({x, y});
        creatures.insert({{sx, sy}, h});
        return {sx, sy};
    }
    return {x, y};
}

int solve(const vector<string>& b, int attack_power=3) {
    a = b;
    string s;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    creatures.clear();
    n = a.size();
    m = a[0].size();
    memset(total_creatures, 0, sizeof(total_creatures));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == 'E'|| a[i][j] == 'G') {
                total_creatures[a[i][j]]++;
                creatures.insert({{i, j}, 200});
            }
        }
    }   
    int rounds = 0;
    for (rounds = 0; ; rounds++) {
        bool has_g = total_creatures['G'] != 0, has_e = total_creatures['E'] != 0;
        if (!has_e || !has_g) {
            break;
        }
        vector<vector<char> > moved(n, vector<char>(m, 0));
        bool has_ended = false;
        for (int i = 0; i < n && !has_ended; ++i) {
            for (int j = 0; j < m && !has_ended; ++j) {
                if (moved[i][j]) {
                    continue;
                }
                if (a[i][j] == 'G' || a[i][j] == 'E') {
                    int targets = total_creatures[other(a[i][j])];
                    if (targets == 0) {
                        has_ended = true;
                        break;
                    }
                    int attack_strength = (a[i][j] == 'E' ? attack_power : 3);
                    auto new_pos = do_move(i, j);
                    moved[new_pos.first][new_pos.second] = 1;
                    do_attack(new_pos.first, new_pos.second, attack_strength);
                }
            }
        }
        if (has_ended) {
            break;
        }
    }
    return rounds;
}

int outcome(int rounds) {
    ll total = 0;
    for (auto it : creatures) {
        total += it.second;
    }
    return (ll)rounds * total;
}
int main() {
    vector<string> b;
    string s;
    while (getline(cin, s)) {
        b.push_back(s);
    }

    int rounds = solve(b);
    cout << "Part 1 " << outcome(rounds) << endl;

    int number_of_elfs = 0;
    for (auto l : b) {
        for (auto c : l) {
            number_of_elfs += (c == 'E');
        }
    }
    int beg = 1, end = 201;
    while (end - beg > 1) {
        int mid = (end + beg) / 2;
        solve(b, mid);
        if (total_creatures['E'] == number_of_elfs) {
            end = mid;
        } else {
            beg = mid;
        }
    }
    rounds = solve(b, end);
    cout << "Part 2 " << outcome(rounds) << endl;
    return 0;
}