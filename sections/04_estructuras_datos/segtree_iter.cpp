// 1. Segment Tree Iterativo (0-indexed, ocupa solo 2*N de
// memoria)
struct IterSegTree {
    int n;
    // INF para min, -INF para max, 0 para suma
    const ll NEUTRAL = INF;
    vll t;
    ll op(ll a, ll b) const { return min(a, b); }

    IterSegTree(const vll& a)
        : n(sz(a)), t(2 * sz(a), NEUTRAL) {
        for (int i = 0; i < n; ++i) t[n + i] = a[i];
        for (int i = n - 1; i >= 1; --i)
            t[i] = op(t[i << 1], t[i << 1 | 1]);
    }
    // Asigna a[p] = val (0-indexed)
    void update(int p, ll val) {
        for (t[p += n] = val; p > 1; p >>= 1)
            t[p >> 1] = op(t[p], t[p ^ 1]);
    }
    // Rango CERRADO 0-indexed [l, r]
    ll query(int l, int r) const {
        ll vl = NEUTRAL, vr = NEUTRAL;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) vl = op(vl, t[l++]);
            if (r & 1) vr = op(t[--r], vr);
        }
        return op(vl, vr);
    }
};
