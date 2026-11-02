// Genera las 2^k sumas de subconjuntos de un rango [l, r)
// en O(2^k)
vll get_subset_sums(const vll& a, int l, int r) {
  vll sums = {0};
  for (int i = l; i < r; ++i) {
    int cur_sz = sz(sums);
    sums.resize(2 * cur_sz);
    for (int j = 0; j < cur_sz; ++j)
      sums[cur_sz + j] = sums[j] + a[i];
    inplace_merge(sums.begin(), sums.begin() + cur_sz,
      sums.end());
  }
  return sums; // Ya queda ordenado en O(2^(r-l))!
}

// 1. Cuenta cuantos subconjuntos (con multiplicidad) suman
// 'target'
ll mitm_subset_sum_count(const vll& a, ll target) {
  int n = sz(a), mid = n / 2;
  vll L = get_subset_sums(a, 0, mid);
  vll R = get_subset_sums(a, mid, n);
  ll ways = 0;
  for (ll x : L) {
    auto [it1, it2] = equal_range(all(R), target - x);
    ways += (it2 - it1);
  }
  return ways;
}
