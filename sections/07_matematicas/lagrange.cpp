// Interpolacion de Lagrange en O(D) para x = 0, 1, ..., D
// 'y' contiene P(0), P(1), ..., P(D). Evalua P(x) mod MOD.
ll lagrange_eval_consecutive(const vll& y, ll x,
  ll mod = MOD) {
  int n = sz(y); // Grado = n - 1
  if (x < n) return (y[x] % mod + mod) % mod;
  vll pref(n), suff(n), fact(n), inv_fact(n);
  pref[0] = x % mod;
  for (int i = 1; i < n; ++i) pref[i] = pref[i - 1] * ((
    x - i) % mod) % mod;
  suff[n - 1] = (x - (n - 1)) % mod;
  for (int i = n - 2; i >= 0; --i) suff[i] = suff[i + 1] * (
    (x - i) % mod) % mod;

  auto bpow = [&](ll b, ll e) {
    ll r = 1;
    for (; e > 0; e >>= 1, b = b * b % mod) if (
      e & 1) r = r * b % mod;
    return r;
  };
  fact[0] = 1;
  for (int i = 1; i < n; ++i) fact[i] = fact[i
    - 1] * i % mod;
  inv_fact[n - 1] = bpow(fact[n - 1], mod - 2);
  for (int i = n - 2; i >= 0; --i) inv_fact[i] = inv_fact[i
    + 1] * (i + 1) % mod;

  ll ans = 0;
  for (int i = 0; i < n; ++i) {
    ll num = 1;
    if (i > 0) num = num * pref[i - 1] % mod;
    if (i + 1 < n) num = num * suff[i + 1] % mod;
    ll den = inv_fact[i] * inv_fact[n - 1 - i] % mod;
    if ((n - 1 - i) & 1) den = mod - den;
    ans = (ans + y[i] * num % mod * den) % mod;
  }
  return (ans % mod + mod) % mod;
}
