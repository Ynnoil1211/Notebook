// 3. Persistent Segment Tree (Suma en versiones historicas)
struct PersistentST {
    struct Node {
        ll val;
        int l, r;
    };
    int n;
    vector<Node> t;
    vi roots;

    PersistentST(const vll& a) : n(sz(a)) {
        t.reserve(n * 22);
        roots.pb(build(0, n - 1, a));
    }
    int build(int tl, int tr, const vll& a) {
        int v = sz(t);
        t.pb({0, 0, 0});
        if (tl == tr) {
            t[v].val = a[tl];
            return v;
        }
        int tm = (tl + tr) / 2;
        int left = build(tl, tm, a);
        int right = build(tm + 1, tr, a);
        t[v] = {t[left].val + t[right].val, left, right};
        return v;
    }
    int update(int prv, int tl, int tr, int pos,
               ll new_val) {
        int v = sz(t);
        t.pb(t[prv]);
        if (tl == tr) {
            t[v].val = new_val;
            return v;
        }
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            int nl = update(t[prv].l, tl, tm, pos, new_val);
            t[v].l = nl;
        } else {
            int nr =
                update(t[prv].r, tm + 1, tr, pos, new_val);
            t[v].r = nr;
        }
        t[v].val = t[t[v].l].val + t[t[v].r].val;
        return v;
    }
    ll query(int v, int tl, int tr, int ql, int qr) const {
        if (qr < tl || tr < ql) return 0;
        if (ql <= tl && tr <= qr) return t[v].val;
        int tm = (tl + tr) / 2;
        return query(t[v].l, tl, tm, ql, qr) +
               query(t[v].r, tm + 1, tr, ql, qr);
    }
    // Crea una nueva version a partir de 'ver' y retorna su
    // ID
    int create_version(int ver, int pos, ll val) {
        int new_root =
            update(roots[ver], 0, n - 1, pos, val);
        roots.pb(new_root);
        return sz(roots) - 1;
    }
    ll query_version(int ver, int ql, int qr) const {
        return query(roots[ver], 0, n - 1, ql, qr);
    }
};
