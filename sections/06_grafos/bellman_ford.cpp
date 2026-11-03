// 2. Bellman-Ford con Reconstruccion de Ciclo Negativo
// Alcanzable
// (Si quieres detectar CUALQUIER ciclo negativo en el grafo
// aunque no sea
// alcanzable desde 's', inicializa todos los d[1..n] = 0 en
// vez de INF).
struct BFEdge {
    int u, v;
    ll w;
};
bool bellman_ford_cycle(int s, int n,
                        const vector<BFEdge>& edges, vll& d,
                        vi& neg_cycle) {
    d.assign(n + 1, INF);
    vi p(n + 1, -1);
    d[s] = 0;
    int x = -1;
    for (int i = 1; i <= n; ++i) {
        x = -1;
        for (const auto& e : edges) {
            if (d[e.u] >= INF) continue;
            ll nd = max(-INF, d[e.u] + e.w);
            if (nd < d[e.v]) {
                d[e.v] = nd;
                p[e.v] = e.u;
                x = e.v;
            }
        }
    }
    if (x == -1) return false;  // No hay ciclo negativo
    // Entrar con certeza al ciclo
    for (int i = 0; i < n; ++i) x = p[x];
    neg_cycle.clear();
    for (int cur = x;; cur = p[cur]) {
        neg_cycle.pb(cur);
        if (cur == x && sz(neg_cycle) > 1) break;
    }
    reverse(all(neg_cycle));
    return true;
}
