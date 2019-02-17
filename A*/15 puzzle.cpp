#include <bits/stdc++.h>

using namespace std;

using Puzzle = array<array<int, 4>, 4>;

Puzzle starting;
map<Puzzle, int> weight;

int distanceSum(const Puzzle& p) {
    int res = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (p[i][j]) {
                int xFinal = (p[i][j] - 1) / 4;
                int yFinal = (p[i][j] - 1) % 4;
                res += abs(i - xFinal) + abs(j - yFinal);
            }
        }
    }
    return res;
}

bool valid(int i, int j) {
    return (0 <= i && i < 4) && (0 <= j && j < 4);
}

vector<Puzzle> neighbors(Puzzle p) {
    vector<Puzzle> v;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (p[i][j] == 0) {
                auto pushNeighbor = [&](int x, int y) {
                    if (valid(x, y)) {
                        swap(p[x][y], p[i][j]);
                        v.push_back(p);
                        swap(p[x][y], p[i][j]);
                    }
                };
                pushNeighbor(i + 1, j);
                pushNeighbor(i - 1, j);
                pushNeighbor(i, j + 1);
                pushNeighbor(i, j - 1);
                return v;
            }
        }
    }
    return v;
}

int aStar() {
    Puzzle finalStep;
    finalStep[0] = {1,  2,  3,  4};
    finalStep[1] = {5,  6,  7,  8};
    finalStep[2] = {9,  10, 11, 12};
    finalStep[3] = {13, 14, 15, 0};

    using State = tuple<int, int, Puzzle>;
    priority_queue<State, vector<State>, greater<State>> pq;
    pq.emplace(distanceSum(starting), 0, starting);
    weight[starting] = 0;
    while (pq.size()) {
        int h, w;
        Puzzle p;
        tie(h, w, p) = pq.top(); pq.pop();
        if (p == finalStep) {
            return w;
        }
        if (w > weight[p]) {
            continue;
        }
        for (auto&& toGo : neighbors(p)) {
            if (!weight.count(toGo) || weight[toGo] > w + 1) {
                weight[toGo] = w + 1;
                pq.emplace(distanceSum(toGo) + w + 1, w + 1, toGo);
            }
        }
    }
    return -1;
}

void input() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    for (auto& row : starting)
        for (auto& cell : row)
            cin >> cell;
}

int main() {
    input();
    cout << aStar() << endl;
}