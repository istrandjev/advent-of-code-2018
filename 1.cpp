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
    int frequency = 0;
    int delta;
    vector<int> deltas;
    while (scanf("%d", &delta) != EOF) {
        frequency += delta;
        deltas.push_back(delta);
    }
    cout << "Part 1 " << frequency << endl;
    frequency = 0;
    unordered_set<int> met;
    bool repeated = false;
    int repeated_value;
    while (!repeated) {
        for (auto d : deltas) {
            frequency += d;
            if (met.count(frequency)) {
                repeated = true;
                repeated_value = frequency;
                break;
            }
            met.insert(frequency);
        } 
    }
    cout << "Part 2 " << repeated_value << endl; 
    return 0;
}