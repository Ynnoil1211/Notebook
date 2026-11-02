// 6. Logaritmo Discreto (Baby-Step Giant-Step): menor x >=
// 0 con a^x = b (mod m)
ll discrete_log(ll a, ll b, ll m) {
  a %= m; b %= m;
  ll k = 1, add = 0, g;
  while ((g = std::gcd(a, m)) > 1) {
    if (b == k) return add;
    if (b % g != 0) return -1;
    b /= g; m /= g; ++add;
    k = (k * a / g) % m;
  }
  ll n = sqrt(m) + 1, an = 1;
  for (int i = 0; i < n; ++i) an = (an * a) % m;
  safe_umap<ll, ll> vals;
  for (ll q = 0, cur = b; q <= n; ++q) {
    vals[cur] = q;
    cur = (cur * a) % m;
  }
  for (ll p = 1, cur = k; p <= n; ++p) {
    cur = (cur * an) % m;
    if (vals.find(cur) != vals.end())
      return n * p - vals[cur] + add;
  }
  return -1;
}
