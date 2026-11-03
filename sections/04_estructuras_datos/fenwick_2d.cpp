// 2. Fenwick Tree 2D (1-indexed: 1 <= r <= n, 1 <= c <= m)
struct Fenwick2D {
    int n, m;
    vector<vll> bit;
    Fenwick2D(int n, int m)
        : n(n), m(m), bit(n + 1, vll(m + 1, 0)) {}

    void add(int r, int c, ll delta) {
        for (int i = r; i <= n; i += i & -i)
            for (int j = c; j <= m; j += j & -j)
                bit[i][j] += delta;
    }
    ll query(int r, int c) const {  // Suma en [1..r][1..c]
        ll sum = 0;
        for (int i = r; i > 0; i -= i & -i)
            for (int j = c; j > 0; j -= j & -j)
                sum += bit[i][j];
        return sum;
    }
    ll query(int r1, int c1, int r2, int c2) const {
        return query(r2, c2) - query(r1 - 1, c2) -
               query(r2, c1 - 1) + query(r1 - 1, c1 - 1);
    }
};
