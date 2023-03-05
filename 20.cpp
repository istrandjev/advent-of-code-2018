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

vector<int> closing;
vector<string> field;
vector<vector<int> > or_by_level;
string s;
int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
int char_to_dir[128];

unordered_set<ll> mem;

ll code(int x, int y, int from, int to) {
    ll res = from;
    res *= 15000;
    res += to;
    res *= 8000;
    res += x;
    res *= 8000;
    res += y;
    return res;
}

vector<pair<int, int> > dfs(int x, int y, int from, int to, int level) {
    ll kod = code(x, y, from, to);
    auto it = mem.find(kod);
    if (it != mem.end()) {
        return {};
    }
    mem.insert(kod);
    if (s[from] == '(' && closing[from] == to) {
        auto res = dfs(x, y, from + 1, to - 1, level + 1);;
        return res;
    }
    auto next_or = upper_bound(all(or_by_level[level]), from);
    if (next_or != or_by_level[level].end() && *next_or <= to ) {
        vector<pair<int, int> > res = dfs(x, y, from, *next_or - 1, level);
        
        auto tmp = dfs(x, y, *next_or + 1, to, level);
        res.insert(res.end(), all(tmp));
        return res;
    } 

    int ox = x;
    int oy = y;
    while (from <= to && isalpha(s[from])) {
        int l = char_to_dir[s[from]];
        field[x + moves[l][0]][y + moves[l][1]] = ((x + moves[l][0]) % 2 == 0 ? '|' : '-');
        x += moves[l][0] * 2;
        y += moves[l][1] * 2;
        from++;
    }
    if (from > to) {
        return {{x, y}};
    } else if (s[from] == '(') {
        vector<pair<int, int> > step = dfs(x, y, from + 1, closing[from] - 1, level + 1);
        if (closing[from] != to) {
            vector<pair<int, int> > result; 
            for (auto p : step) {
                vector<pair<int, int> > tmp = dfs(p.first, p.second, closing[from] + 1, to, level);
                result.insert(result.end(), tmp.begin(), tmp.end());
            }
            return result;
        } else {
            return step;
        }
    }
    return {};
}
int main() {
    field.clear();
    field.resize(8001, string(8001, '.'));
    for (int i = 1; i < 8001; i += 2) {
        for (int j = 1; j < 8001; j ++) {
            field[i][j] = '#';
        }
    }
    for (int i = 1; i < 8001; i ++) {
        for (int j = 1; j < 8001; j += 2) {
            field[i][j] = '#';
        }
    }
    
    getline(cin, s);
    s = s.substr(1, (int)s.size() - 2);
    vector<int> opened;
    closing.clear();
    closing.resize(s.size(), -1);
    or_by_level.clear();
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '(') {
            opened.push_back(i);
        }
        if (s[i] == ')') {
            closing[opened.back()] = i;
            opened.pop_back();
        }
        if (s[i] == '|') {
            while (or_by_level.size() < opened.size() + 1) {
                or_by_level.push_back({});
            }
            or_by_level[opened.size()].push_back(i);
        }
    }
    char_to_dir['N'] = 0;
    char_to_dir['E'] = 1;
    char_to_dir['S'] = 2;
    char_to_dir['W'] = 3;

    dfs(4000, 4000, 0, (int)s.size() - 1, 0);
    queue<pair<int, int> > q;
    vector<vector<int> > vis(8048, vector<int>(8048, -1));
    q.push({4000, 4000});
    vis[4000][4000] = 0;
    int minx = 4000, maxx = 4000, miny = 4000, maxy = 4000;
    int res1 = 0;
    while (!q.empty()) {
        auto c = q.front();
        q.pop();
        for (int l = 0; l < 4; ++l) {
            if (field[c.first + moves[l][0]][c.second + moves[l][1]] == '#') {
                continue;
            }
            pair<int, int> nxt(c.first + moves[l][0] * 2, c.second + moves[l][1] * 2);
            if (vis[nxt.first][nxt.second] != -1) {
                continue;
            }
            vis[nxt.first][nxt.second] = vis[c.first][c.second] + 1;
            res1 = max(res1, vis[nxt.first][nxt.second]);
            q.push(nxt);
        }
    }
    cout << "Part 1 " << res1 << endl;
    int res2 = 0;
    for (int i = 0; i < (int)vis.size(); ++i) {
        for (int j = 0; j < (int)vis.size(); ++j) {
            res2 += (vis[i][j] >= 1000);
        }
    }
    cout << "Part 2 " << res2 << endl;
    return 0;
}