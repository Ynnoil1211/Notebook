// 3. Difference Array 2D (Sumar 'val' al rectangulo
// [r1..r2][c1..c2])
struct DiffArray2D {
    int n, m;
    vector<vll> d;
    DiffArray2D(int n, int m)
        : n(n), m(m), d(n + 2, vll(m + 2, 0)) {}
    void add_rect(int r1, int c1, int r2, int c2, ll val) {
        d[r1][c1] += val;
        d[r1][c2 + 1] -= val;
        d[r2 + 1][c1] -= val;
        d[r2 + 1][c2 + 1] += val;
    }
    vector<vll> build() const {
        vector<vll> res(n + 1, vll(m + 1, 0));
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= m; ++j)
                res[i][j] = d[i][j] + res[i - 1][j] +
                            res[i][j - 1] -
                            res[i - 1][j - 1];
        return res;
    }
};
