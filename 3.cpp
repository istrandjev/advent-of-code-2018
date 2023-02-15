#include <iostream>
#include <string>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <regex>
#define all(x) x.begin(), x.end()
using namespace std;
typedef long long ll;

struct rect {
    int id;
    int x0, y0;
    int w, h;
};
int main() {
    regex claim("#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)");
    string s;
    vector<vector<int> > fabric(1024, vector<int>(1024));
    smatch match;
    vector<rect> rects;  
    while (getline(cin, s)) {
        if (regex_match(s, match, claim)) {
            int x0 = stoi(match.str(2));
            int y0 = stoi(match.str(3));
            int w = stoi(match.str(4));
            int h = stoi(match.str(5));
            int id_int = stoi(match.str(1));
            rects.push_back(rect{id_int, x0, y0, w, h});
            for (int ix = x0; ix < x0 + w; ++ix) {
                for (int iy =  y0; iy < y0 + h; ++iy) {
                    fabric[ix][iy]++;
                }
            }
        }
    }
    int res1 = 0;
    for (auto row :  fabric) {
        for (auto cell : row) {
            if (cell >= 2) {
                res1++;
            }
        }
    }
    cout << "Part 1 " << res1 << endl;
    for (auto r : rects) {
        bool uniq = true;
        for (int ix = r.x0; ix < r.x0 + r.w; ++ix) {
            for (int iy =  r.y0; iy < r.y0 + r.h; ++iy) {
                if (fabric[ix][iy] > 1) {
                    uniq = false;
                }
            }
        }
        if (uniq) {
            cout << "Part 2 " << r.id << endl;
        }
    }
    return 0;
}