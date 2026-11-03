// 3. LCA (Lowest Common Ancestor) + K-th Ancestor +
// Distancia en Arbol
struct LCA {
    int n, LOG;
    vi dep;
    vector<vi> up;

    LCA(int n, int root, const vector<vi>& adj) : n(n) {
        LOG = __lg(max(1, n)) + 1;
        dep.assign(n + 1, 0);
        up.assign(n + 1, vi(LOG, root));
        dfs(root, root, adj);
    }
    void dfs(int u, int p, const vector<vi>& adj) {
        up[u][0] = p;
        for (int j = 1; j < LOG; ++j)
            up[u][j] = up[up[u][j - 1]][j - 1];
        for (int v : adj[u]) {
            if (v == p) continue;
            dep[v] = dep[u] + 1;
            dfs(v, u, adj);
        }
    }
    int kth_ancestor(int u, int k) const {
        for (int j = 0; j < LOG; ++j)
            if ((k >> j) & 1) u = up[u][j];
        return u;
    }
    int lca(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);
        u = kth_ancestor(u, dep[u] - dep[v]);
        if (u == v) return u;
        for (int j = LOG - 1; j >= 0; --j)
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        return up[u][0];
    }
    int dist(int u, int v) const {
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
    }
};
