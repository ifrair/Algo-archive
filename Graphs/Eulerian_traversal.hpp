#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
 
using namespace std;

vector<unordered_map<int, pair<int, int>>> v;
vector<int> ans;

void dfs(int node, int gate0, int gate1) {
    while (!v[node].empty()) {
        auto it = v[node].begin();
        auto gate_from = it->first;
        auto to = (it->second).first;
        auto gate_to = (it->second).second;
        v[node].erase(gate_from);
        v[to].erase(gate_to);
        dfs(to, gate_from, gate_to);
    }
    if (gate0 != -1) {
        ans.push_back(gate1);
        ans.push_back(gate0);
    }
}

int main() {
    int n;
    cin >> n;
    v.resize(n);
    for (int i = 0; i < 2 * n; ++i) {
        int x, y;
        cin >> x >> y;
        int x_old = x, y_old = y;
        x = (x - 1) / 4;
        y = (y - 1) / 4;
        v[x][x_old] = {y, y_old};
        v[y][y_old] = {x, x_old};
    }
    dfs(0, -1, -1);
    if (ans.size() != 4 * n) {
        cout << "No";
        return 0;
    }
    cout << "Yes\n";
    reverse(ans.begin(), ans.end());
    for (auto x: ans) {
        cout << x << ' ';
    }
}
