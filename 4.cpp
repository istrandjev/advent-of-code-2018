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


int main() {
    vector<string> entries;
    string s;
    while (getline(cin, s)) {
        entries.push_back(s);
    }
    unordered_map<int, vector<int> > guards;
    sort(all(entries));
    int current_guard = -1;
    string timestamp_re("\\[\\d{4}-\\d{2}-\\d{2} \\d{2}:(\\d{2})\\] ");
    regex new_guard_re(timestamp_re + "Guard #(\\d+) begins shift");
    regex falls_asleep_re(timestamp_re + "falls asleep");
    regex wakes_up_re(timestamp_re + "wakes up");
    smatch match;
    int last_asleep = -1;
    for (auto entry : entries) {
        if (regex_match(entry, match, new_guard_re)) {
            current_guard = stoi(match.str(2));
        } else if (regex_match(entry, match, falls_asleep_re)) {
            last_asleep = stoi(match.str(1));
        } else if (regex_match(entry, match, wakes_up_re)) {
            int wakes_up = stoi(match.str(1));
            auto it = guards.find(current_guard);
            if (it == guards.end()) {
                it = guards.insert({current_guard, vector<int>(61, 0)}).first;
            }
            for (int i = last_asleep; i < wakes_up; ++i) {
                it->second[i]++;
            }
        }
    }
    int best = -1;
    int best_minutes = 0;
    int minute;
    for (auto it : guards) {
        int total = accumulate(all(it.second), 0);
        if (total > best_minutes) {
            best = it.first;
            best_minutes = total;
            minute = -1;
            int most_slept = 0;
            for (int t = 0; t < (int)it.second.size(); ++t) {
                if (it.second[t] > most_slept) {
                    most_slept = it.second[t];
                    minute = t;
                }
            }
        }
    }
    cout << "Part 1 " << best * minute << endl;  
    int most_minute = 0;
    int chosen_minute;
    int most_guard;
    for (auto it : guards) {
        for (int t = 0; t < (int)it.second.size(); ++t) {
            if (it.second[t] > most_minute) {
                most_minute = it.second[t];
                chosen_minute = t;
                most_guard = it.first;
            }
        }
    }
    cout << "Part 2 " << chosen_minute * most_guard << endl; 
    return 0;
}