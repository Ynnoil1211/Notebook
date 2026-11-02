struct FunctionalGraph {
  static const int LOG = 62; // Soporta K <= 2e18
  int n, cyc_cnt = 0;
  vi to, in_deg, dist_cyc, root_cyc, cyc_id, cyc_pos,
    cyc_len;
  vector<vi> up;

  FunctionalGraph(int n, const vi& nxt)
    : n(n), to(nxt), in_deg(n + 1, 0), dist_cyc(n + 1, 0),
      root_cyc(n + 1, 0), cyc_id(n + 1, 0), cyc_pos(n + 1,
        0),
      cyc_len(n + 1, 0), up(LOG, vi(n + 1, 0)) {
    for (int i = 1; i <= n; ++i) {
      up[0][i] = to[i];
      in_deg[to[i]]++;
    }
    for (int j = 1; j < LOG; ++j)
      for (int i = 1; i <= n; ++i)
        up[j][i] = up[j - 1][up[j - 1][i]];

    queue<int> q;
    vector<bool> on_cyc(n + 1, true);
    for (int i = 1; i <= n; ++i)
      if (in_deg[i] == 0) { q.push(i); on_cyc[i] = false; }
    while (!q.empty()) {
      int u = q.front(); q.pop();
      int v = to[u];
      if (--in_deg[v] == 0) {
        on_cyc[v] = false;
        q.push(v);
      }
    }
    for (int i = 1; i <= n; ++i) {
      if (on_cyc[i] && !cyc_id[i]) {
        ++cyc_cnt;
        int cur = i, pos = 0;
        while (!cyc_id[cur]) {
          cyc_id[cur] = cyc_cnt;
          cyc_pos[cur] = pos++;
          root_cyc[cur] = cur;
          cur = to[cur];
        }
        cyc_len[cyc_cnt] = pos;
      }
    }
    for (int i = 1; i <= n; ++i)
      if (!on_cyc[i]) resolve_tree(i);
  }
  void resolve_tree(int u) {
    if (cyc_id[u]) return;
    int v = to[u];
    if (!cyc_id[v]) resolve_tree(v);
    dist_cyc[u] = dist_cyc[v] + 1;
    root_cyc[u] = root_cyc[v];
    cyc_id[u] = cyc_id[v];
  }
  int jump(int u, ll k) const {
    for (int j = 0; j < LOG; ++j)
      if ((k >> j) & 1LL) u = up[j][u];
    return u;
  }
  // Minima cantidad de pasos para ir de u a v (-1 si
  // imposible)
  ll min_steps(int u, int v) const {
    if (cyc_id[u] != cyc_id[v]) return -1;
    if (dist_cyc[u] < dist_cyc[v]) return -1;
    ll d = dist_cyc[u] - dist_cyc[v];
    int u_anc = jump(u, d);
    if (dist_cyc[v] > 0) return (u_anc == v) ? d : -1;
    int len = cyc_len[cyc_id[v]];
    int diff = (cyc_pos[v] - cyc_pos[u_anc] + len) % len;
    return d + diff;
  }
};
