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

int serial_number = 9445;

int get_power_level(int x, int y) {
    int rack_id = x + 10;
    int res = rack_id * y + serial_number;
    res *= rack_id;
    int value = (res % 1000 - res % 100) / 100;
    return value - 5;
}

int get(const vector<vector<int> >& a, int x0, int y0, int w, int h) {
    return a[y0 + h][x0 + w] - a[y0][x0 + w] - a[y0 + h][x0] + a[y0][x0];
}

int main() {
    vector<vector<int> > a(301, vector<int>(301));
    for (int x = 1; x <= 300; ++x) {
        for (int y = 1; y <= 300; ++y) {
            a[x][y] = get_power_level(x, y);
        }
    }
    for (int i = 1; i < (int)a.size(); ++i) {
        for (int j = 1; j < (int)a[i].size(); ++j) {
            a[i][j] += a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1]; 
        }
    }
    
    int best = 0, bx, by;
    for (int x = 1; x <= 300 - 2; ++x) {
        for (int y = 1; y <= 300 - 2; ++y) {
            int score = get(a, x - 1, y - 1, 3, 3);
            if (score > best) {
                best = score;
                bx = x;
                by = y;
            }
        }
    }
    printf("Part 1: %d,%d\n", by, bx);
    best = 0;
    int bestw;
    for (int w = 1; w <= 300; ++w) {
        for (int x = 1; x <= 300 - w + 1; ++x) {
            for (int y = 1; y <= 300 - w + 1; ++y) {
                int score = get(a, x - 1, y - 1, w, w);
                if (score > best) {
                    best = score;
                    bx = x;
                    by = y;
                    bestw = w;
                }
            }
        }
    }
    printf("Part 2: %d,%d,%d\n", by, bx, bestw);
    return 0;

}