// 2. DP de Caminos / TSP con dp[mask][last] en O(N^2 * 2^N)
//    y reconstruccion del orden de nodos visitados.
pair<ll, vi> tsp_subset_dp(const vector<vll>& dist,
                           int start = 0,
                           bool close_cycle = true) {
  int n = sz(dist), full = (1 << n);
  vector<vll> dp(full, vll(n, INF));
  vector<vi> par(full, vi(n, -1));
  dp[1 << start][start] = 0;

  for (int mask = 1; mask < full; ++mask) {
    for (int u = 0; u < n; ++u) {
      if (!(mask & (
        1 << u)) || dp[mask][u] >= INF) continue;
      for (int v = 0; v < n; ++v) {
        if ((mask & (
          1 << v)) || dist[u][v] >= INF) continue;
        int nxt = mask | (1 << v);
        ll cand = dp[mask][u] + dist[u][v];
        if (cand < dp[nxt][v]) {
          dp[nxt][v] = cand;
          par[nxt][v] = u;
        }
      }
    }
  }
  ll best = INF;
  int last = -1, all_m = full - 1;
  for (int u = 0; u < n; ++u) {
    if (dp[all_m][u] >= INF) continue;
    ll extra = close_cycle ? dist[u][start] : 0;
    if (extra < INF && dp[all_m][u] + extra < best) {
      best = dp[all_m][u] + extra;
      last = u;
    }
  }
  if (last == -1) return {INF, {}};
  vi path;
  for (int m = all_m, cur = last; cur != -1; ) {
    path.pb(cur);
    int p = par[m][cur];
    m ^= (1 << cur);
    cur = p;
  }
  reverse(all(path));
  return {best, path};
}
