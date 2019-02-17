#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9 + 9;
int n, m, t; // |V|, |E|, source, target
vector<vector<pair<int, int>>> adjMinus;
vector<vector<int>> f, p;

void input() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    ++n;
    adjMinus.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w; // u, v in 1..|V| (in original graph)
        adjMinus[v].emplace_back(u, w);
    }
    // adding s with index 0
    f.resize(n + 1, vector<int>(n, INF));
    p.resize(n + 1, vector<int>(n, -1));
    f[0][0] = 0;
    for (int i = 1; i < n; ++i) {
        adjMinus[i].emplace_back(0, 0);
    }
}

vector<int> minMeanCycle() {
    for (int k = 1; k <= n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (auto [x, w] : adjMinus[i]) {
                if (f[k][i] > f[k - 1][x] + w) {
                    f[k][i] = f[k - 1][x] + w;
                    p[k][i] = x;
                }
            }
        }
    }
    if (all_of(f[n].begin(), f[n].end(), [](auto x) { return x == INF; })) {
        // the graph is acyclic
        return {};
    }
    auto eval = [&](int x) {
        double mx = -INFINITY;
        for (int k = 0; k < n; ++k) {
            if (f[k][x] < INF) {
                mx = max(mx, 1. * (f[n][x] - f[k][x]) / (n - k));
            }
        }
        return mx;
    };
    int x = -1;
    double value = INFINITY;
    for (int i = 0; i < n; ++i) {
        double e = eval(i);
        if (e > -INFINITY && e < value) {
            value = e;
            x = i;
        }
    }
    vector<int> res;
    for (int k = n; x != 0 && k > 0; --k) {
        res.push_back(x);
        x = p[k][x];
    }
    reverse(res.begin(), res.end());
    set<int> visited;
    vector<int> cycle;
    for (int i : res) {
        cycle.push_back(i);
        if (visited.count(i)) {
            break;
        }
        visited.insert(i);
    }
    return cycle;
}

int main() {
    input();
    auto cycle = minMeanCycle();
    for (int i : cycle) {
        cout << i << ' ';
    }
}