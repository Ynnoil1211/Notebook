// 1. Segment Tree con Lazy Propagation (Suma en rango +
// Query Suma y Min)
struct LazySegTree {
  int n;
  vll sum_t, min_t, lazy;

  LazySegTree(const vll& a)
    : n(sz(a)), sum_t(4 * n, 0), min_t(4 * n, 0), lazy(
      4 * n, 0) {
    if (n > 0) build(1, 0, n - 1, a);
  }
  void apply(int v, int l, int r, ll val) {
    sum_t[v] += val * (r - l + 1);
    min_t[v] += val;
    lazy[v] += val;
  }
  void push(int v, int l, int r) {
    if (lazy[v] != 0) {
      int m = (l + r) / 2;
      apply(v * 2, l, m, lazy[v]);
      apply(v * 2 + 1, m + 1, r, lazy[v]);
      lazy[v] = 0;
    }
  }
  void pull(int v) {
    sum_t[v] = sum_t[v * 2] + sum_t[v * 2 + 1];
    min_t[v] = min(min_t[v * 2], min_t[v * 2 + 1]);
  }
  void build(int v, int l, int r, const vll& a) {
    if (l == r) { sum_t[v] = min_t[v] = a[l]; return; }
    int m = (l + r) / 2;
    build(v * 2, l, m, a);
    build(v * 2 + 1, m + 1, r, a);
    pull(v);
  }
  void add_range(int v, int l, int r, int ql, int qr,
    ll val) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) { apply(v, l, r, val); return; }
    push(v, l, r);
    int m = (l + r) / 2;
    add_range(v * 2, l, m, ql, qr, val);
    add_range(v * 2 + 1, m + 1, r, ql, qr, val);
    pull(v);
  }
  ll query_sum(int v, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return 0;
    if (ql <= l && r <= qr) return sum_t[v];
    push(v, l, r);
    int m = (l + r) / 2;
    return query_sum(v * 2, l, m, ql, qr)
         + query_sum(v * 2 + 1, m + 1, r, ql, qr);
  }
  ll query_min(int v, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return INF;
    if (ql <= l && r <= qr) return min_t[v];
    push(v, l, r);
    int m = (l + r) / 2;
    return min(query_min(v * 2, l, m, ql, qr),
               query_min(v * 2 + 1, m + 1, r, ql, qr));
  }
  void add_range(int ql, int qr, ll val) {
    add_range(1, 0, n - 1, ql, qr, val);
  }
  ll query_sum(int ql, int qr) {
    return query_sum(1, 0, n - 1, ql, qr); }
  ll query_min(int ql, int qr) {
    return query_min(1, 0, n - 1, ql, qr); }
};
