// 2. Puentes y Puntos de Articulacion en Grafo No Dirigido
// (Tarjan)
struct BridgesAndCuts {
  int n, timer = 0;
  vi tin, low;
  vector<bool> is_cut;
  vector<pii> bridges;

  BridgesAndCuts(int n, const vector<vi>& adj)
    : n(n), tin(n + 1, 0), low(n + 1, 0), is_cut(n + 1,
      false) {
    for (int i = 1; i <= n; ++i)
      if (!tin[i]) dfs(i, -1, adj);
  }
  void dfs(int u, int p, const vector<vi>& adj) {
    tin[u] = low[u] = ++timer;
    int children = 0;
    bool skipped_parent = false;
    for (int v : adj[u]) {
      if (v == u) continue;
      if (v == p && !skipped_parent) {
        skipped_parent = true; continue; }
      if (tin[v]) {
        low[u] = min(low[u], tin[v]);
      } else {
        children++;
        dfs(v, u, adj);
        low[u] = min(low[u], low[v]);
        if (low[v] > tin[u]) bridges.pb({u, v});
        if (p != -1 && low[v] >= tin[u]) is_cut[u] = true;
      }
    }
    if (p == -1 && children > 1) is_cut[u] = true;
  }
};
