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

ll registers[6];
int tied_register;
int instruction_pointer;
void add1(int a, int b, int c) {
    registers[c] = registers[a] + registers[b];
}
void add2(int a, int b, int c) {
    registers[c] = registers[a] + (ll)b;
}
void mul1(int a, int b, int c) {
    registers[c] = registers[a] * registers[b];
}
void mul2(int a, int b, int c) {
    registers[c] = registers[a] * (ll)b;
}
void ban1(int a, int b, int c) {
    registers[c] = (registers[a] & registers[b]);
}
void ban2(int a, int b, int c) {
    registers[c] = (registers[a] & (ll)b);
}
void bor1(int a, int b, int c) {
    registers[c] = (registers[a] | registers[b]);
}
void bor2(int a, int b, int c) {
    registers[c] = (registers[a] | (ll)b);
}
void set1(int a, int b, int c) {
    registers[c] = registers[a];
}
void set2(int a, int b, int c) {
    registers[c] = (ll)a;
}
void gt1(int a, int b, int c) {
    registers[c] = int((ll)a > registers[b]);
}
void gt2(int a, int b, int c) {
    registers[c] = int(registers[a] > (ll)b);
}
void gt3(int a, int b, int c) {
    registers[c] = int(registers[a] > registers[b]);
}
void eq1(int a, int b, int c) {
    registers[c] = int((ll)a == registers[b]);
}
void eq2(int a, int b, int c) {
    registers[c] = int(registers[a] == (ll)b);
}
void eq3(int a, int b, int c) {
    registers[c] = int(registers[a] == registers[b]);
}

void tie_register(int a) {
    tied_register = a;
}
struct Step {
    Operation op;
    int x, y, z;
    void call() const {
        op(x, y, z);
    }
};

void do_step(const vector<Step>& program) {
    bool is_tied = (tied_register != -1);
    if (is_tied) {
        registers[tied_register] = instruction_pointer;
    }
    program[instruction_pointer].call();
    if (is_tied) {
        instruction_pointer = registers[tied_register];
    }
    instruction_pointer++;
}
int main() {
    regex tie_register_re("#ip (\\d)");
    regex op_regex("(\\w+) (\\d+) (\\d+) (\\d+)");
    unordered_map<string, Operation> ops{
        {"addr", add1}, {"addi", add2}, {"mulr", mul1}, {"muli", mul2},
        {"banr", ban1}, {"bani", ban2}, {"borr", bor1}, {"bori", bor2}, 
        {"setr", set1}, {"seti", set2}, {"gtir", gt1}, {"gtri", gt2}, {"gtrr", gt3}, 
        {"eqir", eq1}, {"eqri", eq2}, {"eqrr", eq3}
    };
    
    memset(registers, 0, sizeof(registers));
    tied_register = -1;
    string s;
    vector<Step> program;
    smatch match;
    while (getline(cin, s)) {
        if (regex_match(s, match, tie_register_re)) {
            int val = stoi(match.str(1));
            tie_register(val);
        } else if (regex_match(s, match, op_regex)) {
            int vals[3];
            auto it = ops.find(match.str(1));
            if (it == ops.end()) {
                cout << "No operation defined for " << match.str(1) << endl;
                exit(1);
            }
            for (int i = 2; i <= 4; ++i) {
                vals[i - 2] = stoi(match.str(i));
            }
            program.push_back(Step{it->second, vals[0], vals[1], vals[2]});
        } else {
            cout << "Could not parse " << s << endl;
            exit(1);
        }
    }
    instruction_pointer = 0;
    while (instruction_pointer >= 0 && instruction_pointer < (int)program.size()) {
        do_step(program);
    }
    cout << "Part 1 " << registers[0] << endl;
    memset(registers, 0, sizeof(registers));
    instruction_pointer = 0;
    registers[0] = 1;
    while (instruction_pointer >= 0 && instruction_pointer < (int)program.size()) {
        if (instruction_pointer == 3) {
            break; 
        }
        
        do_step(program);
    }
    ll res2 = 0;
    for (ll x = 1; x * x <= registers[5]; ++x) {
        if (registers[5] % x == 0) {
            res2 += x;
            if (x * x != registers[5]) {
                res2 += registers[5] / x;
            }
        }
    }
    cout << "Part 2 " << res2 << endl;
    return 0;
}