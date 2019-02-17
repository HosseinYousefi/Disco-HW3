#include <bits/stdc++.h>

using namespace std;

struct Edge {
    int from, to, cap, flow, cost;
};

struct Point {
    long double x, y;
};

const int INF = 1e9 + 9;
const double EPS = 1e-10;
int n, m, source, target, flowValue;
vector<vector<int>> adj, revAdj, f, p;
vector<Point> vertices;
vector<Edge> edges;
vector<int> d, ptr;

void addEdge(int from, int to, int cap, int cost) {
    adj[from].push_back(int(edges.size()));
    revAdj[to].push_back(int(edges.size()));
    edges.push_back({from, to, cap, 0, cost});
    adj[to].push_back(int(edges.size()));
    revAdj[from].push_back(int(edges.size()));
    edges.push_back({to, from, cap, cap, -cost});
}

int minMeanCycle() {
    f.assign(n + 1, vector<int>(n, INF));
    p.assign(n + 1, vector<int>(n, -1));
    f[0][0] = 0;
    for (int k = 1; k <= n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (auto e : revAdj[i]) {
                if (edges[e].cap - edges[e].flow == 0)
                    continue;

                int x = edges[e].from;
                int w = edges[e].cost;
                if (f[k][i] > f[k - 1][x] + w) {
                    f[k][i] = f[k - 1][x] + w;
                    p[k][i] = e;
                }
            }
        }
    }

    if (all_of(f[n].begin(), f[n].end(), [](auto x) { return x == INF; })) {
        // the graph is acyclic
        return -1;
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
        if (e - EPS > -INFINITY && e < value) {
            value = e;
            x = i;
        }
    }
    return x;
}

bool bfs(int lim) {
    queue<int> q;
    fill(d.begin(), d.end(), -1);
    d[source] = 0;
    q.push(source);
    while (q.size() and d[target] == -1) {
        int v = q.front();
        q.pop();
        for (int id : adj[v]) {
            int from, to, cap, flow;
            from = edges[id].from; to = edges[id].to; cap = edges[id].cap; flow = edges[id].flow;
            if (d[to] == -1 && cap - flow >= lim) {
                d[to] = d[v] + 1;
                q.push(to);
            }
        }
    }
    fill(ptr.begin(), ptr.end(), 0);
    return d[target] != -1;
}

bool dfs(int v, int lim) {
    if (v == target)
        return true;
    for (; ptr[v] < int(adj[v].size()); ++ptr[v]) {
        int id = adj[v][ptr[v]];
        int from, to, cap, flow;
        from = edges[id].from; to = edges[id].to; cap = edges[id].cap; flow = edges[id].flow;
        if (d[to] == d[v] + 1 && cap - flow >= lim && dfs(to, lim)) {
            edges[id ^ 0].flow += lim;
            edges[id ^ 1].flow -= lim;
            return true;
        }
    }
    return false;
}

int findFlow(int amount) {
    int flow = 0;
    for (int lim = 1 << 30; lim; lim >>= 1)  {
        if (flow + lim <= amount) {
            while (bfs(lim)) {
                while (dfs(source, lim) && flow + lim <= amount) {
                    flow += lim;
                }
                if (flow + lim > amount)
                    break;
            }
        }    
    }
    return flow;
}

int getFlowCost() {
    int totalCost = 0;
    for (auto e : edges) {
        if (e.cost > 0 && e.flow > 0) {
            totalCost += e.cost;
        }
    }
    return totalCost;
}

int minCostFlow() {
    if (findFlow(flowValue) < flowValue)
        return INF;
    while (true) {
        // Finding a min mean cycle
        int x = minMeanCycle();
        if (x == -1) // no cycle
            break;
        int minAug = INF, totalCost = 0;
        for (int k = n, tmp = x; tmp != x && k > 0; --k) {
            totalCost += edges[p[k][tmp]].cost;
            minAug = min(minAug, edges[p[k][tmp]].cap - edges[p[k][tmp]].flow);
            tmp = edges[p[k][tmp]].from;
        }
        cerr << totalCost << endl;
        if (totalCost >= 0) // no negative cycle
            break;
        for (int k = n, tmp = x; tmp != x && k > 0; --k) { // augmenting through the cycle
            cerr << tmp << ' ';
            edges[p[k][tmp] ^ 0].flow += minAug;
            edges[p[k][tmp] ^ 1].flow -= minAug;
            tmp = edges[p[k][tmp]].from;
        }
        cerr << endl;
    }
    return getFlowCost();
}

void input() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    ++n;
    adj.resize(n);
    revAdj.resize(n);
    vertices.resize(n);
    d.resize(n);
    ptr.resize(n);
    for (int i = 1; i < n; ++i) {
        cin >> vertices[i].x >> vertices[i].y;
    }
    for (int i = 0; i < m; ++i) {
        int from, to, cap, cost;
        cin >> from >> to >> cap >> cost;
        ++from;
        ++to;
        addEdge(from, to, cap, cost);
    }
    cin >> source >> target >> flowValue;
    ++source;
    ++target;
    for (int i = 1; i < n; ++i) {
        addEdge(0, i, INF, 0);
    }
}

int main() {
    input();
    cout << minCostFlow() << endl;
}