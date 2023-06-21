#include <minisat/core/Solver.h>
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;
using Minisat::lbool;
using Minisat::mkLit;

int m, n, r = 0, sMax = 0;
vector<int> regionSize;
vector<vector<int>> H, R;
Minisat::Solver S;
chrono::steady_clock::time_point start, finish;

Minisat::Var h(int i, int j, int v) {
    i--, j--, v--;
    return (i * n + j) * sMax + v;
}

// Each cell must contain a single value ranging from 1 to regionSize[k] where k is the region label of said cell
void phi_1() {
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            for (int v = 1; v < regionSize[R[i][j]]; v++)
                for (int v_ = v + 1; v_ <= regionSize[R[i][j]]; v_++)
                    S.addClause(~mkLit(h(i, j, v)), ~mkLit(h(i, j, v_)));
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
        {
            Minisat::vec<Minisat::Lit> clause;
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                clause.push(mkLit(h(i, j, v)));
            S.addClause(clause);
        }
}

// Each region k composed of regionSize[k] cells must be filled with distinct values from the set {1, 2, ..., regionSize[k]}
void phi_2() {
    for (int k = 1; k <= r; k++)
        for (int v = 1; v <= regionSize[k]; v++)
        {
            Minisat::vec<Minisat::Lit> clause;
            for (int i = 1; i <= m; i++)
                for (int j = 1; j <= n; j++)
                    if (R[i][j] == k)
                        clause.push(mkLit(h(i, j, v)));
            S.addClause(clause);
        }
}

// No two adjacent cells, either orthogonally or diagonally, can contain the same value
void phi_3() {
    for (int i = 1; i <= m - 1; i++)
        for (int j = 1; j <= n; j++)
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                S.addClause(~mkLit(h(i, j, v)), ~mkLit(h(i + 1, j, v)));

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n - 1; j++)
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                S.addClause(~mkLit(h(i, j, v)), ~mkLit(h(i, j + 1, v)));

    for (int i = 2; i <= m; i++)
        for (int j = 1; j <= n; j++)
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                S.addClause(~mkLit(h(i, j, v)), ~mkLit(h(i - 1, j, v)));

    for (int i = 1; i <= m; i++)
        for (int j = 2; j <= n; j++)
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                S.addClause(~mkLit(h(i, j, v)), ~mkLit(h(i, j - 1, v)));

    for (int i = 2; i <= m; i++)
        for (int j = 2; j <= n; j++)
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                S.addClause(~mkLit(h(i, j, v)), ~mkLit(h(i - 1, j - 1, v)));

    for (int i = 2; i <= m; i++)
        for (int j = 1; j <= n - 1; j++)
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                S.addClause(~mkLit(h(i, j, v)), ~mkLit(h(i - 1, j + 1, v)));

    for (int i = 1; i <= m - 1; i++)
        for (int j = 2; j <= n; j++)
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                S.addClause(~mkLit(h(i, j, v)), ~mkLit(h(i + 1, j - 1, v)));

    for (int i = 1; i <= m - 1; i++)
        for (int j = 1; j <= n - 1; j++)
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                S.addClause(~mkLit(h(i, j, v)), ~mkLit(h(i + 1, j + 1, v)));
}

void hint() {
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            if (H[i][j] != 0)
                S.addClause(mkLit(h(i, j, H[i][j])));
}

void readInput() {
    cin >> m >> n;
    H.resize(m + 1, vector<int>(n + 1));
    R.resize(m + 1, vector<int>(n + 1));

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            cin >> H[i][j];

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            cin >> R[i][j], r = max(r, R[i][j]);

    start = chrono::steady_clock::now();

    regionSize.resize(r + 1);
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            regionSize[R[i][j]]++, sMax = max(sMax, regionSize[R[i][j]]);

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            for (int v = 1; v <= sMax; v++)
                static_cast<void>(S.newVar());
}

void printSolution() {
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            for (int v = 1; v <= regionSize[R[i][j]]; v++)
                if (S.modelValue(h(i, j, v)).isTrue())
                    H[i][j] = v;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            cout << H[i][j] << " \n"[j == n];
    long double timeTaken = chrono::duration_cast<chrono::microseconds>(finish - start).count();
    cout << "Time taken: "<< timeTaken/1000.0 << " ms" << '\n';
}

int main() {
    readInput();
    phi_1(), phi_2(), phi_3(), hint();
    S.solve();
    finish = chrono::steady_clock::now();
    printSolution();
}