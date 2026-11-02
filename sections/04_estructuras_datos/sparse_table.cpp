struct SparseTable {
  int n, K;
  vector<vll> st;
  vi lg;

  ll op(ll a, ll b) const {
    // Cambiar a max(a, b) o std::gcd(a, b)
    return min(a, b);
  }

  SparseTable(const vll& a) {
    n = sz(a);
    K = __lg(max(1, n)) + 1;
    st.assign(K, vll(n));
    lg.assign(n + 1, 0);
    for (int i = 2; i <= n; ++i) lg[i] = lg[i / 2] + 1;
    st[0] = a;
    for (int j = 1; j < K; ++j)
      for (int i = 0; i + (1 << j) <= n; ++i)
        st[j][i] = op(st[j - 1][i], st[j - 1][i + (1 << (
          j - 1))]);
  }

  // Consulta en rango 0-indexed [L, R] inclusivo en O(1)
  ll query(int L, int R) const {
    int j = lg[R - L + 1];
    return op(st[j][L], st[j][R - (1 << j) + 1]);
  }
};
