// 4. Persistente de Frecuencias: k-esimo menor en
// subarreglo [l..r] (0-indexed, k >= 1)
struct PersistentFreqST {
    struct Node {
        int cnt, l, r;
    };
    int m;  // Tamano del alfabeto de valores comprimidos
    vll vals;
    vector<Node> t;
    vi roots;

    PersistentFreqST(const vll& a) : vals(a) {
        sort(all(vals));
        vals.erase(unique(all(vals)), vals.end());
        m = max(1, sz(vals));
        t.reserve((sz(a) + 1) * 20);
        roots.pb(build(0, m - 1));
        for (ll x : a) {
            int pos =
                lower_bound(all(vals), x) - vals.begin();
            roots.pb(insert(roots.back(), 0, m - 1, pos));
        }
    }
    int build(int tl, int tr) {
        int v = sz(t);
        t.pb({0, 0, 0});
        if (tl == tr) return v;
        int tm = (tl + tr) / 2;
        int left = build(tl, tm), right = build(tm + 1, tr);
        t[v] = {0, left, right};
        return v;
    }
    int insert(int prv, int tl, int tr, int pos) {
        int v = sz(t);
        t.pb(t[prv]);
        t[v].cnt++;
        if (tl == tr) return v;
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            int nl = insert(t[prv].l, tl, tm, pos);
            t[v].l = nl;
        } else {
            int nr = insert(t[prv].r, tm + 1, tr, pos);
            t[v].r = nr;
        }
        return v;
    }
    int kth_idx(int vl, int vr, int tl, int tr,
                int k) const {
        if (tl == tr) return tl;
        int tm = (tl + tr) / 2;
        int left_cnt = t[t[vr].l].cnt - t[t[vl].l].cnt;
        if (k <= left_cnt)
            return kth_idx(t[vl].l, t[vr].l, tl, tm, k);
        return kth_idx(t[vl].r, t[vr].r, tm + 1, tr,
                       k - left_cnt);
    }
    // Retorna el k-esimo menor valor (1-indexed: 1 <= k <=
    // r
    // - l + 1) en a[l..r]
    ll query_kth(int l, int r, int k) const {
        return vals[kth_idx(roots[l], roots[r + 1], 0,
                            m - 1, k)];
    }
};
