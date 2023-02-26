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

ll get_number(const vector<int>& sequence, int start, int len) {
    ll res = 0;
    for (int i = start; i < start + len; ++i) {
        res *= 10;
        res += sequence[i];
    }
    return res;
}
int main() {
    const int target = 540391;
    int target_len = 0;
    for (int temp = target; temp > 0; target_len++, temp /= 10);
    vector<int> sequence{3,7};
    int c0 = 0, c1 = 1;
    int res2 = -1;
    auto check_target = [&]() {
        if (sequence.size() >= target_len && get_number(sequence, (int)sequence.size() - target_len, target_len) == target && res2 == -1) {
            res2 = (int)sequence.size() - target_len;
        } 
    };
    while (sequence.size() < target + 10 || res2 == -1) {
        int s = sequence[c0] + sequence[c1];
        if ( s < 10) {
            sequence.push_back(s);
            check_target();
        } else {
            sequence.push_back(s / 10);
            check_target();
            sequence.push_back(s % 10);
            check_target();
        }
        c0 = (c0 + sequence[c0] + 1) % sequence.size();
        c1 = (c1 + sequence[c1] + 1) % sequence.size();
    }
    ll res1 = get_number(sequence, target, 10);
    cout << "Part 1 "  << res1 << endl;
    cout << "Part 2 " << res2 << endl;
    return 0;
}