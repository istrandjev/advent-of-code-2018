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

char complement(char c) {
    if (isupper(c)) {
        return tolower(c);
    } else {
        return toupper(c);
    }
}

int solve(string s) {
    string temp;
    while (true) {
        temp.clear();
        for (int i = 0; i < (int)s.size(); ++i) {
            if (temp.size() == 0 || s[i] != complement(temp.back())) {
                temp.push_back(s[i]);
            } else {
                temp.pop_back();
            }
        }
        if (temp.size() == s.size()) {
            break;
        }
        s = temp;
    }
    return temp.size();
}
string remove_char(char char_to_remove, const string& s) {
    string res;
    char upcase = toupper(char_to_remove), lowcase = tolower(char_to_remove);
    for (auto x : s) {
        if (x != upcase && x != lowcase) {
            res.push_back(x);
        }
    }
    return res;
}
int main() {
    string s;
    getline(cin, s);

    cout << "Part 1 " << solve(s) << endl;
    int best = solve(remove_char('a', s));
    for (char c = 'b'; c <= 'z'; ++c) {
        best = min(best, solve(remove_char(c, s)));
    }
    cout << "Part 2 " << best << endl;
    return 0;
}