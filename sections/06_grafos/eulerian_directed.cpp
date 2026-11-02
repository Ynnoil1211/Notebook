// Hierholzer Canónico para Grafo Dirigido con multiaristas
// (1-indexed)
// Nota: find_tour() reinicia cur_edge y used_edge, por lo
// que admite llamadas repetidas.
struct EulerianDirected {
  int n, m = 0;
  vector<vector<pii>> adj; // {v, edge_id}
  vi in_deg, out_deg, cur_edge;
  vector<bool> used_edge;

  EulerianDirected(int n) : n(n), adj(n + 1), in_deg(n + 1,
    0),
    out_deg(n + 1, 0), cur_edge(n + 1, 0) {}

  void add_edge(int u, int v) {
    adj[u].push_back({v, ++m});
    out_deg[u]++; in_deg[v]++;
  }
  vi find_tour() {
    if (m == 0) return {1};
    fill(all(cur_edge), 0);
    used_edge.assign(m + 1, false);
    int start = 1, one_out = 0, one_in = 0;
    for (int i = 1; i <= n; ++i) {
      if (out_deg[i] - in_deg[i] == 1) { one_out++;
        start = i; }
      else if (in_deg[i] - out_deg[i] == 1) one_in++;
      // Imposible
      else if (in_deg[i] != out_deg[i]) return {};
    }
    if (!(one_out == 0 && one_in == 0) && !(
      one_out == 1 && one_in == 1))
      return {};

    for (int i = 1; i <= n; ++i)
      if (out_deg[i] > 0 && one_out == 0) { start = i;
        break; }

    vi path, st = {start};
    while (!st.empty()) {
      int u = st.back();
      while (cur_edge[u] < sz(
        adj[u]) && used_edge[adj[u][cur_edge[u]].second])
        cur_edge[u]++;
      if (cur_edge[u] < sz(adj[u])) {
        auto [v, id] = adj[u][cur_edge[u]++];
        used_edge[id] = true;
        st.push_back(v);
      } else {
        path.push_back(u);
        st.pop_back();
      }
    }
    reverse(all(path));
    if (sz(path) != m + 1) return {}; // Grafo desconectado
    return path;
  }
};
