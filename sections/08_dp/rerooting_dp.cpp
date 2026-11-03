// Rerooting DP: Suma de distancias desde cada nodo u a
// todos los demas
// 1) dfs_down: calcula sz[u] y down[u] (contribucion del
// subarbol de u)
// 2) dfs_up: al mover la raiz de u a su hijo v, los sz[v]
// nodos en el
// subarbol de v se acercan 1 paso y los (n - sz[v]) fuera
// se alejan 1.
vll rerooting_sum_distances(int n, const vector<vi>& adj) {
    vi sub_sz(n + 1, 0);
    vll down(n + 1, 0), ans(n + 1, 0);

    auto dfs_down = [&](auto& self, int u, int p) -> void {
        sub_sz[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue;
            self(self, v, u);
            sub_sz[u] += sub_sz[v];
            down[u] += down[v] + sub_sz[v];
        }
    };
    auto dfs_up = [&](auto& self, int u, int p) -> void {
        for (int v : adj[u]) {
            if (v == p) continue;
            ans[v] = ans[u] + (n - sub_sz[v]) - sub_sz[v];
            self(self, v, u);
        }
    };
    if (n >= 1) {
        dfs_down(dfs_down, 1, 0);
        ans[1] = down[1];
        dfs_up(dfs_up, 1, 0);
    }
    return ans;  // ans[1..n]
}
