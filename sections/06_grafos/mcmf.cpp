// 2. Min-Cost Max-Flow (MCMF con SPFA; requiere red sin
// ciclos negativos)
struct MCMF {
    struct Edge {
        int to, rev;
        ll cap, cost, flow = 0;
    };
    int n;
    vector<vector<Edge>> g;
    vll dist, cap_pushed;
    vi par_u, par_e;
    vector<bool> in_q;

    MCMF(int n)
        : n(n), g(n + 1), dist(n + 1), cap_pushed(n + 1),
          par_u(n + 1), par_e(n + 1), in_q(n + 1) {}

    void add_edge(int u, int v, ll cap, ll cost) {
        g[u].pb({v, sz(g[v]), cap, cost, 0});
        g[v].pb({u, sz(g[u]) - 1, 0, -cost, 0});
    }
    bool spfa(int s, int t) {
        fill(all(dist), INF);
        fill(all(in_q), false);
        queue<int> q;
        dist[s] = 0;
        cap_pushed[s] = INF;
        q.push(s);
        in_q[s] = true;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_q[u] = false;
            for (int i = 0; i < sz(g[u]); ++i) {
                auto& e = g[u][i];
                if (e.cap - e.flow > 0 &&
                    dist[u] + e.cost < dist[e.to]) {
                    dist[e.to] = dist[u] + e.cost;
                    par_u[e.to] = u;
                    par_e[e.to] = i;
                    cap_pushed[e.to] =
                        min(cap_pushed[u], e.cap - e.flow);
                    if (!in_q[e.to]) {
                        q.push(e.to);
                        in_q[e.to] = true;
                    }
                }
            }
        }
        return dist[t] != INF;
    }
    // Retorna {flujo_total, costo_minimo} enviando a lo
    // sumo
    // max_f
    pll solve(int s, int t, ll max_f = INF) {
        ll flow = 0, cost = 0;
        while (flow < max_f && spfa(s, t)) {
            ll f = min(max_f - flow, cap_pushed[t]);
            flow += f;
            cost += f * dist[t];
            for (int cur = t; cur != s; cur = par_u[cur]) {
                int p = par_u[cur], idx = par_e[cur];
                g[p][idx].flow += f;
                g[cur][g[p][idx].rev].flow -= f;
            }
        }
        return {flow, cost};
    }
};
