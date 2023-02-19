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
    regex instruction("Step ([A-Z]) must be finished before step ([A-Z]) can begin.");    
    
    vector<int> deg(26);
    vector<vector<int> > ne(26);
    string s;
    smatch match;
    vector<char> met(26, 0);
    while (getline(cin, s)) {
        if (regex_match(s, match, instruction)) {
            int a = match.str(1)[0] - 'A';
            int b = match.str(2)[0] - 'A';
            ne[a].push_back(b);
            met[a] = 1;
            met[b] = 1; 
            deg[b]++;
        }
    }
    vector<int> original_degrees = deg;

    priority_queue<int> pq;
    for (int i = 0; i < 26; ++i) {
        if (deg[i] == 0 && met[i]) {
            pq.push(-i);
        }
    }
    string res1;
    while (!pq.empty()) {
        int c = -pq.top();
        pq.pop();
        res1.push_back(c + 'A');
        for (auto nxt : ne[c]) {
            deg[nxt]--;
            if (deg[nxt] == 0) {
                pq.push(-nxt);
            }
        }
    }
    cout << "Part 1 " << res1 << endl;
    deg = original_degrees;
    int num_workers = 5;
    int penalty = 60;
    vector<pair<int, int> > workers(num_workers, {0, 0});
    
    for (int i = 0; i < 26; ++i) {
        if (deg[i] == 0 && met[i]) {
            pq.push(-i);
        }
    }
    int steps2 = 0;
    int processed = 0;
    int total = 0;
    for (auto m  :met) {
        total += (int)m;
    }
    for (steps2 = 0; processed < total;steps2++) {
        vector<int> new_processed;
        for (int wi = 0; wi < (int)workers.size(); ++wi) {
            if (workers[wi].first == 0) {
                if (!pq.empty()) {
                    workers[wi] = {-pq.top() + penalty, -pq.top()};
                    if (workers[wi].first == 0) {
                        new_processed.push_back(workers[wi].second);
                    }
                    pq.pop();
                    
                }
            } else if (workers[wi].first == 1) {
                new_processed.push_back(workers[wi].second);
                workers[wi].first--;
            } else {
                workers[wi].first--;
            }
            
        }
        processed += (int)new_processed.size();
        for (auto np : new_processed) {
            for (auto nxt : ne[np]) {
                deg[nxt]--;
                if (deg[nxt] == 0) {
                    pq.push(-nxt);
                }
            } 
        }
    }
    cout << "Part 2 " << steps2 << endl;
    return 0;
}