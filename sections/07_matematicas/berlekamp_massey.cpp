// Berlekamp-Massey + Cayley-Hamilton para sucesiones
// modulares
struct BerlekampMassey {
  ll mod;
  BerlekampMassey(ll m = MOD) : mod(m) {}

  ll extgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) { x = 1; y = 0; return a; }
    ll d = extgcd(b, a % b, y, x);
    y -= (a / b) * x;
    return d;
  }
  ll norm(ll a) const {
    a %= mod;
    return (a < 0) ? (a + mod) : a;
  }
  ll inv(ll a) {
    ll x, y; extgcd(norm(a), mod, x, y);
    return norm(x);
  }

  // Requiere mod primo para que toda discrepancia no nula
  // sea invertible
  vll BM(const vll& x) {
    vll ls, cur;
    int lf = 0; ll ld = 0;
    for (int i = 0; i < sz(x); ++i) {
      ll t = 0;
      for (int j = 0; j < sz(cur); ++j)
        t = (t + norm(x[i - j - 1]) * cur[j]) % mod;
      ll d = norm(x[i] - t);
      if (d == 0) continue;
      if (cur.empty()) {
        cur.resize(i + 1, 0);
        lf = i; ld = d;
        continue;
      }
      ll k = d * inv(ld) % mod;
      vll c(i - lf - 1, 0); c.push_back(k);
      for (int j = 0; j < sz(ls); ++j)
        c.push_back(norm(-ls[j]) * k % mod);
      if (sz(c) < sz(cur)) c.resize(sz(cur), 0);
      for (int j = 0; j < sz(cur); ++j)
        c[j] = (c[j] + cur[j]) % mod;
      if (i - lf + sz(ls) >= sz(cur)) {
        ls = cur; lf = i; ld = d;
      }
      cur = c;
    }
    for (ll& val : cur) val = norm(val);
    return cur;
  }

  vll poly_mul(const vll& p, const vll& q, const vll& h,
    int m) {
    vll t(2 * m, 0);
    for (int i = 0; i < m; ++i) if (p[i])
      for (int j = 0; j < m; ++j)
        t[i + j] = (t[i + j] + p[i] * q[j]) % mod;
    for (int i = 2 * m - 1; i >= m; --i) if (t[i])
      for (int j = 0; j < m; ++j)
        t[i - j - 1] = (t[i - j - 1] + t[i] * h[j]) % mod;
    t.resize(m);
    return t;
  }

  ll get_kth(const vll& first_terms, ll k) {
    if (k < sz(first_terms)) return (
      first_terms[k] % mod + mod) % mod;
    vll h = BM(first_terms);
    int m = sz(h);
    if (!m) return 0;
    vll s(m, 0), t(m, 0);
    s[0] = 1;
    if (m != 1) t[1] = 1; else t[0] = h[0];
    for (; k > 0; k >>= 1) {
      if (k & 1) s = poly_mul(s, t, h, m);
      t = poly_mul(t, t, h, m);
    }
    ll ans = 0;
    for (int i = 0; i < m; ++i)
      ans = (ans + s[i] * first_terms[i]) % mod;
    return (ans % mod + mod) % mod;
  }
};
