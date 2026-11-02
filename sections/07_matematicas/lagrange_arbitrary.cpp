// 2. Interpolacion de Lagrange en O(N^2) para puntos
// arbitrarios distintos (xs[i], ys[i])
ll lagrange_eval_arbitrary(const vll& xs, const vll& ys,
  ll x, ll mod = MOD) {
  int n = sz(xs);
  auto norm = [&](ll v) { v %= mod;
    return v < 0 ? v + mod : v; };
  auto bpow = [&](ll b, ll e) {
    ll r = 1;
    for (b = norm(b); e > 0; e >>= 1, b = b * b % mod)
      if (e & 1) r = r * b % mod;
    return r;
  };
  ll ans = 0;
  for (int i = 0; i < n; ++i) {
    ll num = 1, den = 1;
    for (int j = 0; j < n; ++j) {
      if (i == j) continue;
      num = num * norm(x - xs[j]) % mod;
      den = den * norm(xs[i] - xs[j]) % mod;
    }
    ll term = norm(ys[i]) * num % mod * bpow(den,
      mod - 2) % mod;
    ans = (ans + term) % mod;
  }
  return ans;
}
