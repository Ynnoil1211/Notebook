// 3b. Eliminacion Gaussiana Modular sobre primo MOD en O(N
// * M * min(N, M))
// Matriz aumentada 'a' de tamano N x (M + 1). Retorna 0
// (sin sol), 1 (unica), 2 (infinitas).
int gauss_mod(vector<vll> a, vll& ans, ll mod = MOD) {
  int n = sz(a), m = sz(a[0]) - 1;
  vi where(m, -1);
  for (int col = 0, row = 0; col < m && row < n; ++col) {
    int sel = -1;
    for (int i = row; i < n; ++i)
      if (norm_mod(a[i][col], mod) != 0) { sel = i; break; }
    if (sel == -1) continue;
    swap(a[sel], a[row]);
    where[col] = row;
    ll inv_piv = inv_fermat(a[row][col], mod);
    for (int j = col; j <= m; ++j)
      a[row][j] = norm_mod(a[row][j], mod) * inv_piv % mod;
    for (int i = 0; i < n; ++i)
      if (i != row && norm_mod(a[i][col], mod) != 0) {
        ll factor = norm_mod(a[i][col], mod);
        for (int j = col; j <= m; ++j)
          a[i][j] = sub_mod(a[i][j], factor * a[row][j],
            mod);
      }
    row++;
  }
  ans.assign(m, 0);
  for (int i = 0; i < m; ++i)
    if (where[i] != -1) ans[i] = norm_mod(a[where[i]][m],
      mod);
  for (int i = 0; i < n; ++i) {
    ll sum = 0;
    for (int j = 0; j < m; ++j)
      sum = (sum + ans[j] * norm_mod(a[i][j], mod)) % mod;
    if (sum != norm_mod(a[i][m], mod)) return 0;
  }
  for (int i = 0; i < m; ++i) if (where[i] == -1) return 2;
  return 1;
}
