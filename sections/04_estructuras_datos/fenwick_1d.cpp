// 1. Fenwick Tree 1D (1-indexed: 1 <= idx <= n)
struct Fenwick {
  int n;
  vll bit;
  Fenwick(int n) : n(n), bit(n + 1, 0) {}

  void add(int idx, ll delta) {
    for (; idx <= n; idx += idx & -idx) bit[idx] += delta;
  }
  ll query(int idx) const { // Suma en [1..idx]
    ll sum = 0;
    for (; idx > 0; idx -= idx & -idx) sum += bit[idx];
    return sum;
  }
  ll query(int l, int r) const { // Suma en [l..r]
    return (l > r) ? 0 : query(r) - query(l - 1);
  }
  // Menor indice idx tal que query(1..idx) >= target
  // (valores >= 0)
  int lower_bound(ll target) const {
    int idx = 0;
    ll cur = 0;
    for (int pw = 1 << __lg(max(1, n)); pw > 0; pw >>= 1) {
      if (idx + pw <= n && cur + bit[idx + pw] < target) {
        idx += pw;
        cur += bit[idx];
      }
    }
    return idx + 1;
  }
};
