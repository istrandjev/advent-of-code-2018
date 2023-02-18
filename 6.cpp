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

int main() {
    vector<pair<int, int> > a;

    pair<int, int> temp;
    int offset = 500;
    while (scanf("%d, %d", &temp.first, &temp.second) != EOF) {
        temp.first += offset;
        temp.second += offset;
        a.push_back(temp);
    }
    vector<vector<int> > closest(1000, vector<int>(1000, -1));

    for (int i = 0; i < (int)closest.size(); ++i) {
        for (int j = 0; j < (int)closest[i].size(); ++j) {
            int best = 0;
            int bestd = abs(i - a[0].first) + abs(j - a[0].second);
            int best_cnt = 1;
            for (int l = 1; l < (int)a.size(); ++l) {
                int td = abs(i - a[l].first) + abs(j - a[l].second);
                if (td == bestd) {
                    best_cnt++;
                } else if (td < bestd) {
                    best_cnt = 1;
                    bestd = td;
                    best = l;
                }
            }
            if (best_cnt == 1) {
                closest[i][j] = best;
            }
        }
    }
    vector<char> inf(a.size(), 0);
    for (int i = 0; i < (int)closest.size(); ++i) {
        if (closest[i][0] != -1) {
            inf[closest[i][0]] = 1;
        }
        if (closest[i].back() != -1) {
            inf[closest[i].back()] = 1;
        }
    }
    for (int i = 0; i < (int)closest[0].size(); ++i) {
        if (closest[0][i] != -1) {
            inf[closest[0][i]] = 1;
        }
        if (closest.back()[i] != -1) {
            inf[closest.back()[i]] = 1;
        }
    }
    vector<int> cnt(a.size(), 0);
    for (int i = 0; i < (int)closest.size(); ++i) {
        for (int j = 0; j < (int)closest[i].size(); ++j) {
            int c = closest[i][j];
            if (c != - 1) {
                cnt[c]++;
            }
        }
    }
    int best_finite = -1;
    for (int i = 0; i < (int)a.size(); ++i) {
        if (inf[i]) {
            continue;
        }
        best_finite = max(best_finite, cnt[i]);
    }
    cout << "Part 1 " << best_finite << endl;
    int res2 = 0;
    for (int i = 0; i < (int)closest.size(); ++i) {
        for (int j = 0; j < (int)closest[i].size(); ++j) {
            int total = 0;
            for (auto t : a) {
                total += abs(t.first - i) + abs(t.second - j);
            }
            if (total < 10000) {
                res2++;
            }
        }
    }
    cout << "Part 2 " << res2 << endl;
    return 0;
}