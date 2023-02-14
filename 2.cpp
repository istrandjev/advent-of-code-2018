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

int cnt[128];
int main() {
    string s;
    int twos = 0, threes = 0;
    vector<string> boxes; 
    while (getline(cin, s)) {
        memset(cnt, 0, sizeof(cnt));
        boxes.push_back(s);
        for (auto c: s) {
            cnt[c]++;
        }
        bool has_two = false, has_three = false;
        for (int i = 'a'; i <= 'z'; ++i) {
            if (cnt[i] == 2) {
                has_two = true;
            }
            if (cnt[i] == 3) {
                has_three = true;
            }
        }
        twos += has_two;
        threes += has_three;
    }
    cout << "Part 1 " << twos * threes << endl;
    bool found = false;
    for (int i = 0; i < (int)boxes.size() && !found; ++i) {
        for (int j = i + 1; j < (int)boxes.size() && !found; ++j) {
            int diff = 0;
            if (boxes[i].size() != boxes[j].size()) {
                continue;
            }
            for (int l = 0; l < (int)boxes[i].size(); ++l) {
                diff += (boxes[i][l] != boxes[j][l]);
            }

            if (diff == 1) {
                cout << "Part 2 ";
                for (int l = 0; l < (int)boxes[i].size(); ++l) {
                    if (boxes[i][l] == boxes[j][l]) {
                        cout << boxes[i][l];
                    }
                }
                cout << endl;
                found = true;
            }
        }
    }
    return 0;
}