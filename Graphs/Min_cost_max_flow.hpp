// Ford-Falkerson min-cost-max-flow
// Dijkstra with Johnson potentials
// Assignment problem

#pragma once

#include <iostream>
#include <vector>

using namespace std;


struct Edge {
    int cap = 0;
    int flow = 0;
    long long cost = 0;
    long long start_cost = 0;
};

const long long INF_DIST = 2e18;
const long long INF_CAP = 2e9;

void ford_belman(const vector<vector<Edge>>& edges, size_t n, size_t s, vector<long long>& distances) {
    distances.assign(n, INF_DIST);
    distances[s] = 0;
    for (size_t iteration = 0; iteration < n - 1; ++iteration) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (edges[i][j].cap > edges[i][j].flow &&
                    distances[j] > distances[i] + edges[i][j].cost) {
                    distances[j] = distances[i] + edges[i][j].cost;
                }
            }
        }
    }
}

void dijkstra(
    const vector<vector<Edge>>& edges,
    size_t n,
    size_t s,
    vector<long long>& distances,
    vector<size_t>& parents
) {
    distances.assign(n, INF_DIST);
    distances[s] = 0;
    parents.assign(n, n);
    vector<bool> used(n, false);
    while (true) {
        size_t cur = 0;
        long long min_dist = INF_DIST;
        for (size_t i = 0; i < n; ++i) {
            if (!used[i] && min_dist > distances[i]) {
                min_dist = distances[i];
                cur = i;
            }
        }
        if (min_dist == INF_DIST) break;
        used[cur] = true;
        for (size_t to = 0; to < n; ++to) {
            if (edges[cur][to].cap > edges[cur][to].flow &&
                distances[to] > distances[cur] + edges[cur][to].cost) {
                distances[to] = distances[cur] + edges[cur][to].cost;
                parents[to] = cur;
            }
        }
    }
}

void update_potentials(vector<vector<Edge>>& edges, size_t n, const vector<long long>& potentials) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            edges[i][j].cost += potentials[i] - potentials[j];
        }
    }
}

int main() {
    size_t n;
    cin >> n;
    size_t num_v = n * 2 + 2;
    size_t s = n * 2;
    size_t t = n * 2 + 1;

    vector<vector<Edge>> edges(num_v, vector<Edge>(num_v));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = n; j < n * 2; ++j) {
            long long cost;
            cin >> cost;
            edges[i][j] = {1, 0, cost, cost};
            edges[j][i] = {0, 0, -cost, -cost};
        }
    }
    for (int i = 0; i < n; ++i) {
        edges[s][i].cap = 1;
        edges[i + n][t].cap = 1;
    }
    
    long long ans = 0;
    vector<long long> potentials;
    ford_belman(edges, num_v, s, potentials);
    update_potentials(edges, num_v, potentials);
    while (true) {
        vector<size_t> parents;
        vector<long long> distances;
        dijkstra(edges, num_v, s, distances, parents);
        if (parents[t] == num_v) {
            break;
        }
        int min_cap = INF_CAP;
        for (size_t i = t; i != s; i = parents[i]) {
            min_cap = min(min_cap, edges[parents[i]][i].cap - edges[parents[i]][i].flow);
        }
        for (size_t i = t; i != s; i = parents[i]) {
            edges[parents[i]][i].flow += min_cap;
            edges[i][parents[i]].flow -= min_cap;
            ans += edges[parents[i]][i].start_cost;
        }
        dijkstra(edges, num_v, s, potentials, parents);
        update_potentials(edges, num_v, potentials);
    }
    cout << ans << endl;
}

// https://acmp.ru/asp/do/index.asp?main=task&id_course=2&id_section=32&id_topic=57&id_problem=1050
