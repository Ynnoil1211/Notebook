// 2b. Kuhn (Matching Bipartito en O(N1 * E)) +
// Reconstruccion de Konig (Min Vertex Cover)
// Lado izquierdo: 1..n1, Lado derecho: 1..n2.
struct BipartiteMVC {
  int n1, n2;
  vector<vi> adj;
  vi mt_l, mt_r, vis_l, vis_r;
  int timer = 0;

  BipartiteMVC(int n1, int n2)
    : n1(n1), n2(n2), adj(n1 + 1), mt_l(n1 + 1, 0),
      mt_r(n2 + 1, 0), vis_l(n1 + 1, 0), vis_r(n2 + 1, 0) {}

  // u in [1..n1], v in [1..n2]
  void add_edge(int u, int v) { adj[u].pb(v); }

  bool dfs(int u) {
    if (vis_l[u] == timer) return false;
    vis_l[u] = timer;
    for (int v : adj[u]) {
      if (!mt_r[v] || dfs(mt_r[v])) {
        mt_l[u] = v; mt_r[v] = u;
        return true;
      }
    }
    return false;
  }
  int max_matching() {
    int matched = 0;
    for (int u = 1; u <= n1; ++u)
      if (!mt_l[u]) { ++timer; if (dfs(u)) matched++; }
    return matched;
  }
  void dfs_konig(int u) {
    vis_l[u] = 1;
    for (int v : adj[u]) {
      if (!vis_r[v] && mt_l[u] != v) {
        vis_r[v] = 1;
        if (mt_r[v] && !vis_l[mt_r[v]]) dfs_konig(mt_r[v]);
      }
    }
  }
  // Retorna {nodos_izquierda_en_MVC, nodos_derecha_en_MVC}
  // (Teorema de Konig)
  pair<vi, vi> min_vertex_cover() {
    max_matching();
    fill(all(vis_l), 0); fill(all(vis_r), 0);
    for (int u = 1; u <= n1; ++u)
      if (!mt_l[u]) dfs_konig(u);
    vi cov_l, cov_r;
    for (int u = 1; u <= n1; ++u) if (!vis_l[u]) cov_l.pb(
      u);
    for (int v = 1; v <= n2; ++v) if (vis_r[v]) cov_r.pb(v);
    return {cov_l, cov_r};
  }
};
