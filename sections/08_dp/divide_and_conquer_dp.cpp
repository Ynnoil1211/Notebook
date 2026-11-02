// 2. Divide and Conquer DP Optimization en O(K * N log N)
vll dp_prev, dp_cur;

template <typename CostFn>
void compute_dc(int l, int r, int optl, int optr,
  const CostFn& cost_fn) {
  if (l > r) return;
  int mid = (l + r) / 2;
  pair<ll, int> best = {INF, optl};
  int hi = min(mid, optr);
  for (int j = optl; j <= hi; ++j) {
    if (dp_prev[j - 1] >= INF) continue;
    ll cand = dp_prev[j - 1] + cost_fn(j, mid);
    if (cand < best.fi) best = {cand, j};
  }
  dp_cur[mid] = best.fi;
  int opt = best.se;
  compute_dc(l, mid - 1, optl, (best.fi < INF ? opt : hi),
    cost_fn);
  compute_dc(mid + 1, r, (best.fi < INF ? opt : optl), optr,
    cost_fn);
}

// Driver completo de D&C DP: particionar 1..n en
// exactamente k grupos (1-indexed)
template <typename CostFn>
ll solve_dc_dp(int n, int k, const CostFn& cost_fn) {
  dp_prev.assign(n + 1, INF);
  dp_cur.assign(n + 1, INF);
  dp_prev[0] = 0;
  for (int step = 1; step <= k; ++step) {
    fill(all(dp_cur), INF);
    compute_dc(step, n, step, n, cost_fn);
    dp_prev = dp_cur;
  }
  return dp_prev[n];
}
