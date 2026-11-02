// 2. Prefix Sum 2D (Submatrices 1-based [r1..r2][c1..c2])
struct PrefixSum2D {
  vector<vll> p;
  PrefixSum2D(const vector<vll>& mat) {
    int n = sz(mat), m = n ? sz(mat[0]) : 0;
    p.assign(n + 1, vll(m + 1, 0));
    for (int i = 1; i <= n; ++i)
      for (int j = 1; j <= m; ++j)
        p[i][j] = mat[i - 1][j - 1] + p[i - 1][j]
                + p[i][j - 1] - p[i - 1][j - 1];
  }
  ll query(int r1, int c1, int r2, int c2) const {
    if (r1 > r2 || c1 > c2) return 0;
    return p[r2][c2] - p[r1 - 1][c2]
         - p[r2][c1 - 1] + p[r1 - 1][c1 - 1];
  }
};
