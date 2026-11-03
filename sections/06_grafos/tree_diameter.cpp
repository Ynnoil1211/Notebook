// 1. Diametro de un Arbol (Doble BFS con reconstruccion de
// ruta)
vi adj_tr[200005];
int par_tr[200005];

pii bfs_farthest(int start, int n) {
    vi d(n + 1, -1);
    queue<int> q;
    q.push(start);
    d[start] = 0;
    par_tr[start] = -1;
    int far = start;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (d[u] > d[far]) far = u;
        for (int v : adj_tr[u]) {
            if (d[v] == -1) {
                d[v] = d[u] + 1;
                par_tr[v] = u;
                q.push(v);
            }
        }
    }
    return {far, d[far]};
}
vi get_diameter_path(int n) {
    pii p1 = bfs_farthest(1, n);
    pii p2 = bfs_farthest(p1.fi, n);
    vi path;
    for (int cur = p2.fi; cur != -1; cur = par_tr[cur])
        path.pb(cur);
    reverse(all(path));
    // Longitud del diametro en aristas = sz(path) - 1
    return path;
}
