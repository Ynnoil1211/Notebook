// 2. Segment Tree Recursivo Clasico (0-indexed)
struct SegTree {
    int n;
    const ll NEUTRAL = 0;
    vll t;
    ll merge(ll a, ll b) const { return a + b; }

    SegTree(const vll& a)
        : n(sz(a)), t(4 * sz(a), NEUTRAL) {
        if (n > 0) build(1, 0, n - 1, a);
    }
    void build(int v, int l, int r, const vll& a) {
        if (l == r) {
            t[v] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(v * 2, l, m, a);
        build(v * 2 + 1, m + 1, r, a);
        t[v] = merge(t[v * 2], t[v * 2 + 1]);
    }
    void update(int v, int l, int r, int pos, ll val) {
        if (l == r) {
            t[v] = val;
            return;
        }
        int m = (l + r) / 2;
        if (pos <= m) update(v * 2, l, m, pos, val);
        else update(v * 2 + 1, m + 1, r, pos, val);
        t[v] = merge(t[v * 2], t[v * 2 + 1]);
    }
    ll query(int v, int l, int r, int ql, int qr) const {
        if (qr < l || r < ql) return NEUTRAL;
        if (ql <= l && r <= qr) return t[v];
        int m = (l + r) / 2;
        return merge(query(v * 2, l, m, ql, qr),
                     query(v * 2 + 1, m + 1, r, ql, qr));
    }
    void update(int pos, ll val) {
        update(1, 0, n - 1, pos, val);
    }
    ll query(int ql, int qr) const {
        return query(1, 0, n - 1, ql, qr);
    }
};
