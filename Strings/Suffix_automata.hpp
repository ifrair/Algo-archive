// Suffix automation
// Number of substrings calculation problem

// https://acmp.ru/asp/do/index.asp?main=task&id_course=2&id_section=18&id_topic=42&id_problem=610

#pragma once

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const size_t alph_sz = 26;
struct Node {
    Node() {
        edge.resize(alph_sz, nullptr);
    }
    vector<Node*> edge;
    Node* link = nullptr;
    size_t len = 0;
    int dp = 0;
    bool used = 0;
};
Node* last;
Node* root;

void add_node(int val) {
    auto new_last = new Node();
    new_last->len = last->len + 1;
    auto cur = last;
    last = new_last;
    while (cur) {
        if (cur->edge[val]) break;
        cur->edge[val] = new_last;
        cur = cur->link;
    }
    if (!cur) {
        last->link = root;
        return;
    }
    auto cur_next = cur->edge[val];
    if (cur_next->len == cur->len + 1) {
        last->link = cur_next;
        return;
    }
    auto clone = new Node();
    *clone = *cur_next;
    clone->len = cur->len + 1;
    cur_next->link = clone;
    last->link = clone;
    while (cur) {
        if (cur->edge[val] != cur_next) break;
        cur->edge[val] = clone;
        cur = cur->link;
    }
}

void dfs(Node* cur, vector<Node*>& order) {
    if (cur->used) return;
    cur->used = 1;
    for (auto to : cur->edge) {
        if (!to) continue;
        dfs(to, order);
    }
    order.push_back(cur);
}

void calc_ans(int& ans) {
    vector<Node*> order;
    dfs(root, order);
    reverse(order.begin(), order.end());
    root->dp = 1;
    for (auto node : order) {
        ans += node->dp;
        for (auto to : node->edge) {
            if (!to) continue;
            to->dp += node->dp;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(NULL); cout.tie(NULL);
    string s;
    cin >> s;
    last = new Node();
    root = last;
    for (int i = 0; i < s.size(); ++i) {
        add_node(s[i] - 'a');
    }
    int ans = -1;
    calc_ans(ans);
    cout << ans;
}
