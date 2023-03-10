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
    ll x, y, z;
    ll dist(const point& p) const {
        return abs(x - p.x) + abs(y - p.y) + abs(z - p.z);
    }
};

ostream& operator<<(ostream& out, const point& p) {
    out << p.x << ", " << p.y << ", " << p.z;
    return out;
}

struct nanobot {
    point pos;
    ll r;
    bool in_range(const point& p) const {
        return r >= pos.dist(p);
    }
};

struct rect {
    ll x0, x1;
    ll y0, y1;
    bool contains(int x, int y) const {
        return x >= x0 && x <= x1 && y >= y0 && y <= y1;
    }
};

rect get_rect(ll x, ll y, ll r) {
    return rect{y - x - r, y - x + r, y + x - r, y + x + r};
}

struct help{
    ll y;
    bool opening;
    int bot_index;
    bool operator<(const help& other) const{
        if (y != other.y) {
            return y < other.y;
        }
        if (opening != other.opening) {
            return opening < other.opening;
        }
        return bot_index < other.bot_index;
    }
};
pair<ll, ll> solve_plane(ll z, const vector<nanobot>& bots, bool reverse) {
    vector<pair<rect, int> > rects;
    int idx = 0;
    for (auto b : bots) {
        ll rem = b.r - abs(z - b.pos.z);
        if (rem > 0) {
            rects.push_back({get_rect(b.pos.x, b.pos.y, rem), idx});
        }
        idx++;
    }

    vector<help> even_ys, odd_ys;
    unordered_set<ll> xs;
    for (auto r : rects) {
        xs.insert(r.first.x0);
        xs.insert(r.first.x0 - 1);
        xs.insert(r.first.x0 + 1);
        xs.insert(r.first.x1);
        xs.insert(r.first.x1 - 1);
        xs.insert(r.first.x1 + 1);
        if (r.first.y0 % 2 == 0) {
            even_ys.push_back(help{r.first.y0, true, r.second});
            odd_ys.push_back(help{r.first.y0 + 1, true, r.second});
        } else {
            even_ys.push_back(help{r.first.y0 + 1, true, r.second});
            odd_ys.push_back(help{r.first.y0, true, r.second});
        }
        if ((r.first.y1 + 1) % 2 == 0) {
            even_ys.push_back(help{r.first.y1 + 1, false, r.second});
            odd_ys.push_back(help{r.first.y1 + 2, false, r.second});
        } else {
            even_ys.push_back(help{r.first.y1 + 2, false, r.second});
            odd_ys.push_back(help{r.first.y1 + 1, false, r.second});
        }
    }

    sort(all(odd_ys));
    sort(all(even_ys));
    pair<ll, ll> best(0, 0);
    point best_point;
    for (ll x : xs) {
        int opened = 0;
        const vector<help>& ys = (x % 2 == 0 ? even_ys : odd_ys);
        for (auto yi : ys) {
            ll bx = bots[yi.bot_index].pos.y - bots[yi.bot_index].pos.x;
            ll br = bots[yi.bot_index].r - abs(z - bots[yi.bot_index].pos.z);
            if (bx - br > x - (x % 2 != (bx - br) % 2) ||
                 bx + br < x + (x % 2 != (bx - br) % 2)) {
                continue;
            }
            if (yi.opening) {
                opened++;

                ll score = (abs((yi.y - x) / 2) + abs((x + yi.y) / 2) + abs(z));
                if (reverse) {
                    score *= -1;
                }
                pair<ll,ll> temp{opened, score};
                if (temp > best) {
                    best = temp;
                    best_point = point{(yi.y - x) / 2, (yi.y + x) / 2, z};
                }
            } else {
                opened--;
            }
        }
    }
    return best;
}

pair<ll, ll> solve(const vector<nanobot>& bots) {
    ll c = 0;
    pair<ll, ll> c_value;
    auto hill_climbing = [&](bool minimize_distance) {
        c_value = solve_plane(c, bots, minimize_distance);
        ll step = 50000000;
        while (step > 0) {
            pair<ll, ll> lv = solve_plane(c - step, bots, minimize_distance);
            pair<ll, ll> rv = solve_plane(c + step, bots, minimize_distance);
            if (c_value >= lv && c_value >= rv) {
                step /= 2;
            } else if (lv > rv) {
                c -= step;
                c_value = lv;
            } else {
                c += step;
                c_value = rv;
            }
        }
    };
    // For some reason first maximizing and then minimizing the distance works
    hill_climbing(false);
    c_value.second *= -1;
    hill_climbing(true);
    return c_value;
}
int main() {
    regex nanobot_regex("pos=<(-?\\d+),(-?\\d+),(-?\\d+)>, r=(-?\\d+)");
    string s;
    vector<nanobot> bots;
    smatch match;
    while (getline(cin, s)) {
        if (regex_match(s, match, nanobot_regex)) {
            bots.push_back(nanobot{{stoll(match.str(1)), stoll(match.str(2)), stoll(match.str(3))}, stoll(match.str(4))});
        } else {
            cout << "Could not parse bot " << s << endl;
            exit(1);
        }
    }
    int strongest = 0;
    for (int i = 1; i < (int)bots.size(); ++i) {
        if (bots[i].r > bots[strongest].r) {
            strongest = i;
        }
    }
    int res1 = 0;
    for (auto b : bots) {
        if (bots[strongest].in_range(b.pos)) {
            res1++;
        }
    }
    cout << "Part 1 " << res1 << endl;
    
    auto best = solve(bots);
    cout << "Part 2 " << -best.second << endl;
    
    return 0;
}