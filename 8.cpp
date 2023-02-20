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

struct node {
    vector<node*> children;
    vector<int> metadata;
    vector<int> mem;
    ~node() {
        for (auto n : children) {
            delete n;
        }
    }
};

node* parse_node() {
    int size_children, size_metadata;
    cin >> size_children >> size_metadata;
    node* result = new node();
    for (int i = 0; i < size_children; ++i) {
        result->children.push_back(parse_node());
    }
    for (int i = 0; i < size_metadata; ++i) {
        int md;
        scanf("%d", &md);
        result->metadata.push_back(md);
    }
    return result;
}

int sum_metadata(node* n) {
    int res = accumulate(all(n->metadata), 0);
    for (auto c : n->children) {
        res += sum_metadata(c);
    }
    return res;
}
int value(node *n) {
    if (n->children.size() == 0) {
        return accumulate(all(n->metadata), 0);
    }
    n->mem.clear();
    n->mem.resize(n->children.size(), -1);
    int res = 0;
    for (int md : n->metadata) {
        md--;
        if (md >= n->children.size()) {
            continue;
        }
        if (n->mem[md] == -1) {
            n->mem[md] = value(n->children[md]);
        }
        res += n->mem[md];
    }
    return res;
}
int main() {
    node* root = parse_node();
    cout << "Part 1 " << sum_metadata(root) << endl;
    cout << "Part 2 " << value(root) << endl;
    return 0;
}