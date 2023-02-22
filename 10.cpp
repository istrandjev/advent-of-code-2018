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
    int x, y;
    int vx, vy;
    pair<int, int> get(int second) {
        return {x + vx * second, y + vy * second};
    }
};

void print_screen(const vector<point>& points, int second) {
    vector<pair<int, int> > pixels(points.size());
        
    for (auto p : points) {
        pixels.push_back(p.get(second));
    }
    int minx, maxx, miny, maxy;
    minx = maxx = pixels[0].first;
    miny = maxy = pixels[0].second;
    for (auto px : pixels) {
        minx = min(minx, px.first);
        miny = min(miny, px.second);
        maxx = max(maxx, px.first);
        maxy = max(maxy, px.second);
    }

    vector<string> screen(maxy - miny + 1, string(maxx - minx + 1, ' '));
    for (auto px : pixels) {
        screen[px.second - miny][px.first - minx] = '#';
    }
    for (int i = 0; i <= maxy - miny; ++i) {
        for (int j = 0; j <= maxx - minx; ++j) {
            cout << screen[i][j];
        }
        cout << endl;
    }

}
int main() {
    vector<point> points;
    
    string s;
    while (getline(cin, s)) {
        point temp;
        sscanf(s.c_str(), "position=<%d, %d> velocity=< %d, %d>", &temp.x, &temp.y, &temp.vx, &temp.vy);
        points.push_back(temp);
    }
    
    ll min_area = 1000000LL * 1000000LL;
    int bests = 0;
    for (int s = 1; s <= 50000; ++s) {
        vector<pair<int, int> > pixels(points.size());
        
        for (auto p : points) {
            pixels.push_back(p.get(s));
        }
        int minx, maxx, miny, maxy;
        minx = maxx = pixels[0].first;
        miny = maxy = pixels[0].second;
        for (auto px : pixels) {
            minx = min(minx, px.first);
            miny = min(miny, px.second);
            maxx = max(maxx, px.first);
            maxy = max(maxy, px.second);
        }

        ll area = maxx - minx + 1;
        area *= maxy - miny + 1;
        if (area < min_area) {
            min_area = area;
            bests = s;
        }   
    }
    cout << "Part 1 " << endl;
    print_screen(points, bests);
    cout << "Part 2 " << bests << endl;
    return 0;
}