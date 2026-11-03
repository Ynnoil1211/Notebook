// 2. DSU con Rollback + Dynamic Connectivity Offline
struct RollbackDSU {
    vi p, r, hist;
    int comp;
    RollbackDSU(int n) : p(n + 1), r(n + 1, 1), comp(n) {
        iota(all(p), 0);
    }
    int find(int i) const {
        return (p[i] == i) ? i : find(p[i]);
    }
    void unite(int i, int j) {
        i = find(i);
        j = find(j);
        if (i == j) return;
        if (r[i] > r[j]) swap(i, j);
        p[i] = j;
        r[j] += r[i];
        hist.pb(i);
        comp--;
    }
    int snapshot() const { return sz(hist); }
    void rollback(int snap) {
        while (sz(hist) > snap) {
            int i = hist.back();
            hist.pop_back();
            r[p[i]] -= r[i];
            p[i] = i;
            comp++;
        }
    }
};

struct DynCon {
    enum { ADD, DEL, QUERY };
    struct Ev {
        int type, u, v;
    };
    vector<Ev> q;
    RollbackDSU dsu;
    vi mt, ans;
    map<pii, int> active;

    DynCon(int n) : dsu(n) {}
    void add_edge(int u, int v) {
        if (u > v) swap(u, v);
        active[{u, v}] = sz(q);
        q.pb({ADD, u, v});
        mt.pb(-1);
    }
    void rem_edge(int u, int v) {
        if (u > v) swap(u, v);
        int pr = active[{u, v}];
        mt[pr] = sz(q);
        q.pb({DEL, u, v});
        mt.pb(pr);
    }
    void add_query() {
        q.pb({QUERY, -1, -1});
        mt.pb(-1);
    }

    void go(int l, int r) {
        if (l + 1 == r) {
            if (q[l].type == QUERY) ans.pb(dsu.comp);
            return;
        }
        int snap = dsu.snapshot(), mid = (l + r) / 2;
        for (int i = mid; i < r; ++i)
            if (mt[i] != -1 && mt[i] < l)
                dsu.unite(q[i].u, q[i].v);
        go(l, mid);
        dsu.rollback(snap);
        for (int i = l; i < mid; ++i)
            if (mt[i] >= r) dsu.unite(q[i].u, q[i].v);
        go(mid, r);
        dsu.rollback(snap);
    }
    vi solve() {
        if (q.empty()) return {};
        for (int i = 0; i < sz(q); ++i)
            if (q[i].type == ADD && mt[i] == -1)
                mt[i] = sz(q);
        go(0, sz(q));
        return ans;
    }
};
