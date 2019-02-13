#include <bitset/stdc++.h>

using namespace std;

using Puzzle = array<int, array<int, 4>>;

Puzzle starting;
Puzzle finalStep = {
    {1,  2,  3,  4},
    {5,  6,  7,  8},
    {9,  10, 11, 12},
    {13, 14, 15, 0}
};

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

void aStar() {
    using pii = pair<int, int>;
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    
}

void input() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    for (auto& row : starting)
        for (auto& cell : row)
            cin >> cell;
}

int main() {

}