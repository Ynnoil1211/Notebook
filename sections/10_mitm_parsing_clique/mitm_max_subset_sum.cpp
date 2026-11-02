// 2. Mayor suma de subconjunto <= limit (usando two
// pointers en O(2^(N/2)))
ll mitm_max_subset_sum_le(const vll& a, ll limit) {
  int n = sz(a), mid = n / 2;
  vll L = get_subset_sums(a, 0, mid);
  vll R = get_subset_sums(a, mid, n);
  ll best = -INF;
  int j = sz(R) - 1;
  for (ll x : L) {
    while (j >= 0 && x + R[j] > limit) --j;
    if (j >= 0) best = max(best, x + R[j]);
  }
  return best;
}
