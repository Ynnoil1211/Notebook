// 3. Tarjan SCC (Componentes Fuertemente Conexas en Grafo
// Dirigido)
// Nota: comp_id[u] queda en orden topologico INVERSO
// (scc_cnt..1)
struct TarjanSCC {
  int n, timer = 0, scc_cnt = 0;
  vi tin, low, comp_id, st;
  vector<bool> in_st;

  TarjanSCC(int n, const vector<vi>& adj)
    : n(n), tin(n + 1, 0), low(n + 1, 0),
      comp_id(n + 1, 0), in_st(n + 1, false) {
    for (int i = 1; i <= n; ++i)
      if (!tin[i]) dfs(i, adj);
  }
  void dfs(int u, const vector<vi>& adj) {
    tin[u] = low[u] = ++timer;
    st.pb(u); in_st[u] = true;
    for (int v : adj[u]) {
      if (!tin[v]) {
        dfs(v, adj);
        low[u] = min(low[u], low[v]);
      } else if (in_st[v]) {
        low[u] = min(low[u], tin[v]);
      }
    }
    if (low[u] == tin[u]) {
      scc_cnt++;
      while (true) {
        int v = st.back(); st.pop_back();
        in_st[v] = false;
        comp_id[v] = scc_cnt;
        if (u == v) break;
      }
    }
  }
};
