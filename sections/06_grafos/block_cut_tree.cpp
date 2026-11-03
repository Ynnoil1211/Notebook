struct BlockCutTree {
    struct Edge {
        int to, id;
    };
    int n, m = 0;
    vector<vector<Edge>> adj;
    vector<vi> bct_adj, blocks;
    vi dfn, low, is_cut, bct_id;
    vector<pair<pii, int>> st;
    int timer = 0, bct_nodes = 0;
    vector<vector<pii>> biconn_edges;

    BlockCutTree(int n)
        : n(n), adj(n + 1), dfn(n + 1, 0), low(n + 1, 0),
          is_cut(n + 1, 0), bct_id(n + 1, 0) {}

    void add_edge(int u, int v) {
        if (u == v) return;
        ++m;
        adj[u].push_back({v, m});
        adj[v].push_back({u, m});
    }
    void dfs(int u, int p_edge = 0) {
        dfn[u] = low[u] = ++timer;
        if (adj[u].empty()) {
            blocks.push_back({u});
            return;
        }
        int children = 0;
        for (auto [v, id] : adj[u]) {
            if (id == p_edge) continue;
            if (dfn[v]) {
                low[u] = min(low[u], dfn[v]);
                if (dfn[v] < dfn[u])
                    st.push_back({{u, v}, id});
            } else {
                children++;
                st.push_back({{u, v}, id});
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    if (p_edge != 0 || children > 1)
                        is_cut[u] = 1;
                    biconn_edges.push_back({});
                    vi comp_v;
                    while (true) {
                        auto [edge, eid] = st.back();
                        st.pop_back();
                        biconn_edges.back().push_back(edge);
                        comp_v.push_back(edge.fi);
                        comp_v.push_back(edge.se);
                        if (eid == id) break;
                    }
                    sort(all(comp_v));
                    comp_v.erase(unique(all(comp_v)),
                                 comp_v.end());
                    blocks.push_back(comp_v);
                }
            }
        }
    }
    void build() {
        for (int i = 1; i <= n; ++i)
            if (!dfn[i]) dfs(i);
        int cuts = 0;
        for (int i = 1; i <= n; ++i)
            if (is_cut[i]) bct_id[i] = ++cuts;
        bct_nodes = cuts;
        bct_adj.assign(cuts + sz(blocks) + 1, {});

        for (const auto& verts : blocks) {
            int block_node = ++bct_nodes;
            for (int u : verts) {
                if (is_cut[u]) {
                    bct_adj[bct_id[u]].push_back(
                        block_node);
                    bct_adj[block_node].push_back(
                        bct_id[u]);
                } else bct_id[u] = block_node;
            }
        }
    }
};
