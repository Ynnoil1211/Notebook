// 5. Contar Triangulos (3-ciclos) en un grafo en O(M *
// sqrt(M))
ll count_triangles(int n, const vector<pii>& edges) {
    vi deg(n + 1, 0), mark(n + 1, 0);
    for (auto [u, v] : edges) {
        deg[u]++;
        deg[v]++;
    }
    vector<vi> dag(n + 1);
    for (auto [u, v] : edges) {
        if (tie(deg[u], u) > tie(deg[v], v)) swap(u, v);
        dag[u].pb(
            v);  // Orienta de menor grado a mayor grado
    }
    ll triangles = 0;
    for (int u = 1; u <= n; ++u) {
        for (int v : dag[u]) mark[v] = u;
        for (int v : dag[u])
            for (int w : dag[v])
                if (mark[w] == u) triangles++;
    }
    return triangles;
}
