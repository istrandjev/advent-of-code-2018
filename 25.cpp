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

struct point {
    ll x, y, z, t;
    ll dist(const point& p) const {
        return abs(x - p.x) + abs(y - p.y) + abs(z - p.z) + abs(t - p.t);
    }
};

int main() {
    point temp;
    vector<point> points;
    while (scanf("%lld,%lld,%lld,%lld",&temp.x, &temp.y, &temp.z, &temp.t) != EOF) {
        points.push_back(temp);
    }

    vector<vector<int> > ne(points.size());
    for (int i = 0; i < (int)points.size(); ++i) {
        for (int j = i + 1; j < (int)points.size(); ++j) {
            if (points[i].dist(points[j]) <= 3) {
                ne[i].push_back(j);
                ne[j].push_back(i);
            }
        }
    }
    vector<char> vis(ne.size());
    int res1 = 0;
    for (int i = 0; i < (int)ne.size(); ++i) {
        if (vis[i]) {
            continue;
        }
        res1++;
        vis[i] = 1;
        queue<int> q;
        q.push(i);
        while (!q.empty()) {
            int c = q.front();
            q.pop();
            for (auto nxt : ne[c]) {
                if (vis[nxt]) {
                    continue;
                }
                vis[nxt] = 1;
                q.push(nxt);
            }
        }
    }
    cout << "Part 1 " << res1 << endl;
    return 0;
}