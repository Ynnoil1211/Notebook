// 1. Heavy-Light Decomposition (HLD) + Segment Tree
// Iterativo
struct HLD {
    int n, timer = 0;
    vector<vi> adj;
    vi par, dep, heavy, head, pos, sz_sub;
    IterSegTree st;

    HLD(int n, int root, const vector<vi>& g,
        const vll& vals)
        : n(n), adj(g), par(n + 1), dep(n + 1, 0),
          heavy(n + 1, -1), head(n + 1), pos(n + 1),
          sz_sub(n + 1, 1), st(vll(n, 0)) {
        par[root] = root;
        dfs_sz(root);
        dfs_hld(root, root);
        for (int u = 1; u <= n; ++u)
            st.update(pos[u], vals[u]);
    }
    void dfs_sz(int u) {
        int max_c = 0;
        for (int v : adj[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            dep[v] = dep[u] + 1;
            dfs_sz(v);
            sz_sub[u] += sz_sub[v];
            if (sz_sub[v] > max_c) {
                max_c = sz_sub[v];
                heavy[u] = v;
            }
        }
    }
    void dfs_hld(int u, int h) {
        head[u] = h;
        pos[u] = timer++;
        if (heavy[u] != -1) dfs_hld(heavy[u], h);
        for (int v : adj[u])
            if (v != par[u] && v != heavy[u]) dfs_hld(v, v);
    }
    void update_node(int u, ll val) {
        st.update(pos[u], val);
    }

    // Consulta en el camino entre u y v (minimo en este
    // ejemplo)
    ll query_path(int u, int v) const {
        ll res = st.NEUTRAL;
        for (; head[u] != head[v]; v = par[head[v]]) {
            if (dep[head[u]] > dep[head[v]]) swap(u, v);
            res =
                st.op(res, st.query(pos[head[v]], pos[v]));
        }
        if (dep[u] > dep[v]) swap(u, v);
        res = st.op(res, st.query(pos[u], pos[v]));
        // NOTA: Si los pesos estan en las ARISTAS
        // (guardados en
        // el hijo),
        // cambiar la linea anterior por:
        // if (u != v) res = st.op(res, st.query(pos[u] + 1,
        // pos[v]));
        return res;
    }
    // Consulta en todo el subarbol de u en O(log N):
    ll query_subtree(int u) const {
        return st.query(pos[u], pos[u] + sz_sub[u] - 1);
    }
};
