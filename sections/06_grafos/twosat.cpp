// 4. 2-SAT Solver en O(N + M) (Variables 0-indexed: 0..n-1)
struct TwoSAT {
    int n;
    vector<vi> adj;
    vector<bool> ans;

    TwoSAT(int n) : n(n), adj(2 * n + 1), ans(n, false) {}
    // Nodo para variable 'x' (0..n-1):
    // Si val == true -> nodo (x + 1); si val == false ->
    // nodo
    // (x + 1 + n)
    int var_node(int x, bool val) const {
        return val ? (x + 1) : (x + 1 + n);
    }
    int neg(int u) const {
        return (u <= n) ? (u + n) : (u - n);
    }

    void add_imp(int u, int v) {
        adj[u].pb(v);
        adj[neg(v)].pb(neg(u));
    }
    // (x == val_x) OR (y == val_y)
    void add_or(int x, bool vx, int y, bool vy) {
        int u = var_node(x, vx), v = var_node(y, vy);
        add_imp(neg(u), v);
    }
    void set_val(int x, bool vx) { add_or(x, vx, x, vx); }
    void make_eq(int x, int y) {
        add_or(x, true, y, false);
        add_or(x, false, y, true);
    }
    void make_diff(int x, int y) {
        add_or(x, true, y, true);
        add_or(x, false, y, false);
    }
    bool solve() {
        TarjanSCC scc(2 * n, adj);
        for (int i = 0; i < n; ++i) {
            int u = i + 1, nu = i + 1 + n;
            if (scc.comp_id[u] == scc.comp_id[nu])
                return false;
            // En Tarjan, menor comp_id significa mas abajo
            // en el
            // DAG (sink)
            ans[i] = (scc.comp_id[u] < scc.comp_id[nu]);
        }
        return true;
    }
};
