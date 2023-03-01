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

typedef void (*Operation)(int, int, int);

int registers[4];
void add1(int a, int b, int c) {
    registers[c] = registers[a] + registers[b];
}
void add2(int a, int b, int c) {
    registers[c] = registers[a] + b;
}
void mul1(int a, int b, int c) {
    registers[c] = registers[a] * registers[b];
}
void mul2(int a, int b, int c) {
    registers[c] = registers[a] * b;
}
void ban1(int a, int b, int c) {
    registers[c] = (registers[a] & registers[b]);
}
void ban2(int a, int b, int c) {
    registers[c] = (registers[a] & b);
}
void bor1(int a, int b, int c) {
    registers[c] = (registers[a] | registers[b]);
}
void bor2(int a, int b, int c) {
    registers[c] = (registers[a] | b);
}
void set1(int a, int b, int c) {
    registers[c] = registers[a];
}
void set2(int a, int b, int c) {
    registers[c] = a;
}
void gt1(int a, int b, int c) {
    registers[c] = int(a > registers[b]);
}
void gt2(int a, int b, int c) {
    registers[c] = int(registers[a] > b);
}
void gt3(int a, int b, int c) {
    registers[c] = int(registers[a] > registers[b]);
}
void eq1(int a, int b, int c) {
    registers[c] = int(a == registers[b]);
}
void eq2(int a, int b, int c) {
    registers[c] = int(registers[a] == b);
}
void eq3(int a, int b, int c) {
    registers[c] = int(registers[a] == registers[b]);
}

vector<int> get(const smatch& match) {
    vector<int> res;
    for (int i = 1; i <= 4; ++i) {
        res.push_back(stoi(match.str(i)));
    }
    return res;
}

bool matches(const vector<int>& before, const vector<int>& values, const vector<int>& after, Operation op) {
    for (int i = 0; i < 4; ++i) {
        registers[i] = before[i];
    }
    op(values[1], values[2], values[3]);
    bool good = true;
    for (int i = 0; i < 4; ++i) {
        if (registers[i] != after[i]) {
            good = false;
        }
    }
    return good;
}
int count_ops(const vector<int>& before, const vector<int>& values, const vector<int>& after, const vector<Operation>& all_ops) {
    int answer = 0;
    for (auto op: all_ops) {
        answer += matches(before, values, after, op);
    }
    return answer;
}

struct edge {
    int to;
    int next, cap;
    edge(int t = 0, int n = 0, int c = 0) {
        to = t;
        next = n;
        cap = c;
    }
};
const int max_edge = 600000;
const int max_nodes = max_edge;
const int max_size = 100010;
edge edges[max_edge];
int first[max_edge];
int num_edges;
inline void add_edge(int from, int to, int cap, int cap2) {
    edges[num_edges].to = to;
    edges[num_edges].cap = cap;
    edges[num_edges].next = first[from];
    first[from] = num_edges++;

    edges[num_edges].to = from;
    edges[num_edges].cap = cap2;
    edges[num_edges].next = first[to];
    first[to] = num_edges++;
}
int vis[max_nodes];
char used[max_nodes];
int source, sink;
int dfs(int from, int cap) {
    if (from == sink) {
        return cap;
    }
    for (int u = first[from]; u != -1; u = edges[u].next) {
        if (vis[from] != vis[edges[u].to] + 1 || edges[u].cap <= 0
                || used[edges[u].to])
            continue;
        int r = dfs(edges[u].to, min(cap, edges[u].cap));
        if (r != -1) {
            edges[u].cap -= r;
            edges[u ^ 1].cap += r;
            return r;
        }
    }
    used[from] = 1;
    return -1;
}
char used1[max_size];
char used2[max_size];
vector<pair<int, int> > bipartite_matching(int n, int m,
        const vector<pair<int, int> >& edge) {
    memset(first, -1, sizeof(first));
    num_edges = 0;
    const int inf = 10000000;
    memset(used1, 0, sizeof(used1));
    memset(used2, 0, sizeof(used2));
    for (int i = 0; i < (int) edge.size(); i++) {
        if (used1[edge[i].first] || used2[edge[i].second]) {
            add_edge(edge[i].first, edge[i].second + n, inf, 0);
        } else {
            add_edge(edge[i].first, edge[i].second + n, inf - 1, 1);
            used1[edge[i].first] = 1;
            used2[edge[i].second] = 1;
        }
    }
    const int so = n + m;
    const int si = n + m + 1;
    source = so;
    sink = si;
    for (int i = 0; i < n; i++) {
        if (!used1[i]) {
            add_edge(so, i, 1, 0);
        } else {
            add_edge(so, i, 0, 1);
        }
    }
    for (int i = 0; i < m; i++) {
        if (!used2[i]) {
            add_edge(i + n, si, 1, 0);
        } else {
            add_edge(i + n, si, 0, 1);
        }
    }
    while (true) {
        queue<int> tc;
        tc.push(si);
        memset(vis, -1, sizeof(vis));
        vis[si] = 0;
        while (!tc.empty() && vis[so] == -1) {
            int c = tc.front();
            tc.pop();
            for (int u = first[c]; u != -1; u = edges[u].next) {
                if (vis[edges[u].to] == -1 && edges[u ^ 1].cap > 0) {
                    vis[edges[u].to] = vis[c] + 1;
                    tc.push(edges[u].to);
                    if (edges[u].to == so) {
                        break;
                    }
                }
            }
        }
        if (vis[so] == -1) {
            break;
        }
        memset(used, 0, sizeof(used));
        bool updated = false;
        while (true) {
            int r = dfs(so, inf);
            if (r == -1) {
                break;
            }
            updated = true;
        }
        if (!updated)
            break;
    }
    vector < pair<int, int> > res;
    for (int i = 0; edges[i + 1].to != so; i += 2) {
        if (edges[i].cap != inf) {
            if (edges[i].cap < inf) {
                res.push_back(make_pair(edges[i ^ 1].to, edges[i].to - n));
            }
        }
    }
    return res;
}
int main() {
    regex before_re("Before: \\[(\\d+), (\\d+), (\\d+), (\\d+)\\]");
    regex numbers("(\\d+) (\\d+) (\\d+) (\\d+)");
    regex after_re("After:  \\[(\\d+), (\\d+), (\\d+), (\\d+)\\]");
    smatch match;
    vector<Operation> all_ops{add1, add2, mul1, mul2, ban1, ban2, bor1, bor2, set1, set2, gt1, gt2, gt3, eq1, eq2, eq3};
    string s;
    int res1 = 0;
    vector<vector<char> > code_to_op(all_ops.size(), vector<char>(all_ops.size(), 1));
    vector<vector<int> > program;
    while (getline(cin, s)) {
        if (!regex_match(s, match, before_re)) {
            break;
        }
        auto before = get(match);
        getline(cin, s);
        if (!regex_match(s, match, numbers)) {
            cout << "Could not parse " << s << endl;
        }
        auto values = get(match);
        getline(cin, s);
        if (!regex_match(s, match, after_re)) {
            cout << "Could not parse after " << s << endl;
        }
        auto after = get(match);
        res1 += (count_ops(before, values, after, all_ops) >= 3);
        for (int i = 0; i < all_ops.size(); ++i) {
            if (!matches(before, values, after, all_ops[i])) {
                code_to_op[values[0]][i] = 0;
            }
        }
        getline(cin, s);
    }
    getline(cin, s);
    while (getline(cin, s)) {
        if (!regex_match(s, match, numbers)) {
            cout << "Could not parse program line " << s << endl;
            exit(1);
        }
        program.push_back(get(match));
    }
    cout << "Part 1 " << res1 << endl;

    vector<pair<int, int> > edges;
    for (int i = 0 ; i < code_to_op.size(); ++i) {
        for (int j = 0; j < code_to_op[i].size(); ++j) {
            if (code_to_op[i][j]) {
                edges.push_back({i, j});
            }
        }
    }
    vector<pair<int, int> > matching = bipartite_matching(all_ops.size(), all_ops.size(), edges);
    vector<int> decode_op(all_ops.size());
    for (auto p : matching) {
        decode_op[p.first] = p.second;
    }
    memset(registers, 0, sizeof(registers));
    for (auto p : program) {
        auto cop = all_ops[decode_op[p[0]]];
        cop(p[1], p[2], p[3]);
    }
    cout << "Part 2 " << registers[0] << endl;
    return 0;
}