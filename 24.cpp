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

struct army {
    set<string> immunities;
    set<string> weaknesses;
    int units;
    int health;
    int initiative;
    int damage;
    string damage_type;
    int effective_power() const {
        return units * damage;
    }
};

vector<string> split(const string& s, char delim=' ') {
    vector<string> res;
    string cur;
    for (auto c : s) {
        if (c == delim) {
            res.push_back(cur);
            cur = "";
        } else {
            cur.push_back(c);
        }
    }
    if (s.back() != delim) {
        res.push_back(cur);
    }
    return res;
}

void update_with_part(army& current_army, const string& part) {
    if (part.size() == 0) {
        return;
    }
    regex r(" ?(immune|weak) to (.*)");

    smatch match;
    if (!regex_match(part, match, r)) {
        cout << "Could not parse part " << part << endl;
        exit(1);
    }
    set<string>& property = match.str(1) == "weak" ? current_army.weaknesses : current_army.immunities;
    for (auto dt : split(match.str(2), ',')) {
        int from = 0;
        int len = dt.size();
        while (dt[len - 1] == ' ') {
            len--;
        }
        while (dt[from] == ' ') {
            from++;
            len--;
        }
        if (from != 0 || len != (int)dt.size()) {
            dt = dt.substr(from, len);
        }
        
        property.insert(dt);
    }
}

void update_modifiers(army& current_army, const string& s) {
    vector<string> parts = split(s, ';');
    for (const string& part: parts) {
        update_with_part(current_army, part);
    }
}

void print_army(const army& t) {
    printf("Army units=%d@%d damage %d (%s)  [", t.units, t.health, t.damage, t.damage_type.c_str());
    printf("Immune to");
    for (auto im :t.immunities) {
        printf(" %s", im.c_str());
    }
    printf("; Weak to");
    for (auto im :t.weaknesses) {
        printf(" %s", im.c_str());
    }
    cout << "]"<< endl;
}
void print_armies(const vector<army>& armies) {
    for (auto t : armies) {
        print_army(t);    
    }
}

ll get_damage(const army& attacker, const army& defender) {
    ll result = attacker.effective_power();
    if (defender.weaknesses.count(attacker.damage_type)) {
        result *= 2;
    } else if (defender.immunities.count(attacker.damage_type)) {
        result *= 0;
    }
    return result;
}

pair<pair<int, int>, int> get_target_score(const army& attacker, const army& defender) {
    return {{get_damage(attacker, defender), defender.effective_power()}, defender.initiative};
}

vector<pair<int, int> > select_targets(const vector<army>& attacking, const vector<army>& defending) {
    vector<char> targeted(defending.size(), 0);

    vector<pair<pair<int, int>, int> > attackers;
    for (int i = 0; i < (int)attacking.size(); ++i) {
        attackers.push_back({{attacking[i].effective_power(), attacking[i].initiative}, i});
    }
    sort(all(attackers));
    reverse(all(attackers));

    vector<pair<int, int> > res;
    for (int i = 0; i < (int)attackers.size(); ++i) {
        int ai = attackers[i].second;
        const army& current = attacking[ai];

        int ti = -1;
        for (int j = 0; j < (int)defending.size(); ++j) {
            if (targeted[j]) {
                continue;
            }
            if (get_damage(current, defending[j]) == 0) {
                continue;
            }
            if (ti == -1 || get_target_score(current, defending[j]) > get_target_score(current, defending[ti])) {
                ti = j;
            }
        }
        if (ti != -1) {
            targeted[ti] = 1;
            res.push_back({ai, ti});
        }
    }
    return res;
}

pair<vector<army>, vector<army> > solve(const vector<army>& a, const vector<army>& b) {
    auto immune = a;
    auto infection = b;
    bool killed = true;
    while (infection.size() * immune.size() != 0 && killed) {
        killed = false;
        vector<pair<int, pair<army*, army*> > > fights;
        for (auto ap : select_targets(infection, immune)) {
            fights.push_back({-infection[ap.first].initiative, {&infection[ap.first], &immune[ap.second]}});
        }
        for (auto ap : select_targets(immune, infection)) {
            fights.push_back({-immune[ap.first].initiative, {&immune[ap.first], &infection[ap.second]}});
        }         
        sort(all(fights));

        for (auto fight: fights) {
            ll damage = get_damage(*fight.second.first, *fight.second.second);
            if (damage) {
                if ((int)damage / fight.second.second->health > 0) {
                    killed = true;
                }
                fight.second.second->units = max(0, fight.second.second->units - ((int)damage / fight.second.second->health));
            }
        }
        auto clear_dead = [](vector<army>& to_filter) {
            vector<army> updated;
            for (auto a: to_filter) {
                if (a.units) {
                    updated.push_back(a);
                }
            }
            to_filter = updated;
        };
        clear_dead(immune);
        clear_dead(infection);         
    }
    return {immune, infection};
}
int main() {
    regex army_regex("(\\d+) units each with (\\d+) hit points (\\((.*)\\) )?with an attack that does (\\d+) (\\w+) damage at initiative (\\d+)");
    smatch match;
    string s;

    vector<army> infection, immune;
    vector<army>* current = &infection;
    while (getline(cin, s)) {
        if (s.size() == 0) {
            continue;
        }

        if (s == "Infection:") {
            current = &infection;
            continue;
        }
        if (s == "Immune System:") {
            current = &immune;
            continue;
        }
        if (!regex_match(s, match, army_regex)) {
            cout << "Could not parse " << s << endl;
            return 1;
        }
        army current_army;
        current_army.units = stoi(match.str(1));
        current_army.health = stoi(match.str(2));
        update_modifiers(current_army, match.str(4));
        current_army.damage = stoi(match.str(5));
        current_army.damage_type = match.str(6);
        current_army.initiative = stoi(match.str(7));
        current->push_back(current_army);
    }
    auto sol1 = solve(immune, infection);
    ll res1 = 0;
    for (auto a : sol1.first) {
        res1 += a.units;
    }
    for (auto a : sol1.second) {
        res1 += a.units;
    }
    cout << "Part 1 " << res1 << endl;
    int beg = 0;
    int end = 100000;
    while (end - beg > 1) {
        int mid = (end + beg) / 2;
        for (auto& a : immune) {
            a.damage += mid;
        }
        auto temp = solve(immune, infection);
        if (temp.second.size() == 0) {
            end = mid;
        } else {
            beg = mid;
        }
        for (auto& a : immune) {
            a.damage -= mid;
        }
    }
    for (auto& a : immune) {
        a.damage += end;
    }
    auto sol2 = solve(immune, infection);
    int res2 = 0;
    for (auto a : sol2.first) {
        res2 += a.units;
    }
    cout << "Part 2 " << res2 << endl;
    return 0;
}