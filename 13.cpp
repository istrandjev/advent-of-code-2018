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

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

int get_direction(char c) {
    switch(c) {
        case '>': return 1;
        case 'v': return 2; 
        case '<': return 3; 
        case '^': return 0;
        default: return -1;
    }
}
struct cart {
    int y, x;
    int dir;
    int turn;
    bool crashed;
    cart(int _y, int _x, int d) : y(_y), x(_x), dir(d), turn(0), crashed(false) {}
    bool operator<(const cart& o) const{
        if (y != o.y) {
            return y < o.y;
        }
        if (x != o.x) {
            return x < o.x;
        }
        return dir < o.dir;
    }
};


int main() {
    vector<string> a;
    string s;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    int n = a.size();
    int m = a[0].size();
    vector<cart> carts;
    vector<vector<int> > to_cart(n, vector<int>(m, -1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int d = get_direction(a[i][j]);
            if (d != -1) {
                carts.emplace_back(i, j, d);
                to_cart[i][j] = (int)carts.size() - 1;
                if (d % 2) {
                    a[i][j] = '-';
                } else {
                    a[i][j] = '|';
                }
            }
        }
    }

    bool has_collision = false;
    int collision_x, collision_y;
    int total_crashed = 0;
    for(int step = 0; total_crashed + 1 < carts.size(); ++step) {
        sort(all(carts));
        for (int i = 0; i < (int)carts.size(); ++i) {
            if (carts[i].crashed) {
                continue;
            }
            int ty = carts[i].y + moves[carts[i].dir][0];
            int tx = carts[i].x + moves[carts[i].dir][1];
            if (a[ty][tx] == '+') {
                if (carts[i].turn == 0) {
                    carts[i].dir = (carts[i].dir + 3) % 4;
                } else if (carts[i].turn == 2) {
                    carts[i].dir = (carts[i].dir + 1) % 4;
                }
                carts[i].turn =  (carts[i].turn + 1) % 3;
            } else if (a[ty][tx] == '\\') {
                if (carts[i].dir == 1) {
                    carts[i].dir = 2;
                } else if (carts[i].dir == 0) {
                    carts[i].dir = 3;
                } else if (carts[i].dir == 2) {
                    carts[i].dir = 1; 
                } else {
                    carts[i].dir = 0;
                }
            } else if (a[ty][tx] == '/') {
                if (carts[i].dir == 1) {
                    carts[i].dir = 0;
                } else if (carts[i].dir == 0) {
                    carts[i].dir = 1;
                } else if (carts[i].dir == 2) {
                    carts[i].dir = 3; 
                } else {
                    carts[i].dir = 2;
                }
            }
            for (int j = 0; j < (int)carts.size(); ++j) {
                if (j == i || carts[j].crashed) {
                    continue;
                }

                if (tx == carts[j].x && ty == carts[j].y) {
                    carts[i].crashed = true;
                    carts[j].crashed = true;
                    total_crashed += 2;
                    if (!has_collision) {
                        cout << "Part 1 " << tx << "," << ty << endl; 
                        has_collision = true;
                    }
                    break;
                }
            }
            if (!carts[i].crashed) {
                carts[i].x = tx;
                carts[i].y = ty;
            }
        }
    }
    for (int i = 0; i < (int)carts.size(); ++i) {
        if (!carts[i].crashed) {
            cout << "Part 2 " << carts[i].x << "," << carts[i].y << endl;
            break;
        }
    }
    return 0;
}