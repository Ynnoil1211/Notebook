// 4. Centroid Decomposition (Arbol de Centroides en O(N log
// N))
struct CentroidDecomp {
  int n;
  vector<vi> adj;
  vi sz_c, par_c;
  vector<bool> rem;

  CentroidDecomp(int n, const vector<vi>& g)
    : n(n), adj(g), sz_c(n + 1, 0), par_c(n + 1, 0), rem(
      n + 1, false) {
    build(1, 0);
  }
  int dfs_sz(int u, int p) {
    sz_c[u] = 1;
    for (int v : adj[u])
      if (v != p && !rem[v]) sz_c[u] += dfs_sz(v, u);
    return sz_c[u];
  }
  int get_centroid(int u, int p, int tot) const {
    for (int v : adj[u])
      if (v != p && !rem[v] && sz_c[v] * 2 > tot)
        return get_centroid(v, u, tot);
    return u;
  }
  void build(int u, int p) {
    int tot = dfs_sz(u, 0);
    int c = get_centroid(u, 0, tot);
    // Padre en el arbol de centroides (0 si es la raiz)
    par_c[c] = p;
    rem[c] = true;
    for (int v : adj[c])
      if (!rem[v]) build(v, c);
  }
};
