// 1. Dinic (Max Flow + Min Cut + Bipartite Matching en O(E
// sqrt(V)))
struct Dinic {
    struct Edge {
        int to, rev;
        ll cap, flow = 0;
    };
    int n, s, t;
    vector<vector<Edge>> g;
    vi lvl, ptr;

    Dinic(int n, int s, int t)
        : n(n), s(s), t(t), g(n + 1), lvl(n + 1),
          ptr(n + 1) {}
    void add_edge(int u, int v, ll cap) {
        g[u].pb({v, sz(g[v]), cap, 0});
        // Cambiar 0 por cap si es no dirigido
        g[v].pb({u, sz(g[u]) - 1, 0, 0});
    }
    bool bfs() {
        fill(all(lvl), -1);
        queue<int> q;
        lvl[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& e : g[u])
                if (e.cap - e.flow > 0 && lvl[e.to] == -1) {
                    lvl[e.to] = lvl[u] + 1;
                    q.push(e.to);
                }
        }
        return lvl[t] != -1;
    }
    ll dfs(int u, ll pushed) {
        if (pushed == 0 || u == t) return pushed;
        for (int& cid = ptr[u]; cid < sz(g[u]); ++cid) {
            auto& e = g[u][cid];
            if (lvl[u] + 1 != lvl[e.to] ||
                e.cap - e.flow == 0)
                continue;
            ll tr = dfs(e.to, min(pushed, e.cap - e.flow));
            if (tr == 0) continue;
            e.flow += tr;
            g[e.to][e.rev].flow -= tr;
            return tr;
        }
        return 0;
    }
    ll max_flow() {
        ll flow = 0;
        while (bfs()) {
            fill(all(ptr), 0);
            while (ll pushed = dfs(s, INF)) flow += pushed;
        }
        return flow;
    }
    // Retorna true si 'u' queda del lado de la fuente S en
    // el
    // Min-Cut
    // (Llamar DESPUES de max_flow()):
    bool left_of_min_cut(int u) const {
        return lvl[u] != -1;
    }
};
