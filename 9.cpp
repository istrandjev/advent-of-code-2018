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

struct Node {
    Node* prev;
    Node* next;
    int value;
    Node(int v): value(v), next(NULL), prev(NULL){};
    Node* by_offset(int offset) {
        Node* res = this;
        if (offset > 0) {
            for (int i = 0; i < offset; ++i) {
                res = res->next;
            }
        } else {
            for (int i = 0; i < -offset; ++i) {
                res = res->prev;
            }
        }
        return res;
    }
};

struct Game {
    Node* current;
    Game(): current(NULL) {};
    ~Game() {
        while (current != NULL) {
            remove();
        }
    }
    void remove() {
        Node* new_current = NULL;
        if (current->next != current) {
            new_current = current->next;
            new_current->prev = current->prev;
            current->prev->next = new_current;
        }
        delete current;
        current = new_current;
    }
    void add(int value) {
        Node* new_node = new Node(value);
        if (current == NULL) {
            current = new_node;
            current->prev = new_node;
            current->next = new_node;
        } else {
            current = current->next;
            current->next->prev = new_node;
            new_node->next = current->next;
            current->next = new_node;
            new_node->prev = current;
            current = new_node;
        }
    }
    void print() {
        if (current == NULL) {
            return;
        }
        Node* c = current->next;
        while (c != current) {
            printf("%d -> ", c->value);
            c = c->next;
        }
        printf("%d\n", current->value);
    }
};

ll solve_game(int players, int marbles) {
    vector<ll> scores(players);
    int current_player = 0;   
    Game game;
    for (int mi = 0; mi <= marbles; ++mi) {
        if (mi == 0 || mi % 23 != 0) {
            game.add(mi);
        } else {
            scores[current_player] += mi;
            game.current = game.current->by_offset(-7);
            scores[current_player] += game.current->value;
            game.remove();
        }
        current_player = (current_player + 1) % players;
    }
    return *max_element(all(scores));
}
int main() {
    int players = 471;
    int marbles = 72026;
    
    cout << "Part 1 " << solve_game(players, marbles) << endl;
    cout << "Part 1 " << solve_game(players, marbles * 100) << endl;
    return 0;
}