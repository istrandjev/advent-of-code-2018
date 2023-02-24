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

string do_step(const string& field, const unordered_map<string, char>& rules) {
    string res = field;
    for (int i = 2; i <= (int)field.size() - 2; ++i) {
        string sstr = field.substr(i - 2, 5);
        auto it = rules.find(sstr);
        if (it != rules.end()) {
            res[i] = it->second;
        }
    }
    return res;
}

int main() {
    regex is_re("initial state: ([#.]+)");
    regex rule_re("([#.]+) => ([#.])");
    smatch match;
    string s;
    getline(cin, s);
    if (!regex_match(s, match, is_re)) {
        cout << "Could not parse the initial state " << s << endl;
        return 1;
    }
    string initial_state = match.str(1);
    getline(cin, s);
    unordered_map<string, char> rules;
    while (getline(cin, s)) {
        if (!regex_match(s, match, rule_re)) {
            cout << "Could not parse a rule from " << s << endl;
            return 1;
        }
        rules[match.str(1)] = match.str(2)[0];
    }
    const int offset = 10000;
    string field(offset + initial_state.size() + offset, '.');
    for (int i = 0; i < (int)initial_state.size(); ++i) {
        field[offset + i] = initial_state[i];
    }
    for (int step = 0; step < 1001; ++step) {
        field = do_step(field, rules);
        int answer = 0;
        int total = 0;
        for (int i = 0; i < (int)field.size(); ++i) {
            if (field[i] == '#') {
                answer += i - offset;
                total++;
            }
        }
        if (step == 19) {
            cout << "Part 1 " << answer << endl;
        }
        if (step == 999) {
            cout << "Part 2 "<< (ll)answer + (50000000000LL - 1000LL)*(ll)total << endl;
            break;
        }
    }
     
    return 0;
}