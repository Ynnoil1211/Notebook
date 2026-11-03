// 2. Segment Tree 2D Iterativo (0-indexed:
// [x0..x1][y0..y1])
struct SegTree2D {
    int n, m;
    const ll NEUTRAL = 0;
    vector<vll> st;
    ll op(ll a, ll b) const { return a + b; }

    SegTree2D(const vector<vll>& a)
        : n(sz(a)), m(n ? sz(a[0]) : 0),
          st(2 * n, vll(2 * m, 0)) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                st[i + n][j + m] = a[i][j];
        for (int i = 0; i < n; ++i)
            for (int j = m - 1; j > 0; --j)
                st[i + n][j] = op(st[i + n][j << 1],
                                  st[i + n][j << 1 | 1]);
        for (int i = n - 1; i > 0; --i)
            for (int j = 0; j < 2 * m; ++j)
                st[i][j] =
                    op(st[i << 1][j], st[i << 1 | 1][j]);
    }
    void update(int x, int y, ll v) {
        st[x + n][y + m] = v;
        for (int j = y + m; j > 1; j >>= 1)
            st[x + n][j >> 1] =
                op(st[x + n][j], st[x + n][j ^ 1]);
        for (int i = x + n; i > 1; i >>= 1)
            for (int j = y + m; j > 0; j >>= 1)
                st[i >> 1][j] = op(st[i][j], st[i ^ 1][j]);
    }
    ll query(int x0, int x1, int y0, int y1) const {
        ll res = NEUTRAL;
        for (int i0 = x0 + n, i1 = x1 + n + 1; i0 < i1;
             i0 >>= 1, i1 >>= 1) {
            int rows[2], q = 0;
            if (i0 & 1) rows[q++] = i0++;
            if (i1 & 1) rows[q++] = --i1;
            for (int k = 0; k < q; ++k)
                for (int j0 = y0 + m, j1 = y1 + m + 1;
                     j0 < j1; j0 >>= 1, j1 >>= 1) {
                    if (j0 & 1)
                        res = op(res, st[rows[k]][j0++]);
                    if (j1 & 1)
                        res = op(res, st[rows[k]][--j1]);
                }
        }
        return res;
    }
};
