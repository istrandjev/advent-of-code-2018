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

vector<string> field(2000, string(1000, ' '));
bool dfs(int x, int y) {
    if (field[y][x] != ' ') {
        return field[y][x] == 'X';
    }
    field[y][x] = '@';
    if (y + 1 == field.size()) {
        field[y][x] = 'X';
        return true;
    }
    bool inf = false;
    if (field[y + 1][x] != '#') {
        inf = inf || dfs(x, y + 1);
    }

    if (inf) {
        field[y][x] = 'X';
        return true;
    }
    if (field[y][x - 1] != '#') {
        inf = dfs(x - 1, y) || inf;
    }
    if (field[y][x + 1] != '#') {
        inf = dfs(x + 1, y) || inf;
    }
    if (inf) {
        field[y][x] = 'X';
    }
    if (inf) {
        for (int i = x - 1; i > 0 && field[y][i] == '@' || field[y][i] == 'X'; --i) {
            field[y][i] = 'X';
        }
        for (int i = x + 1; i < 1000 && field[y][i] == '@' || field[y][i] == 'X'; ++i) {
            field[y][i] = 'X';
        }
    }
    return inf;
}
int main() {
    regex bar("(x|y)=(-?\\d+), (x|y)=(-?\\d+)..(-?\\d+)");
    
    string s;
    smatch match;
    int minx = 100000, miny = 100000;
    int maxx = -100000, maxy = -100000;
    
    
    while (getline(cin, s)) {
        if (!regex_match(s, match, bar)) {
            cout << "Could not parse line " << s << endl;
            exit(1);
        }
        int a = stoi(match.str(2)), b = stoi(match.str(4)), c = stoi(match.str(5));
        if (match.str(1) == "x") {
            minx = min(minx, a);
            maxx = max(maxx, a);
            miny = min(miny, b);
            maxy = max(maxy, c);
            for (int y = b; y <= c; ++y) {
                field[y][a] = '#';
            }
        } else {
            miny = min(miny, a);
            maxy = max(maxy, a);
            minx = min(minx, b);
            maxx = max(maxx, c);
            for (int x = b; x <= c; ++x) {
                field[a][x] = '#';
            }
        }
    }
    while (field.size() > maxy + 1) {
        field.pop_back();
    }
    dfs(500, 0);
    
    int res1 = 0;
    cout << miny << endl;
    int res2 = 0;
    for (int i = miny; i <= maxy; ++i) {
        for (auto c : field[i]) {
            res1 += (c == '@' || c == 'X');
            res2 += (c == '@');
        }
    }
    cout << "Part 1 " << res1 << endl;
    cout << "Part 2 " << res2 << endl;

    return 0;
}